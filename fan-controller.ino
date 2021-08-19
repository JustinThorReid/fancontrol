#include "constants.h"
#include "secrets.h"
#include "data.h"

#include "src/arduino-mqtt-helper/MQTTHelper.h"
#include "src/arduino-updater/Updater.h"

MQTTHelper *mqtt;
Updater *updater;

const uint8_t WRITE_TIME = 0;
const uint16_t PULSE_WIDTH = 400;

void setup()
{
  pinMode(TX_PIN, OUTPUT);
  //  transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
  //  Serial.begin(115200);
  //  Serial.println("Start");
  //
  mqtt = MQTTHelper::getInstance();
  mqtt->startConnection(WIFI_SSID, WIFI_PASS, MQTT_HOST, MQTT_USER, MQTT_PASS, MQTT_CLIENT);

  mqtt->subscribe(BEDROOM_FAN, MQTTQOS::EXACTLY_ONCE);
  mqtt->subscribe(BEDROOM_LIGHT, MQTTQOS::EXACTLY_ONCE);
  mqtt->subscribe(OFFICE_FAN, MQTTQOS::EXACTLY_ONCE);
  mqtt->subscribe(OFFICE_LIGHT, MQTTQOS::EXACTLY_ONCE);

  updater = Updater::getInstance();
}

void loop()
{
  updater->loop();

  const MQTTNotification *notification = mqtt->loop();
  if (notification != nullptr)
  {
    boolean shouldEnable = strcmp(notification->payload, "true") == 0;

    if (strcmp(notification->payload, BEDROOM_FAN) == 0)
    {
      // Bedroom fan doesn't have a off command
      transmit(DATA_PULSE_WIDTH, bedFan, sizeof(bedFan));
    }
    else if (strcmp(notification->payload, BEDROOM_LIGHT) == 0)
    {
      transmit(DATA_PULSE_WIDTH, bedLight, sizeof(bedLight));
    }
    else if (strcmp(notification->payload, OFFICE_FAN) == 0)
    {
      if (shouldEnable)
      {
        transmit(DATA_PULSE_WIDTH, officeLowSpeed, sizeof(officeLowSpeed));
      }
      else
      {
        transmit(DATA_PULSE_WIDTH, officeOffSpeed, sizeof(officeOffSpeed));
      }
    }
    else if (strcmp(notification->payload, OFFICE_LIGHT) == 0)
    {
      transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
    }
  }
}

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
}
