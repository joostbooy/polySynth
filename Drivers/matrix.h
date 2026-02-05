#ifndef Matrix_h
#define Matrix_h

#include "micros.h"
#include "stm32f4xx.h"

// Button led matrix 8x16

class Matrix {
 public:
  void init();

  uint8_t currentCollumn() {
	return currentCollumn_;
  }

  void setLeds(uint16_t* data) {
    for (int i = 0; i < kNumCollumns_; ++i) {
      ledRow_[i] = *data++;
    }
  }

  void refresh(uint8_t* swBuffer) {
    ++currentCollumn_ %= kNumCollumns_;
    setCollumn(currentCollumn_);

    // latch switch rows
    GPIOB->BSRR = GPIO_PIN_3 << 16;
    Micros::delay(1);
    GPIOB->BSRR = GPIO_PIN_3;

    *swBuffer = spiTransfer(ledRow_[currentCollumn_]);

    // latch led rows & collumn
    GPIOB->BSRR = GPIO_PIN_4 << 16;
    GPIOB->BSRR = GPIO_PIN_5 << 16;
    Micros::delay(1);
    GPIOB->BSRR = GPIO_PIN_4 | GPIO_PIN_5;
  }

 private:
  static const uint8_t kNumCollumns_ = 8;
  uint8_t currentCollumn_ = 0;
  uint8_t ledRow_[kNumCollumns_];

  void setCollumn(int coll) {
    uint32_t reg = 0;
    coll& 0x01 ? reg |= GPIO_PIN_6 : reg |= GPIO_PIN_6 << 16;
    coll& 0x02 ? reg |= GPIO_PIN_7 : reg |= GPIO_PIN_7 << 16;
    coll& 0x04 ? reg |= GPIO_PIN_8 : reg |= GPIO_PIN_8 << 16;
    GPIOB->BSRR = reg;
    Micros::delay(1);
  }

  uint8_t spiTransfer(uint16_t ledData) {
    volatile uint8_t swData;

    while (!(SPI4->SR & SPI_FLAG_TXE));
    *(volatile uint8_t*)&SPI4->DR = ledData >> 8;

    while (!(SPI4->SR & SPI_FLAG_RXNE));
    swData = *(volatile uint8_t*)&SPI4->DR;

	while (!(SPI4->SR & SPI_FLAG_TXE));
    *(volatile uint8_t*)&SPI4->DR = ledData;

    return swData;
  }
};

#endif
