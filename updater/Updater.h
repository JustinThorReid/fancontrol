#ifndef _c_updater
#define _c_updater

#include "../../secrets.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoHttpClient.h>
#include <InternalStorageESP.h>

class Updater
{
public:
    void loop();

    static Updater *getInstance()
    {
        static Updater instance;
        return &instance;
    };

private:
    Updater();
};

#endif
