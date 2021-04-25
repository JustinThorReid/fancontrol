#ifndef _constants
#define _constants

#define TX_PIN 5
#define DATA_PULSE_WIDTH 400

#define ROOM_BED "bedroom"
#define ROOM_OFFICE "office"
#define FAN "fan"
#define LIGHT "light"

#define BEDROOM_FAN "thor/" + APP_NAME + "/" + ROOM_BED + "/" + FAN + "/enabled"
#define BEDROOM_LIGHT "thor/" + APP_NAME + "/" + ROOM_BED + "/" + LIGHT + "/enabled"
#define OFFICE_FAN "thor/" + APP_NAME + "/" + ROOM_OFFICE + "/" + FAN + "/enabled"
#define OFFICE_LIGHT "thor/" + APP_NAME + "/" + ROOM_OFFICE + "/" + LIGHT + "/enabled"

#endif