#include "navigation.h"
#include "timer.h"

const uint8_t MASK_DIRECTION = (1 << PD4) | (1 << PD5);
const uint8_t MASK_PWM = (1 << PD6) | (1 << PD7);
const uint8_t MASK_LEFT_DIRECTION = (1 << PD4);
const uint8_t MASK_RIGHT_DIRECTION = (1 << PD5);
const uint16_t TURN_DELAY = 1650; 
const uint8_t TURN_DUTYCYCLE = 31;
const uint8_t MAX_CYCLES = 255;
const uint8_t REAJUST_INCREMENT = 15;
const uint8_t BASE_SPEED = 35;

Navigation::Navigation() : dutyCycle_(BASE_SPEED){         
    DDRD |= MASK_DIRECTION | MASK_PWM;
    PORTD |= MASK_DIRECTION;
    timer_.toPWM(0);
}

void Navigation::setPWM(uint8_t newDutyCycle){
    timer_.setComp((newDutyCycle *MAX_CYCLES) / 100);
    dutyCycle_ = newDutyCycle;
}

void Navigation::incPWM(Wheel wheel){
    if(wheel == Wheel::RIGHT){
        timer_.setComp(((dutyCycle_+REAJUST_INCREMENT) *MAX_CYCLES) / 100, Comparison::A);
        timer_.setComp((dutyCycle_ *MAX_CYCLES) / 100, Comparison::B);
    }
    if(wheel == Wheel::LEFT){
        timer_.setComp(((dutyCycle_+REAJUST_INCREMENT) *MAX_CYCLES) / 100, Comparison::B);
        timer_.setComp((dutyCycle_ *MAX_CYCLES) / 100, Comparison::A);
    }
}

void Navigation::turnLeft(bool boost){
    PORTD |= MASK_RIGHT_DIRECTION;
    PORTD &= ~MASK_LEFT_DIRECTION;
    if(boost){
        timer_.setComp(200, Comparison::BOTH);
        _delay_ms(150);
    }
    timer_.setComp(((TURN_DUTYCYCLE *255) / 100), Comparison::BOTH);
}


void Navigation::turnLeft90(){
    stop();
    turnLeft();
    _delay_ms(TURN_DELAY);
    stop();
}

void Navigation::turnRight(bool boost){
    PORTD |= MASK_LEFT_DIRECTION;
    PORTD &= ~MASK_RIGHT_DIRECTION;
    if(boost){
        timer_.setComp(200, Comparison::BOTH);
        _delay_ms(150);
    }
    timer_.setComp(((TURN_DUTYCYCLE *255) / 100), Comparison::BOTH);
}

void Navigation::turnRight90(){
    stop();
    turnRight();
    _delay_ms(TURN_DELAY);
    stop();
}

void Navigation::straightForward(){
    timer_.setComp((dutyCycle_ *MAX_CYCLES) / 100);
    PORTD |= MASK_DIRECTION;
}

void Navigation::straightBackward(){
    timer_.setComp((dutyCycle_ *MAX_CYCLES) / 100);
    PORTD &= ~MASK_DIRECTION;
}

void Navigation::stop(){
    timer_.setComp(0);
}