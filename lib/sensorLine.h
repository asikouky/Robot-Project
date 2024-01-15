/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef SENSORLINE_H
    #define SENSORLINE_H

#include <avr/io.h> 

/*
Description du programme:


Identifications matérielles: 
    - 
*/

struct SensorLineTracker
{
    bool farLeft;
    bool middleLeft;
    bool middle;
    bool middleRight;
    bool farRight;
};

class SensorLine{

public:
    SensorLine();

    bool isLineLeft(); 
    bool isLineRight();
    bool isLeftTurn();
    bool isRightTurn();
    bool isLineBreak();
    bool lineFound();
};
#endif