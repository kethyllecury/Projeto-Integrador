#include "SinricManager.h"
#include "Config.h"

bool SinricManager::onPowerState(const String &deviceId, bool &state) {
    Serial.printf("[SinricPro] Dispositivo %s -> %s\n", deviceId.c_str(), state ? "ON" : "OFF");
    return true;
}

void SinricManager::init() {
    SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
    mySwitch.onPowerState(onPowerState);
    SinricPro.begin(APP_KEY, APP_SECRET);
}

void SinricManager::sendEvent(String deviceId) {
    SinricProSwitch &sw = SinricPro[deviceId];
    if (sw.sendPowerStateEvent(true)) {
        Serial.println("[SINRIC] Evento enviado!");
    }
}
