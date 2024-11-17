#include "Speaker.h"

#define I2S_DOUT      7
#define I2S_BCLK      15
#define I2S_LRC       16

void Speaker::setup() {
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(18);
    xTaskCreatePinnedToCore(&Speaker::playDelegate, "SpeakerTask", 4096 * 2, NULL, 1, NULL, 0);
}

void Speaker::play(const char* source) {
    audio.connecttohost(source);
}

void Speaker::loop() {
    audio.loop();
}