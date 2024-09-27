#include "Led.h"
#include <Arduino.h>

Led::Led(int pin) : mPin(pin) {
    pinMode(pin, OUTPUT);
}

void Led::on() {
    digitalWrite(mPin, HIGH);
}

void Led::off() {
    digitalWrite(mPin, LOW);
}

void Led::setFlickerInterval(int interval) {
    mFlickerInterval = interval;
}

void Led::autoFlicker() {
    on();
    delay(mFlickerInterval);
    off();
    delay(mFlickerInterval);
}