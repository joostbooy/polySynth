#if !defined(Switches_h)
#define Switches_h

#include "filter.h"
#include "oscillator.h"
#include "stm32f4xx.h"

class Switches {
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
  
  void setOsc2(Oscillator::Type2 type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 2) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    data |= (type & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOA->BSRR = data;
  }

  void setMuteOsc1(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }

  void setMuteOsc2(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_6 : GPIO_PIN_6 << 16;
  }
  
  bool readEncoders(int index) {
    switch (index) {
      case 0:
        return GPIOE->IDR & GPIO_PIN_0;
      case 1:
        return GPIOE->IDR & GPIO_PIN_1;
      case 2:
        return GPIOD->IDR & GPIO_PIN_5;
      case 3:
        return GPIOD->IDR & GPIO_PIN_6;
      default:
        break;
    }
    return 0;
  }

  bool readGate(int index) {
    switch (index) {
      case 0:
        return GPIOD->IDR & GPIO_PIN_8;
      case 1:
        return GPIOE->IDR & GPIO_PIN_10;
      default:
        break;
    }
    return 0;
  }

 private:
  //
};

#endif  // Gpio_h
