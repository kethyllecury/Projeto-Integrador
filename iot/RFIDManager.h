#ifndef RFIDMANAGER_H
#define RFIDMANAGER_H

#include <MFRC522.h>
#include <SPI.h>
#include <Arduino.h>

class RFIDManager {
private:
    MFRC522 mfrc522;
    String lastUid;
    unsigned long lastTrigger;
    const unsigned long triggerDebounceMs = 3000;

    bool isHex(String s);
    bool validateUid(String uid);

public:
    RFIDManager();
    void init();
    String readUID();
    bool isNewUID(String uid);
};

#endif
