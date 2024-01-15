#include "timer1.h"

const uint8_t MASK_PRESCALER_1024 = (1 << CS12) | (1 << CS10);
const uint8_t MASK_CTC = (1 << WGM02);
const uint8_t MASK_INTERRUPTION = (1 << OCIE1A);
Timer1::Timer1(){
    cli();
    //Prescaler 0 pour arreter le compteur
    *counter = 0;
    TCCR1B = MASK_CTC;
    TIMSK1 = 0;
    sei();
}
Timer1::~Timer1(){}

void Timer1::restart(){
    cli();
    *counter = 0;
    TCCR1B = MASK_CTC | MASK_PRESCALER_1024;
    TIMSK1 = MASK_INTERRUPTION;
    sei();
}

void Timer1::stop(){
    cli();
    TCCR1B = MASK_CTC;
    TIMSK1 = 0;
    sei();
}

void Timer1::setComp(const uint16_t time, const Comparison1 comparisons){
    cli();
    *counter = 0;
    uint16_t numCycles = delayToCycles(time);
    if (comparisons == Comparison1::A || comparisons == Comparison1::BOTH)
        OCR1A = numCycles;
    if (comparisons == Comparison1::B || comparisons == Comparison1::BOTH)
        OCR1B = numCycles;
    sei();
}

uint16_t Timer1::delayToCycles(uint16_t miliseconds){
    if(miliseconds >= 8300) //The max is 8388.308 ms for 16 bits and prescaler 1024
        miliseconds = 8300;
    return (8000000/1024/1000) * miliseconds;
}