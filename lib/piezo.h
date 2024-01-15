/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#include "timer.h"
#pragma once

/*
Description du programme:


Identifications matérielles: 
    - 
*/

class Piezo{
    private:
        Timer timer_ = Timer(0);
    public:
        Piezo();
        void emit(uint8_t note);
        void pause();
        void emitFor(uint8_t note, uint8_t time);

};