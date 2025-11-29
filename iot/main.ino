#include "Config.h"
#include "WiFiManager.h"
#include "RFIDManager.h"
#include "BuzzerManager.h"
#include "BackendManager.h"
#include "SinricManager.h"

RFIDManager rfid;
BuzzerManager buzzer;

void setup() {
    Serial.begin(115200);
    delay(100);

    WiFiManager::connect();
    SinricManager::init();
    rfid.init();
    buzzer.init();

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    SinricPro.handle();
    buzzer.update();

    String uid = rfid.readUID();
    if (uid != "" && rfid.isNewUID(uid)) {
        Serial.print("[RFID] TAG: ");
        Serial.println(uid);

        SinricManager::sendEvent(DEVICE_ID);
        BackendManager::sendUID(uid);

        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON (10s)");
        delay(10000);
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
    }

    delay(10);
}
