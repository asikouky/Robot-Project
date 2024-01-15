#include "robot.h"
//Const Global Value
const uint16_t ROTATION_TIME = 1050;
const uint16_t ROTATION_180_TIME = 2500;
const uint16_t ADVANCE_ONE_SQUARE_TIME = 2500; //was 3700
const uint16_t ADJUST_ADVANCE_ROTATION_TIME = 1400; //was 1900
const uint16_t PAUSE = 300;
const uint16_t MOVE_OUT_LINE_DELAY = 500;
const uint16_t ADJUST_MOVING_TIME = 900;
//Changing Global Value
bool gCanUpdatePosition = true;

#define X 0
#define Y 1

namespace TimerHandler{
    bool gIsTimerOver = false;
}

Robot::Robot(uint8_t xPos, uint8_t yPos, Compass compass) :xPos_(xPos), yPos_(yPos), 
                                        compass_(compass), display_(&DDRC, &PORTC){}

//############################################
//########### ROBOT MOTION SECTION ###########
//############################################
void Robot::followLine(){
    // ## Update coord ##
    if (sensorLine.isRightTurn() && sensorLine.isLineRight() && !sensorLine.isLineBreak()
     && gCanUpdatePosition){
        lastCrossSide = Wheel::RIGHT;
        updatePosition();
        gCanUpdatePosition = false;
        TimerHandler::gIsTimerOver = false;
        timer1.restart();
    }
    else if (sensorLine.isLeftTurn() && sensorLine.isLineLeft() && !sensorLine.isLineBreak() 
    && gCanUpdatePosition){
        lastCrossSide = Wheel::LEFT;  
        updatePosition();
        gCanUpdatePosition = false;
        TimerHandler::gIsTimerOver = false;
        timer1.restart();
    }

    // ## Timer handler section for double tile ##
    if(TimerHandler::gIsTimerOver && gCanUpdatePosition){
        updatePosition();
        timer1.stop(); 
        TimerHandler::gIsTimerOver = false;
    }

    if(!sensorLine.isRightTurn() && !sensorLine.isLeftTurn() ){
        gCanUpdatePosition = true;
    }
    // ## Adjuste robot motion while following line ##
    if (sensorLine.isLineLeft() || sensorLine.isLeftTurn() ){
        navigation.incPWM(Wheel::RIGHT) ;
    }

    else if(sensorLine.isLineRight() || sensorLine.isRightTurn()){
        navigation.incPWM(Wheel::LEFT);
    }
    else{ 
        navigation.straightForward();
    }
    
}

void Robot::turnRobotWithAdvance(Wheel typeTurn, bool isAdjustNeeded){
    timer1.stop();
    navigation.stop();
    _delay_ms(PAUSE);
    if(isAdjustNeeded){
    navigation.straightForward();
    _delay_ms(ADJUST_ADVANCE_ROTATION_TIME);
    }

    navigation.stop();
    _delay_ms(PAUSE);

    typeTurn == Wheel::LEFT ? navigation.turnLeft() : navigation.turnRight();
    _delay_ms(MOVE_OUT_LINE_DELAY); //500ms, to move the sensor out of the straight line

    if(typeTurn == Wheel::LEFT ){
        while(!sensorLine.isLineLeft()){}
    }  
    else{
        while(!sensorLine.isLineRight()){}
    }
    navigation.straightForward();
    navigation.stop();
    _delay_ms(PAUSE);
    
    updateCompass(typeTurn);
    gCanUpdatePosition = false;
    TimerHandler::gIsTimerOver = false;
    timer1.restart();
}

void Robot::turn180(bool isAjustNeed){

    timer1.stop();
    navigation.stop();
    _delay_ms(PAUSE);

    if(isAjustNeed){
        navigation.straightForward();
        _delay_ms(ADJUST_ADVANCE_ROTATION_TIME);
    }

    navigation.turnLeft();
    _delay_ms(ROTATION_180_TIME);
    while(!sensorLine.isLineLeft()){}
    navigation.straightForward();
    navigation.stop();
    _delay_ms(PAUSE);

    updateCompass(Wheel::LEFT);
    updateCompass(Wheel::LEFT);
}

