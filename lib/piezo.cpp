#include "piezo.h"
const uint8_t MASK_PWM = (1 << PB3);
const uint8_t MASK_GND = (1 << PB1);
const uint16_t TIME_SCALE = 100;
const uint8_t MAX_NOTE_64 = 60;
const uint16_t NOTES[37] = {
110,
117,
123,
131,
139,
147,
156,
165,
175,
185,
196,
208,
220,
233,
247,
262,
277,
294,
311,
330,
349,
370,
392,
415,
440,
466,
494,
523,
554,
587,
622,
659,
698,
740,
784,
831,
880,
};

Piezo::Piezo(){
    DDRB |= MASK_PWM | MASK_GND;
    PORTB &= ~MASK_GND;
}

void Piezo::emit(uint8_t note){
    uint16_t comparison = 8000000/NOTES[note-45]/2;
    uint8_t prescaler = 4;
    if (note > MAX_NOTE_64){
        prescaler = 3;
        comparison /= 64;
    }
    else
        comparison /= 256;
    timer_.toCTC(comparison,prescaler, Comparison::A);
    timer_.setToggle();
}

void Piezo::emitFor(uint8_t note, uint8_t time){
    emit(note);
    for (uint8_t i =0; i < time; i++)
        _delay_ms(TIME_SCALE);
    pause();
}
void Piezo::pause(){
    timer_.toCTC(0,0, Comparison::A);
}