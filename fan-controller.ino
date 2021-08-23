#include "constants.h"
#include "secrets.h"

#include "src/arduino-mqtt-helper/MQTTHelper.h"
#include "src/arduino-updater/Updater.h"
#include "src/fan-commands.h"

MQTTHelper *mqtt;
Updater *updater;

void setup()
{
  pinMode(TX_PIN, OUTPUT);
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
    if (strcmp(notification->topic, BEDROOM_FAN) == 0)
    {
      setBedroomFan(strcmp(notification->payload, STATE_ON) == 0);
    }
    else if (strcmp(notification->topic, BEDROOM_LIGHT) == 0)
    {
      setBedroomLight(strcmp(notification->payload, STATE_ON) == 0);
    }
    else if (strcmp(notification->topic, OFFICE_FAN) == 0)
    {
      if (strcmp(notification->payload, STATE_ON) == 0)
      {
        setOfficeFan(true);
      }
      else if (strcmp(notification->payload, STATE_OFF) == 0)
      {
        setOfficeFan(false);
      }
      else if (strcmp(notification->payload, STATE_LOW) == 0)
      {
        setOfficeFanSpeed(FAN_LOW);
      }
      else if (strcmp(notification->payload, STATE_HIGH) == 0)
      {
        setOfficeFanSpeed(FAN_HIGH);
      }
    }
    else if (strcmp(notification->topic, OFFICE_LIGHT) == 0)
    {
      setOfficeLight(strcmp(notification->payload, STATE_ON) == 0);
    }
  }
}
