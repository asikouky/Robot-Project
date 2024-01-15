/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef TIMER1
#define TIMER1
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
typedef volatile uint16_t* BigRegister;

enum class Comparison1 {A,B,BOTH}; //Maybe creating a namespace 

class Timer1{
private:
    BigRegister counter = &TCNT1;

public:
    Timer1();
    ~Timer1();
    void restart();
    void stop();
    void setComp(const uint16_t time, const Comparison1 comparisons = Comparison1::BOTH);
    uint16_t delayToCycles(uint16_t miliseconds);
};
#endif