/*
Travail: FINAL
Section: 01
Équipe: 0305
Correcteur: Ghali Chraibi
*/

#ifndef LED
#define LED
#include <avr/io.h>
#include <util/delay.h>

/*
Description du programme:
La classe Led permet d'allumer la del en choississant le port.

Identifications matérielles: 
    - Broches B4 et B5 relies a la del .
*/

class Led
{
public:
    Led(volatile uint8_t *portx, volatile uint8_t *ddrx, volatile uint8_t *pinx_, uint8_t firstPos, uint8_t secondPos);
    ~Led();

    void setGreen();
    void setRed();
    void setAmber(uint16_t delay);
    void ledToggleOff(uint8_t position);
    void setOff();

private:
    volatile uint8_t *portx_;
    volatile uint8_t *ddrx_;
    volatile uint8_t *pinx_;
    const uint8_t firstPos_;
    const uint8_t secondPos_;
    static constexpr uint8_t DELAY_AMBER_ANIMATION_ = 22;
    uint16_t DOUBLE_DELAY_AMBER_ANIMATION_ = 2 * DELAY_AMBER_ANIMATION_;
    uint16_t delay_counter = 1;
};
#endif