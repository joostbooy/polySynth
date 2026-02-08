#ifndef adc_h
#define adc_h

#include "micros.h"
#include "stm32f4xx.h"

class Adc {
 public:
  void init();

  // Pots
  inline bool potReady() {
    return ADC2->SR & ADC_SR_EOC;
  }

  inline uint16_t readPot() {
    return ADC2->DR;
  }

  inline uint8_t currentPot() {
    return pot_;
  }

  inline void convertNextPot() {
    if (++pot_ >= kNumPots) {
      pot_ = 0;
    }

    selectBank(pot_ / kPotsPerBank);
    selectChannel(pot_ % kPotsPerBank);

    ADC2->CR2 |= ADC_CR2_SWSTART;
  }

  // CV
  inline bool cvReady() {
    return ADC1->SR & ADC_SR_EOC;
  }

  inline uint16_t readCv() {
    return ADC1->DR;
  }

  inline uint8_t currentCv() {
    return cv_;
  }

  inline void convertNextCv() {
    if (++cv_ >= kNumCvs) {
      cv_ = 0;
    }
    ADC1->SQR3 = (cv_ == 0) ? ADC_CHANNEL_8 : ADC_CHANNEL_9;
    ADC1->CR2 |= ADC_CR2_SWSTART;
  }

 private:
  size_t cv_ = 0;
  size_t pot_ = 0;
  uint32_t currBank_ = 0;
  static const size_t kNumCvs = 2;
  static const size_t kNumPots = 32;
  static const size_t kPotsPerBank = 8;

  void selectBank(int bank) {
    GPIOA->BSRR = currBank_;

    switch (bank) {
      case 0:
        currBank_ = GPIO_PIN_7;
        break;
      case 1:
        currBank_ = GPIO_PIN_6;
        break;
      case 2:
        currBank_ = GPIO_PIN_5;
        break;
      case 3:
        currBank_ = GPIO_PIN_4;
        break;
      default:
        break;
    }
    GPIOA->BSRR = currBank_ << 16;
  }

  void selectChannel(int channel) {
    uint32_t data = 0;
    data |= (channel & 1) ? GPIO_PIN_0 : GPIO_PIN_0 << 16;
    data |= (channel & 2) ? GPIO_PIN_1 : GPIO_PIN_1 << 16;
    data |= (channel & 4) ? GPIO_PIN_3 : GPIO_PIN_3 << 16;
    GPIOA->BSRR = data;
  }
};

#endif
