#include "interface.h"

const uint8_t MASK_SELECTION = (1 << PA7);
const uint8_t MASK_VALIDATION = (1 << PD3);
const uint8_t MASK_INTERRUPT = (1 << PD2);

const uint8_t LINE_NB= 4;
const uint8_t COLUMN_NB = 7;

const uint16_t DELAY_BEFORE_START = 2000;
const uint8_t DELAY_LCD_DISPLAY = 200;
Interface::Interface(): disp_(&DDRC, &PORTC), led(&PORTB, &DDRB, &PINB, PB4, PB5){
    //PIN D2 pour la selection
    DDRD &= ~MASK_VALIDATION;
    //PIN D3 pour la validation
    DDRA &= ~MASK_SELECTION;
    DDRD &= ~MASK_INTERRUPT;
    // Led led = Led(&PORTB, &DDRB, &PINB, PB4, PB5);
    //initInterruptSelection();
    initInterruptValidation();
}

void Interface::initInterruptSelection(){
    cli();
    EICRA |= (1 << ISC01);
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);
    sei();
}

void Interface::initInterruptValidation(){
    cli();
    EICRA |= (1 << ISC11);
    EICRA |= (1 << ISC10);
    EIMSK |= (1 << INT1);
    sei();
}

bool Interface::InterruptIsPressed(){
    bool pressed = PIND & (1 << PD2);
    _delay_ms(30);
    return pressed && (PIND &(1 << PD2));
}

bool Interface::SelectIsPressed(){
    bool pressed = !(PINA & (1 << PA7));
    if(pressed){
        _delay_ms(30);
        return !(PINA &(1 << PA7));
    }
    return false;
}


void Interface::incrementLine(){
    counter_select = 1;
    bool wasPressed = false;
    disp_.clear();
    disp_.write("LIGNE");
    disp_[15] = " ";
    disp_<< counter_select;
    do
    {
        if(SelectIsPressed() && !wasPressed){
            counter_select = counter_select % LINE_NB + 1;
            disp_.clear();
            disp_.write("LIGNE");
            disp_[15] = " ";
            disp_<< counter_select;
            _delay_ms(DELAY_LCD_DISPLAY);
            wasPressed = true;
        }
        else if(!SelectIsPressed()){
            wasPressed = false;
        }
    } while (!valid);
    x_position = counter_select;
}

void Interface::incrementColumn(){
    counter_select = 1;
    bool wasPressed = false;
    disp_.clear();
    disp_.write("COLONNE");
    disp_[15] = " ";
    disp_<< counter_select;
    do
    {
        if(SelectIsPressed() && !wasPressed){
            counter_select = counter_select % COLUMN_NB + 1;
            disp_.clear();
            disp_.write("COLONNE");
            disp_[15] = " ";
            disp_<< counter_select;
            _delay_ms(DELAY_LCD_DISPLAY);
            wasPressed = true;
        }
        else if(!SelectIsPressed()){
            wasPressed = false;
        }
    }while (!valid);
    y_position = counter_select;
}         

void Interface::yesOrNo(){
    counter_select = 0;
    choix = YesOrNo::YES;
    bool wasPressed = false;
    disp_.clear();
    disp_ << "(" << x_position << ", " << y_position << ")" << "OK?" ;
    disp_.write("OUI", LCM_FW_HALF_CH);
    do
    {
        if(SelectIsPressed() && !wasPressed){
            counter_select = (counter_select + 1) % 2;
            if(counter_select == 0){
                disp_.write("OUI", LCM_FW_HALF_CH);
                choix = YesOrNo::YES;
            }
            else{
                disp_.write("NON", LCM_FW_HALF_CH);
                choix = YesOrNo::NO;
            }
            _delay_ms(DELAY_LCD_DISPLAY);
            wasPressed = true;
        }
        else if(!SelectIsPressed()){
            wasPressed = false;
        }
    }while(!valid);
}

void Interface::executeState(){
    switch(buttonState){
        case ValidationState::LINE:
            incrementLine();
            break;
        case ValidationState::COLUMN:
            incrementColumn();
            break;
        case ValidationState::FINAL_VALIDATION:
            yesOrNo();
            break;
    }
}

void Interface::goNextState(){
    switch(buttonState){
        case ValidationState::LINE:
            buttonState = ValidationState::COLUMN;
            break;
        case ValidationState::COLUMN:
            buttonState = ValidationState::FINAL_VALIDATION;
            break;
        case ValidationState::FINAL_VALIDATION:
            if(choix == YesOrNo::YES){  
                robot.challenge2(x_position, y_position);
                buttonState = ValidationState::LINE;
            }
            else if (choix == YesOrNo::NO){
                buttonState = ValidationState::LINE;
            }
            break;
    }
    _delay_ms(DELAY_LCD_DISPLAY);
    valid = false;
}

void Interface::choseChallenge(){
    disp_.clear();
    disp_.write("CHOOSE CHALLENGE");
    while(true){
        led.setRed();
        if(InterruptIsPressed()){
            led.setOff();
            disp_.clear();
            disp_.write("CHALLENGE 1");
            _delay_ms(DELAY_BEFORE_START);
            disp_.clear();

            Timer timer0 = Timer(0);
            timer0.setInterrupt();
            timer0.setComp(195, Comparison::B); //0,025s

            uint8_t length = robot.getLenght();
            timer0.stop();

            Piezo piezo;
            piezo.emitFor(75, 10);

            timer0.setInterrupt();
            timer0.restart();
            timer0.setComp(1, Comparison::B);
            robot.returnOriginCorner();
            robot.setFoundCorner(length);
            break;
        }
        else if(SelectIsPressed()){
            led.setGreen();
            disp_.clear();
            disp_.write("CHALLENGE 2");
            _delay_ms(DELAY_BEFORE_START);
            disp_.clear();
            led.setOff();
            while(true) {
                executeState();
                goNextState();
            }
            break;
        }
    }
}
