#ifndef BUZZERMANAGER_H
#define BUZZERMANAGER_H

#include <Arduino.h>

class BuzzerManager {
private:
    unsigned long buzzerLast;
    const unsigned long buzzerOnTime = 1000;
    const unsigned long buzzerOffTime = 1000;
    bool buzzerState;

public:
    BuzzerManager();
    void init();
    void update();
};

#endif