bool Robot::canFollowLine(){
    return !sensorLine.isLineBreak() || sensorLine.isLeftTurn() || sensorLine.isLineLeft() || sensorLine.isLineRight() || sensorLine.isRightTurn();
}
//End of robot motion section

//########################################
//########### POSITION SECTION ###########
//#########################################

void Robot::setCoord(uint8_t xPos, uint8_t yPos){
    xPos_ = xPos;
    yPos_ = yPos;
}

uint8_t Robot::getCoord(uint8_t coordChoice){
    if(coordChoice)
        return yPos_;
    else 
        return xPos_;
}

void Robot::updateCompass(Wheel typeTurn){
    if(typeTurn == Wheel::LEFT)
        compass_ = (Compass)((compass_ + 3) % 4);
    else
        compass_ = (Compass)((compass_ + 1) % 4);
    display_.clear();
    displayPosition();
    displayCompass();
}

void Robot::updatePosition(){
    switch(compass_){
        case Compass::NORTH:
            xPos_--;
            break;
        case Compass::EAST:
            yPos_++;
            break;
        case Compass::SOUTH:
            xPos_++;
            break;
        case Compass::WEST:
            yPos_--;
            break;
        default:
            break;
    }
    display_.clear();
    displayPosition();
    displayCompass();

    sensorLineTracker = {sensorLine.isLeftTurn(), sensorLine.isLineLeft(), sensorLine.isLineBreak(), sensorLine.isLineRight(), sensorLine.isRightTurn()};

}
//End of position section

//#########################################
//########### DISPLAY - SECTION ###########
//#########################################

void Robot::displayCompass(){
    switch (compass_){
        case Compass::NORTH:
            display_.write("NORTH", LCM_FW_HALF_CH);
            break;
        case Compass::EAST:
            display_.write("EAST", LCM_FW_HALF_CH);
            break;
        case Compass::SOUTH:
            display_.write("SOUTH", LCM_FW_HALF_CH);
            break;
        case Compass::WEST:
            display_.write("WEST", LCM_FW_HALF_CH);
            break;
        default:
            break;
    }
}

void Robot::displayPosition(){
    display_ << "(" << xPos_ << ", " << yPos_ << ")"; 
}


//End of find corner

//############################################################
//########### CHALLENGE #1.2 - FIND CORNER - SECTION #########
//############################################################

uint8_t Robot::getLenght(){
    timer1.stop();
    timer1.setComp(ADVANCE_ONE_SQUARE_TIME, Comparison1::A); 
    timer1.restart();

    bool hasRunnedOnce = false;
    while(canFollowLine()){
        followLine();
        if(!gCanUpdatePosition){
            if(!hasRunnedOnce){
                hasRunnedOnce = true;
                crossChecker[xPos_-2] = 1;
            }
        }
        else{
            hasRunnedOnce = false;
        }
    }
    timer1.stop();
    navigation.stop();
    _delay_ms(PAUSE);

     if(xPos_-1 == 1 && lastCrossSide == Wheel::LEFT){
        navigation.straightForward();
        _delay_ms(1000);
        navigation.stop();
        _delay_ms(PAUSE);

        navigation.turnLeft();
        _delay_ms(ROTATION_TIME);
        navigation.stop();
        _delay_ms(PAUSE);

        canFollowLine() ? xPos_ = xPos_ : xPos_++; 
        display_.clear();
        displayPosition();
        displayCompass();
        turnRobotWithAdvance(Wheel::LEFT, false);
    }

    return xPos_-1;
}

void Robot::returnOriginCorner(){
    if(compass_ == Compass::SOUTH){
        navigation.straightForward();
        _delay_ms(ADJUST_ADVANCE_ROTATION_TIME);
        navigation.stop();
        _delay_ms(PAUSE);

        turn180();
    }
    navigation.stop();
    _delay_ms(PAUSE);

    navigation.straightForward();
    
    while(canFollowLine()){ 
        followLine();
    }
    navigation.stop();
    _delay_ms(PAUSE);

    navigation.straightForward();
    _delay_ms(ADJUST_ADVANCE_ROTATION_TIME);
    navigation.stop();
    _delay_ms(PAUSE);

    turn180();
    navigation.stop();
    _delay_ms(PAUSE);
}

