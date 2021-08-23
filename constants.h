#ifndef _constants
#define _constants

#define TX_PIN 5
#define DATA_PULSE_WIDTH 400

const uint8_t WRITE_TIME = 0;
const uint16_t PULSE_WIDTH = 400;

#define ROOM_BED "bedroom"
#define ROOM_OFFICE "office"
#define FAN "fan"
#define LIGHT "light"

#define BEDROOM_FAN "thor/" APP_NAME "/" ROOM_BED "/" FAN "/state"
#define BEDROOM_LIGHT "thor/" APP_NAME "/" ROOM_BED "/" LIGHT "/state"
#define OFFICE_FAN "thor/" APP_NAME "/" ROOM_OFFICE "/" FAN "/state"
#define OFFICE_LIGHT "thor/" APP_NAME "/" ROOM_OFFICE "/" LIGHT "/state"

#define STATE_ON "on"
#define STATE_OFF "off"
#define STATE_LOW "low"
#define STATE_HIGH "high"
#endif