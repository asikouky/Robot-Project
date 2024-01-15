/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef NAVIGATION_H
    #define NAVIGATION_H

#include <avr/io.h> 
#include "timer.h"
#include <stdint.h>

/*
Description du programme:
Le programme permet le déplacement du robot. 
Elle utilise les registres qui génèrent le PWM, configurés dans la classe timer, 
afin de permettre la mobilité des roues.

Identifications matérielles: 
    - Broches PORT D4 et D5 (en sortie) poiur indiquer le sens  
    - Broches PORT D6 et D7 (en sortie) pour le signal PWM des roues
*/

enum class Wheel {RIGHT, LEFT};

class Navigation{


public:
    Navigation();
    ~Navigation() = default;

    void setPWM(uint8_t newDutyCycle);
    void incPWM(Wheel wheel);
    void turnLeft(bool boost = true);
    void turnLeft90();
    void turnRight(bool boost = true);
    void turnRight90();
    void straightForward(); 
    void straightBackward();
    void stop();

private:
    uint8_t dutyCycle_;
    Timer timer_ = Timer(2); 
};
#endif