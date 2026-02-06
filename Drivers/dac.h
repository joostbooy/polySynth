#ifndef Dac_h
#define Dac_h

#include "micros.h"
#include "stm32f4xx.h"

// https://github.com/westlicht/performer/blob/master/src/platform/stm32/drivers/Dac.h
// https://github.com/westlicht/performer/blob/master/src/platform/stm32/drivers/Dac.cpp
// https://github.com/westlicht/performer/blob/master/src/platform/stm32/hal/Delay.h

#define WRITE_INPUT_REGISTER 0
#define UPDATE_OUTPUT_REGISTER 1
#define WRITE_INPUT_REGISTER_UPDATE_ALL 2
#define WRITE_INPUT_REGISTER_UPDATE_N 3
#define POWER_DOWN_UP_DAC 4
#define LOAD_CLEAR_CODE_REGISTER 5
#define LOAD_LDAC_REGISTER 6
#define RESET_POWER_ON 7
#define SETUP_INTERNAL_REF 8

class Dac {
 public:
  enum ClearCode {
    ClearZeroScale = 0,
    ClearMidScale = 1,
    ClearFullScale = 2,
    ClearIgnore = 3,
  };

  void init();

  void set(int muxChannel, int dacChannel, uint16_t value) {
    muxChannel_[muxChannel].value[dacChannel] = value;
  }

  void send() {
    deInhibit();

    setMuxChannel(currMuxChannel_);
    MuxChannel& c = muxChannel_[currMuxChannel_];
    if (++currMuxChannel_ >= kNumMuxChannels) {
      currMuxChannel_ = 0;
    }

    for (int i = 0; i < kNumDacChannels; ++i) {
      if (i < 8) {
        writeOctalDac(WRITE_INPUT_REGISTER_UPDATE_N, i, c.value[i], 0);
      } else {
        writeQuadDac(i - 8, c.value[i]);
      }
    }

    inhibit();
  }

 private:
  static const int kNumDacChannels = 12;
  static const int kNumMuxChannels = 8;

  struct MuxChannel {
    uint16_t value[kNumDacChannels];
  } muxChannel_[kNumMuxChannels];

  volatile uint8_t dummy;
  uint8_t currMuxChannel_;

  void deInhibit() {
    GPIOA->BSRR = GPIO_PIN_8;
  }

  void inhibit() {
    GPIOA->BSRR = GPIO_PIN_8 << 16;
  }

  void setMuxChannel(int channel) {
    uint32_t data = 0;
    data |= (channel & 1) ? GPIO_PIN_1 : GPIO_PIN_1 << 16;
    data |= (channel & 2) ? GPIO_PIN_4 : GPIO_PIN_4 << 16;
    data |= (channel & 4) ? GPIO_PIN_10 : GPIO_PIN_10 << 16;
    GPIOA->BSRR = data;
  }

  // DAC8568
  void writeOctalDac(uint8_t command, uint8_t address, uint16_t data, uint8_t function) {
    uint8_t b1 = command;
    uint8_t b2 = (address << 4) | (data >> 12);
    uint8_t b3 = data >> 4;
    uint8_t b4 = (data & 0xf) << 4 | function;

    GPIOA->BSRR = GPIO_PIN_4 << 16;
    asm("NOP");

    spiWrite(b1);
    spiWrite(b2);
    spiWrite(b3);
    spiWrite(b4);
    asm("NOP");

    GPIOA->BSRR = GPIO_PIN_4;
    asm("NOP");
  }

  // DAC8564
  void writeQuadDac(uint8_t address, uint16_t data) {
    uint8_t b1 = (address << 1) | (1 << 4);
    uint8_t b2 = data >> 8;
    uint8_t b3 = data;

    GPIOA->BSRR = GPIO_PIN_2 << 16;
    asm("NOP");

    spiWrite(b1);
    spiWrite(b2);
    spiWrite(b3);
    asm("NOP");

    GPIOA->BSRR = GPIO_PIN_2;
    asm("NOP");
  }

  void spiWrite(uint8_t data) {
    while (!(SPI1->SR & SPI_FLAG_TXE));
    SPI1->DR = data;

    while (!(SPI1->SR & SPI_FLAG_RXNE));
    dummy = SPI1->DR;
  }
};

#endif
