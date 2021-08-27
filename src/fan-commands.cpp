#include "fan-commands.h"
#include "../constants.h"
#include "../data.h"

FanSpeed stateBedroomFan = FAN_OFF;
Light stateBedroomLight = LIGHT_OFF;
FanSpeed stateOfficeFan = FAN_OFF;
Light stateOfficeLight = LIGHT_OFF;

void transmit(uint16_t pulseWidth, uint8_t *data, uint8_t len)
{
    const uint16_t delayNeeded = pulseWidth - WRITE_TIME;

    for (uint8_t i = 0; i < len; i++)
    {
        const uint8_t b = data[i];
        for (uint8_t j = 8; j > 0; j--)
        {
            if (bitRead(b, j - 1) == 1)
            {
                digitalWrite(TX_PIN, HIGH);
            }
            else
            {
                digitalWrite(TX_PIN, LOW);
            }

            delayMicroseconds(delayNeeded);
        }
    }

    // Repeat to make sure its heard
    for (uint8_t i = 0; i < len; i++)
    {
        const uint8_t b = data[i];
        for (uint8_t j = 8; j > 0; j--)
        {
            if (bitRead(b, j - 1) == 1)
            {
                digitalWrite(TX_PIN, HIGH);
            }
            else
            {
                digitalWrite(TX_PIN, LOW);
            }

            delayMicroseconds(delayNeeded);
        }
    }
}

void setOfficeLight(bool shouldBeOn)
{
    if (stateOfficeLight == LIGHT_ON && !shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
        stateOfficeLight = LIGHT_OFF;
    }
    else if (stateOfficeLight == LIGHT_OFF && shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
        stateOfficeLight = LIGHT_ON;
    }
}

void setOfficeFanSpeed(FanSpeed speed)
{
    if (stateOfficeFan != speed)
    {
        switch (speed)
        {
        case FAN_OFF:
            transmit(DATA_PULSE_WIDTH, officeOffSpeed, sizeof(officeOffSpeed));
            break;
        case FAN_LOW:
            transmit(DATA_PULSE_WIDTH, officeLowSpeed, sizeof(officeLowSpeed));
            break;
        case FAN_HIGH:
            transmit(DATA_PULSE_WIDTH, officeHighSpeed, sizeof(officeHighSpeed));
            break;
        }

        stateOfficeFan = speed;
    }
}

void setOfficeFan(bool shouldBeOn)
{
    if (shouldBeOn)
    {
        setOfficeFanSpeed(FAN_LOW);
    }
    else
    {
        setOfficeFanSpeed(FAN_OFF);
    }
}

void setBedroomLight(bool shouldBeOn)
{
    if (stateBedroomLight == LIGHT_ON && !shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, bedLight, sizeof(bedLight));
        stateBedroomLight = LIGHT_OFF;
    }
    else if (stateOfficeLight == LIGHT_OFF && shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, bedLight, sizeof(bedLight));
        stateBedroomLight = LIGHT_ON;
    }
}

void setBedroomFan(bool shouldBeOn)
{
    if (stateBedroomFan == FAN_MED && !shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, bedFan, sizeof(bedFan));
        stateBedroomFan = FAN_OFF;
    }
    else if (stateBedroomFan == FAN_OFF && shouldBeOn)
    {
        transmit(DATA_PULSE_WIDTH, bedFan, sizeof(bedFan));
        stateBedroomFan = FAN_MED;
    }
}
