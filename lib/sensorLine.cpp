#include "sensorLine.h"

const uint8_t MASK_SENSORS = (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4) | (1 << PA5);
const uint8_t MASK_S1 =(1 << PA1);
const uint8_t MASK_S2 =(1 << PA2);
const uint8_t MASK_S3 =(1 << PA3);
const uint8_t MASK_S4 =(1 << PA4);
const uint8_t MASK_S5 =(1 << PA5);

SensorLine::SensorLine(){
    DDRA &= ~MASK_SENSORS;
}

bool SensorLine::isLineLeft(){
    return (PINA & MASK_S2);
}

bool SensorLine::isLineRight(){
    return (PINA & MASK_S4);
}

bool SensorLine::isLeftTurn(){
    return (PINA & MASK_S1);
}

bool SensorLine::isRightTurn(){
    return (PINA & MASK_S5);
}

bool SensorLine::isLineBreak(){
    return !(PINA & MASK_S3);
}

bool SensorLine::lineFound(){
    return (PINA & MASK_S1) || (PINA & MASK_S2) || (PINA & MASK_S3) || (PINA & MASK_S4) || (PINA & MASK_S5);
}