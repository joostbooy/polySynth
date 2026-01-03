#ifndef adc_h
#define adc_h

#include "micros.h"
#include "stm32f4xx.h"

class Adc {
 public:
  void init();

  inline bool ready() {
    return ADC1->SR & ADC_SR_EOC;
  }

  inline uint16_t read() {
    return ADC1->DR;
  }

  inline uint8_t currPot() {
    return pot_;
  }

  inline void convertNextPot() {
    if (++pot_ >= kNumPots) {
      pot_ = 0;
    }

    selectBank(pot_ / kPotsPerBank);
    selectChannel(pot_ % kPotsPerBank);

    ADC1->CR2 |= ADC_CR2_SWSTART;
  }

 private:
  uint32_t currBank_ = 0;
  size_t pot_ = 0;
  static const size_t kNumPots = 32;
  static const size_t kPotsPerBank = 8;

  void selectBank(int bank) {
    GPIOA->BSRR = currBank_ << 16;

    switch (bank) {
      case 0:
        currBank_ = GPIO_PIN_13;
        break;
      case 1:
        currBank_ = GPIO_PIN_13;
        break;
      case 2:
        currBank_ = GPIO_PIN_13;
        break;
      case 3:
        currBank_ = GPIO_PIN_13;
        break;
      default:
        break;
    }
    GPIOA->BSRR = currBank_;
  }

  void selectChannel(int channel) {
    uint32_t data = 0;
    data |= (channel & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (channel & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (channel & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }
};

#endif
