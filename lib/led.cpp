#include "led.h"

Led::Led(volatile uint8_t *portx,
         volatile uint8_t *ddrx,
         volatile uint8_t *pinx,
         uint8_t firstPos,
         uint8_t secondPos) : portx_(portx),
                           ddrx_(ddrx),
                           pinx_(pinx),
                           firstPos_(firstPos),
                           secondPos_(secondPos)
{
    *ddrx_ |= (1 << firstPos) | (1 << secondPos);
}

Led::~Led() {}

void Led::setGreen()
{
    *portx_ |= (1 << firstPos_);
    *portx_ &= ~(1 << secondPos_);
}

void Led::setRed()
{
    *portx_ |= (1 << secondPos_);
    *portx_ &= ~(1 << firstPos_);
}

void Led::ledToggleOff(uint8_t pos){
    if(*pinx_ & (1 << pos))
        *portx_ &= ~(1 << pos);
    else
        *portx_ |= (1 << pos);
}

void Led::setAmber(uint16_t delay)
{
    while (delay_counter < delay) {
        setGreen();
        _delay_ms(DELAY_AMBER_ANIMATION_);
        setRed();
        _delay_ms(DELAY_AMBER_ANIMATION_);
        delay_counter += DOUBLE_DELAY_AMBER_ANIMATION_;
    }
    setOff();
}

void Led::setOff()
{
    *portx_ &= ~(1 << firstPos_);
    *portx_ &= ~(1 << secondPos_);
}