/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef TIMER
#define TIMER
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/*
Description du programme:


Identifications matérielles: 
    - 
*/

typedef volatile uint8_t* Register;

struct TimerRegisters{
    Register counter, modeA, modeB, compA, compB, interruption;
};

enum class Comparison {A,B,BOTH};

class Timer {
private:
    TimerRegisters timerRegisters;

public:
    Timer(const bool choice);
    ~Timer();
    void restart();
    void stop();
    void toPWM(const uint8_t cycles, const uint8_t prescaler = 3);
    void toCTC(const uint8_t comparison, const uint8_t prescaler = 1, Comparison comparisons = Comparison::BOTH);
    void setComp(const uint8_t comparison, const Comparison comparisons = Comparison::BOTH);
    void setInterrupt();
    void setToggle();
};
#endif