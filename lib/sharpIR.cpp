#include "sharpIR.h"
#include "navigation.h"
const uint8_t baseSensorValue = 30; 
const uint8_t MASK_IR = (1 << PA0);
const uint8_t DIVIDER = 10;
const uint8_t DELAY_BETWEEN_SENSOR_TAKE = 15;

SharpIR::SharpIR(){
    DDRA &= ~MASK_IR;
}

bool SharpIR::isObjectDetected(){
    uint16_t sommeValue = 0;

    for(int i = 0; i < DIVIDER; i++){
        sensorValue = (converter.lecture(0) >> 2);
        sommeValue += sensorValue;
        _delay_ms(DELAY_BETWEEN_SENSOR_TAKE);
    }
    sensorValue = sommeValue/DIVIDER;

    return sensorValue >= baseSensorValue;
}