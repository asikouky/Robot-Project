/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef ROBOT
#define ROBOT
#include <avr/io.h> 
#include <util/delay.h>
#include "navigation.h"
#include "sensorLine.h"
#include "sharpIR.h"
#include "timer1.h"
#include "piezo.h"
#include "gps.h"
#include "lcm_so1602dtr_m_fw.h"
#include "lcm_so1602dtr_m.h"

/*
Description du programme:


Identifications matérielles: 
    - 
*/

namespace TimerHandler{
   extern bool gIsTimerOver;

}

class Robot{
    public:
        Robot(uint8_t xPos = 1, uint8_t yPos = 1, Compass compass = Compass::SOUTH);
        ~Robot() = default;
        //Motion
        void followLine();
        void turnRobotWithAdvance(Wheel typeTurn, bool isAdjustNeeded = true);
        void turn180(bool isAjustNeed = false);
        bool canFollowLine();

        //Position
        void updateCompass(Wheel typeTurn);
        void updatePosition();
        void setCoord(uint8_t xPos, uint8_t yPos);
        uint8_t getCoord(uint8_t coordChoice);

        //LCD
        void displayPosition();
        void displayCompass();

        //Challenge #1.2
        uint8_t getLenght();
        void returnOriginCorner();
        void setFoundCorner(uint8_t lenght);

        bool checkForPole(uint8_t xEnd, uint8_t yEnd);
        void challenge2(uint8_t xEnd, uint8_t yEnd);
        void towardsNextDirection(bool ajusting = true);
 
    private:
        Navigation navigation;
        SensorLine sensorLine;
        SensorLineTracker sensorLineTracker;
        SharpIR sharpIR;
        Timer1 timer1;
        GPS gps;
        LCM display_;
        Piezo piezo;
        uint8_t xPos_;
        uint8_t yPos_;
        Compass compass_;

        uint8_t crossChecker[3] = {0,0,0};
        Wheel lastCrossSide;
        
};

#endif
