#include "AudioRecorder.h"

// #include <Arduino.h>
// #include <I2S.h>
#include <WiFi.h>
#include <sstream>
#include "net.h"
#include <SPIFFS.h>
#include <driver/i2s.h>




extern Net net;

int16_t sBuffer[bufferLen];  // 音频数据缓冲区

void AudioRecorder::setup(int pinWS, int pinSD, int pinSCK) {
    Serial.println("AudioRecorder Setup I2S...");

    // Serial.begin(115200);
    // while (!Serial)
    // ;
    // I2S.setAllPins(pinSCK, pinWS, pinSD, -1, pinSD);

    // Initialize I2S for audio input
  i2s_config_t i2s_config_in = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // 注意：INMP441 输出 32 位数据
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
  };
  i2s_pin_config_t pin_config_in = {
    .bck_io_num = I2S_IN_BCLK,
    .ws_io_num = I2S_IN_LRC,
    .data_out_num = -1,
    .data_in_num = I2S_IN_DIN
  };
  i2s_driver_install(I2S_PORT, &i2s_config_in, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config_in);

}

void AudioRecorder::recordWav(const char *url, int recordTime, int sampleRate, int sampleBits)
{
    // if (!I2S.begin(PDM_MONO_MODE, sampleRate, sampleBits))
    // {
    //     Serial.println("Failed to initialize I2S!");
    //     return;
    // }
    uint32_t sample_size = 0;
    uint32_t record_size = (sampleRate * sampleBits / 8) * recordTime;
    uint8_t *rec_buffer = NULL;
    Serial.printf("Ready to start recording ...\n");

    if (!SPIFFS.begin()) {
        return;
    }
    
    File file = SPIFFS.open("/voice.wav", FILE_WRITE);
    // Write the header to the WAV file
    uint8_t wav_header[WAV_HEADER_SIZE];
    generateWavHeader(wav_header, record_size, sampleRate);
    file.write(wav_header, WAV_HEADER_SIZE);

    // PSRAM malloc for recording
    rec_buffer = (uint8_t *)ps_malloc(record_size);
    if (rec_buffer == NULL)
    {
    Serial.printf("malloc failed!\n");
    while (1)
        ;
    }
    Serial.printf("Buffer: %d bytes\n", ESP.getPsramSize() - ESP.getFreePsram());

    // Start recording
    i2s_read(I2S_NUM_0, rec_buffer, record_size, &sample_size, portMAX_DELAY);
    if (sample_size == 0)
    {
    Serial.printf("Record Failed!\n");
    }
    else
    {
    Serial.printf("Record %d bytes\n", sample_size);
    }

    // Increase volume
    for (uint32_t i = 0; i < sample_size; i += sampleBits / 8)
    {
    (*(uint16_t *)(rec_buffer + i)) <<= VOLUME_GAIN;
    }

    // Write data to the WAV file
    Serial.printf("Writing to the file ...\n");
    if (file.write(rec_buffer, record_size) != record_size)
    Serial.printf("Write file Failed!\n");

    free(rec_buffer);
    file.close();
    Serial.printf("The recording is over.\n");
}

void AudioRecorder::generateWavHeader(uint8_t *wav_header, uint32_t wav_size, uint32_t sample_rate)
{
  // See this for reference: http://soundfile.sapp.org/doc/WaveFormat/
  uint32_t file_size = wav_size + WAV_HEADER_SIZE - 8;
  uint32_t byte_rate = SAMPLE_RATE * SAMPLE_BITS / 8;
  const uint8_t set_wav_header[] = {
      'R', 'I', 'F', 'F',                                                  // ChunkID
      file_size, file_size >> 8, file_size >> 16, file_size >> 24,         // ChunkSize
      'W', 'A', 'V', 'E',                                                  // Format
      'f', 'm', 't', ' ',                                                  // Subchunk1ID
      0x10, 0x00, 0x00, 0x00,                                              // Subchunk1Size (16 for PCM)
      0x01, 0x00,                                                          // AudioFormat (1 for PCM)
      0x01, 0x00,                                                          // NumChannels (1 channel)
      sample_rate, sample_rate >> 8, sample_rate >> 16, sample_rate >> 24, // SampleRate
      byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24,         // ByteRate
      0x02, 0x00,                                                          // BlockAlign
      0x10, 0x00,                                                          // BitsPerSample (16 bits)
      'd', 'a', 't', 'a',                                                  // Subchunk2ID
      wav_size, wav_size >> 8, wav_size >> 16, wav_size >> 24,             // Subchunk2Size
  };
  memcpy(wav_header, set_wav_header, sizeof(set_wav_header));
}

void AudioRecorder::loop() {
//   size_t bytesRead = 0;
//   esp_err_t result = i2s_read(I2S_PORT, sBuffer, bufferLen * sizeof(int16_t), &bytesRead, portMAX_DELAY);
//   if (result == ESP_OK && bytesRead > 0) {
//     // 在这里可以对读取到的音频数据进行处理，比如保存到文件、发送到其他设备等
//     // 目前只是简单地打印一些信息
//     Serial.print("Bytes Read: ");
//     Serial.println(bytesRead);
//   }
}