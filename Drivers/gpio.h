#if !defined(Gpio_h)
#define Gpio_h

#include "stm32f4xx.h"

class Gpio {
 public:
  void init();

  enum FilterType { HP, BP, LP };
  enum FilterRouting { SERIES, PARALEL };
  enum OscType { SAW = 0, TRIANGLE = 1, SINE = 2, NOISE = 2, SQUARE = 3 };

  void setSelectedFilter(FilterType type, FilterRouting routing) {
    uint32_t data = 0;
    uint32_t value = (type * 2) + routing;
    data |= (value & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (value & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (value & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }

  void setFmEnable(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

  void setAmEnable(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

  void setOsc1(OscType type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }

  void setMuteOsc1(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

  void setOsc2(OscType type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }

  void setMuteOsc2(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

 private:
  //
};

#endif  // Gpio_h
