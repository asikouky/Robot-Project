/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef INTERFACE_H
#define INTERFACE_H
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcm_so1602dtr_m_fw.h"
#include "lcm_so1602dtr_m.h"
#include "led.h"
#include "stdlib.h"
#include "robot.h"
#include "piezo.h"

/*
Description du programme:
Le programme utilise une machine à état.
Le programme lit la pin D3, qui relie la sortie du circuit électrique à la carte mère, 
et détermine le résultat en réponse à l'appui du bouton. 
Il utilise également des interruptions pour gérer les délais et les événements associés au bouton.

Identifications matérielles: 
    - Broches PIN D2 et A7 (en entrée - relié au bouton-poussoir du bread board)

Table des états:

+--------------+----------------+----------------+-----+-----+---
| État courant        | État futur (0)  | État futur (1)   | D3 |
+--------------+----------------+----------------+-----+-----+---
| LINE                | LINE            | COLUMN           | 1  |
| COLUMN              | COLUMN          | FINAL_VALIDATION | 1  |
| FINAL_VALIDATION    | FINAL_VALIDATION| LINE             | 1  | 
+--------------+----------------+----------------+-----+-----+---

*/

enum class YesOrNo {YES, NO};

enum class ValidationState {LINE, COLUMN, FINAL_VALIDATION};

class Interface{

public:
    Interface();

    void initInterruptSelection();
    void initInterruptValidation();
    void incrementLine();
    void incrementColumn();
    bool InterruptIsPressed();
    bool SelectIsPressed();
    void executeState();
    void goNextState();
    void yesOrNo();
    void choseChallenge();

    bool isPressed_selection = false;

    bool valid = false;
    uint8_t interrupt = 0;
    uint8_t counter_select = 1;

private:
    uint8_t x_position = 1;
    uint8_t y_position = 1;
    LCM disp_; 
    YesOrNo choix;
    ValidationState buttonState = ValidationState::LINE;
    Robot robot;
    Led led;
};

#endif