#define F_CPU 8000000UL
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "interface.h"

Interface interface;

ISR (TIMER1_COMPA_vect) {
     TimerHandler::gIsTimerOver = true;
}

volatile uint8_t gTimer0Counteur = 0;
ISR (TIMER0_COMPB_vect) {
     Led led = Led(&PORTB, &DDRB, &PINB, PB4, PB5);
    if(gTimer0Counteur == 4){
        led.ledToggleOff(PB4);
        gTimer0Counteur = 0;
    }
    else{
        gTimer0Counteur++;
    }
}

ISR (INT1_vect) {
    interface.valid = true;
    _delay_ms(50);
    EIFR |= (1 << INT1);
}

int main(){
    interface.choseChallenge();
}