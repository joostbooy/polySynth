#if !defined(Gpio_h)
#define Gpio_h

#include "stm32f4xx.h"
#include "filter.h"
#include "oscillator.h"

class Gpio {
 public:
  void init();

  void setSelectedFilter(Filter::Type type, Filter::Routing routing) {
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

  void setOsc1(Oscillator::Type1 type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }

  void setMuteOsc1(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

  void setOsc2(Oscillator::Type2 type) {
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
