#ifndef _MIC_H_
#define _MIC_H_
#include <Arduino.h>

#define SAMPLE_RATE 16000U
#define SAMPLE_BITS 16
#define WAV_HEADER_SIZE 44
#define VOLUME_GAIN 2

// #define I2S_IN_BCLK 40
// #define I2S_IN_LRC 42
// #define I2S_IN_DIN 2

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
#define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 4 // SCK
#define I2S_IN_LRC 5 // WS
#define I2S_IN_DIN 6 // SD
#define RECORD_TIME   10  // seconds, The maximum value is 240

#define bufferLen 1024  // 缓冲区大小，可根据需要调整

class Mic {
    public:
        
        void setup(int pinWS, int pinSD, int pinSCK);
        void recordWav(const char* url, int recordTime, int sampleRate, int sampleBits);

        void loop();
    private:
        void generateWavHeader(uint8_t *wav_header, uint32_t wav_size, uint32_t sample_rate);
        // int mSampleRate;
        // int mSampleBits;
};

#endif

