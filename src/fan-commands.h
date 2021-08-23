#ifndef _COMMANDS
#define _COMMANDS

#include <Arduino.h>

typedef enum FanSpeed
{
    FAN_OFF,
    FAN_LOW,
    FAN_MED,
    FAN_HIGH
} FanSpeed;

typedef enum Light
{
    LIGHT_OFF,
    LIGHT_ON
} Light;

void transmit(uint16_t pulseWidth, uint8_t *data, uint8_t len);
void setOfficeLight(bool shouldBeOn);
void setOfficeFanSpeed(FanSpeed speed);
void setOfficeFan(bool shouldBeOn);
void setBedroomLight(bool shouldBeOn);
void setBedroomFan(bool shouldBeOn);

#endif