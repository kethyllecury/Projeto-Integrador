#include "BuzzerManager.h"
#include "Config.h"

BuzzerManager::BuzzerManager() : buzzerLast(0), buzzerState(false) {}

void BuzzerManager::init() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void BuzzerManager::update() {
    unsigned long now = millis();
    if (!buzzerState && now - buzzerLast >= buzzerOffTime) {
        digitalWrite(BUZZER_PIN, HIGH);
        buzzerState = true;
        buzzerLast = now;
    } else if (buzzerState && now - buzzerLast >= buzzerOnTime) {
        digitalWrite(BUZZER_PIN, LOW);
        buzzerState = false;
        buzzerLast = now;
    }
}
