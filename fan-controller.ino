#include "constants.h"
#include "secrets.h"
#include "data.h"

#include "mqtt/MQTTHelper.h"
#include "updater/Updater.h"

MQTTHelper *mqtt;
Updater *updater;

const uint8_t WRITE_TIME = 0;
const uint16_t PULSE_WIDTH = 400;

void setup()
{
  pinMode(TX_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Start");

  mqtt = MQTTHelper::getInstance();
  mqtt->startConnection(WIFI_SSID, WIFI_PASS, MQTT_HOST, MQTT_USER, MQTT_PASS, MQTT_CLIENT);

  mqtt->subscribe(BEDROOM_FAN, MQTTQOS::AT_MOST_ONCE);
  mqtt->subscribe(BEDROOM_LIGHT, MQTTQOS::AT_MOST_ONCE);
  mqtt->subscribe(OFFICE_FAN, MQTTQOS::AT_MOST_ONCE);
  mqtt->subscribe(OFFICE_LIGHT, MQTTQOS::AT_MOST_ONCE);

  updater = Updater::getInstance();

  transmit(400, lowSpeed, sizeof(lowSpeed));
  transmit(400, lowSpeed, sizeof(lowSpeed));
}

void loop()
{
  updater->loop();

  MQTTNotification *notification = mqtt->loop();
  if (notification != nullptr)
  {
    boolean shouldEnable = strcmp(notification->payload, "true");

    switch (notification->payload)
    {
      // Bedroom fan doesn't have a off command
    case BEDROOM_FAN:
      transmit(DATA_PULSE_WIDTH, bedFan, sizeof(bedFan));
      transmit(DATA_PULSE_WIDTH, bedFan, sizeof(bedFan));
      break;
    case BEDROOM_LIGHT:
      transmit(DATA_PULSE_WIDTH, bedLight, sizeof(bedLight));
      transmit(DATA_PULSE_WIDTH, bedLight, sizeof(bedLight));
      break;

    case OFFICE_FAN:
      if (shouldEnable)
      {
        transmit(DATA_PULSE_WIDTH, officeLowSpeed, sizeof(officeLowSpeed));
        transmit(DATA_PULSE_WIDTH, officeLowSpeed, sizeof(officeLowSpeed));
      }
      else
      {
        transmit(DATA_PULSE_WIDTH, officeOffSpeed, sizeof(officeOffSpeed));
        transmit(DATA_PULSE_WIDTH, officeOffSpeed, sizeof(officeOffSpeed));
      }
      break;
    case OFFICE_LIGHT:
      transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
      transmit(DATA_PULSE_WIDTH, officeLight, sizeof(officeLight));
      break;

    default:
      break;
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