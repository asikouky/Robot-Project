/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef SHARPIR
#define SHARPIR
#include <avr/io.h>
#include "can.h"

/*
Description du programme:


Identifications matérielles: 
    - 
*/

class SharpIR{

public: 
    SharpIR();
    bool isObjectDetected();
private:
    uint8_t sensorValue = 0;
    can converter;
};
#endif