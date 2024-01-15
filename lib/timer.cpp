#include "timer.h"

const TimerRegisters timer0Registers = {&TCNT0, &TCCR0A, &TCCR0B, &OCR0A, &OCR0B, &TIMSK0};
const TimerRegisters timer2Registers = {&TCNT2, &TCCR2A, &TCCR2B, &OCR2A, &OCR2B, &TIMSK2};

const uint8_t MASK_PWM_MODE = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
const uint8_t MASK_CTC_MODE = (1 << WGM01) | (1 << COM0B0);
const uint8_t MASK_COMPAREA_INT = (1 << OCIE0A);

Timer::Timer(const bool choice){
    if (choice)
        timerRegisters = timer2Registers;
    else
        timerRegisters = timer0Registers;
    cli();
    //Prescaler a 0 pour arreter le compteur
    *timerRegisters.modeB = 0;
    sei();
}
Timer::~Timer(){
    *timerRegisters.interruption = 0;
    *timerRegisters.modeB = 0;
}

void Timer::restart(){
    *timerRegisters.counter = 0;
}

void Timer::stop(){
    *timerRegisters.modeB = 0;
    *timerRegisters.interruption = (1 << OCIE1B);
    *timerRegisters.modeA = 0;
}
void Timer::toPWM(const uint8_t cycles, const uint8_t prescaler){
    *timerRegisters.interruption = 0;
    *timerRegisters.compA = cycles;
    *timerRegisters.compB = cycles;
    *timerRegisters.modeA = MASK_PWM_MODE;
    *timerRegisters.modeB = prescaler;
}

void Timer::toCTC(const uint8_t comparison, const uint8_t prescaler, Comparison comparisons){
    cli();
    *timerRegisters.counter = 0;
    comparisons == Comparison::A ? *timerRegisters.compA = comparison : *timerRegisters.compB = comparison;
    *timerRegisters.modeA = MASK_CTC_MODE;
    *timerRegisters.modeB = prescaler;
    sei();
}

void Timer::setInterrupt(){
    *timerRegisters.counter = 0;
    //*timerRegisters.modeA = (1 << WGM01) | (1 << COM0B0) ;
    *timerRegisters.modeB = (1 << CS02) | (1 << CS00);;
    *timerRegisters.interruption = (1 << OCIE1B);
}

void Timer::setToggle(){
    *timerRegisters.modeA |= (1 << COM0A0);
}

void Timer::setComp(const uint8_t comparison, const Comparison comparisons){
    if (comparisons == Comparison::A || comparisons == Comparison::BOTH)
        *timerRegisters.compA = comparison;
    if (comparisons == Comparison::B || comparisons == Comparison::BOTH)
        *timerRegisters.compB = comparison;

}
