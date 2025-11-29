#include "RFIDManager.h"
#include "Config.h"

RFIDManager::RFIDManager() : mfrc522(SS_PIN, RST_PIN), lastUid(""), lastTrigger(0) {}

void RFIDManager::init() {
    SPI.begin(SCK, MISO, MOSI, SS_PIN);
    mfrc522.PCD_Init();
    Serial.println("Aproxime o cartão RFID...");
}

bool RFIDManager::isHex(String s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isxdigit(s.charAt(i))) return false;
    }
    return true;
}

bool RFIDManager::validateUid(String uid) {
    return uid.length() >= 8 && uid.length() <= 16 && isHex(uid);
}

String RFIDManager::readUID() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String uidStr = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
            uidStr += String(mfrc522.uid.uidByte[i], HEX);
        }
        uidStr.toUpperCase();

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        if (validateUid(uidStr)) return uidStr;
    }
    return "";
}

bool RFIDManager::isNewUID(String uid) {
    unsigned long now = millis();
    if (uid != lastUid || (now - lastTrigger) > triggerDebounceMs) {
        lastUid = uid;
        lastTrigger = now;
        return true;
    }
    return false;
}
