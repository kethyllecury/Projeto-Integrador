#ifndef SINRICMANAGER_H
#define SINRICMANAGER_H

#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <Arduino.h>

class SinricManager {
public:
    static void init();
    static void sendEvent(String deviceId);
private:
    static bool onPowerState(const String &deviceId, bool &state);
};

#endif