void Robot::setFoundCorner(uint8_t lenght){
    
lastCrossSide == Wheel::LEFT ? lastCrossSide = Wheel::RIGHT : lastCrossSide = Wheel::LEFT;
    
switch (lenght){
        case 1:
            if(lastCrossSide == Wheel::LEFT){
                xPos_ = 1;
                yPos_ = 1;
                compass_ = Compass::SOUTH;
            }
            else{
                xPos_ = 4;
                yPos_ = 1;
                compass_ = Compass::NORTH;
            }
            break;

        case 2:
            if(crossChecker[0]){ //If the first coord was updated because of crossing an intersection vs timer
                xPos_ = 1;
                yPos_ = 7;
                compass_ = Compass::WEST;              
            }
            else{
                xPos_ = 4;
                yPos_ = 1;
                compass_ = Compass::EAST;
            }
            break;

        case 3:
            if(lastCrossSide == Wheel::LEFT){
                xPos_ = 4;
                yPos_ = 7;
                compass_ = Compass::NORTH;
            }
            else{
                if(crossChecker[0] && !crossChecker[1]){ 
                    xPos_ = 1;
                    yPos_ = 7;
                    compass_ = Compass::SOUTH;
                }
                else if(crossChecker[0] && crossChecker[1]){
                    xPos_ = 4;
                    yPos_ = 7;
                    compass_ = Compass::WEST;                   
                }
                else{
                    xPos_ = 1;
                    yPos_ = 1;
                    compass_ = Compass::EAST;                            
                }
            }
            break;
            
        default:
            xPos_ = lenght;
            yPos_ = 0;
            compass_ = Compass::SOUTH;
    }
    display_.clear();
    displayPosition();
    displayCompass();
}
//End of find corner

//##################################################################
//########### CHALLENGE #2 - FIND SHORTEST WAY - SECTION ###########
//##################################################################

void Robot::towardsNextDirection(bool ajusting){
    timer1.stop();
    Compass nextDirection = gps.nextDirection(xPos_, yPos_);
    if (nextDirection == compass_){
        timer1.stop();
        timer1.setComp(ADJUST_MOVING_TIME);
        timer1.restart();
        while(!TimerHandler::gIsTimerOver){
            gCanUpdatePosition = false;
            followLine();
        }
        timer1.stop();
        TimerHandler::gIsTimerOver = false;
        timer1.setComp(ADVANCE_ONE_SQUARE_TIME);
        return;
    }

    uint8_t directionChange;
    if (nextDirection < compass_)
        directionChange = nextDirection +4 - compass_;
    else
        directionChange = nextDirection - compass_;

    switch(directionChange){
        case 1:
            turnRobotWithAdvance(Wheel::RIGHT,ajusting);
            return;
        case 2:
            turn180();
            return;
        case 3:
            turnRobotWithAdvance(Wheel::LEFT,ajusting);
            return;
    }
}

bool Robot::checkForPole(uint8_t xEnd, uint8_t yEnd){
    if (sharpIR.isObjectDetected()){
            timer1.stop();
            navigation.stop();
            gps.placePole(xPos_, yPos_, compass_);
            gps.setItinerary(xPos_, yPos_, xEnd, yEnd);
            piezo.emitFor(55,20);
            towardsNextDirection(false);
            return true;
        }
    return false;
}

void Robot::challenge2(uint8_t xEnd, uint8_t yEnd){
    display_.clear();
    displayPosition();
    displayCompass();

    uint8_t xPos = xPos_;
    uint8_t yPos = yPos_;

    gps.setItinerary(xPos_, yPos_, xEnd, yEnd);
    towardsNextDirection(false);
    checkForPole(xEnd, yEnd);
    
    timer1.setComp(ADVANCE_ONE_SQUARE_TIME,Comparison1::A);
    timer1.restart();

    while(xEnd != xPos_ || yEnd != yPos_){
        if(xPos != xPos_ || yPos != yPos_){
            towardsNextDirection();
            xPos = xPos_;
            yPos = yPos_;
            checkForPole(xEnd, yEnd);
            timer1.restart();
        }
        followLine();
    }
    turn180(true);

    piezo.emitFor(70,2);
    _delay_ms(100);
    piezo.emitFor(70,2);
    _delay_ms(100);
    piezo.emitFor(70,2);
    _delay_ms(100);
    piezo.emitFor(70,2);
    _delay_ms(100);
    piezo.emitFor(70,2);
}