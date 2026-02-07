#if !defined(Switches_h)
#define Switches_h

#include "filter.h"
#include "oscillator.h"
#include "stm32f4xx.h"

class Switches {
 public:
  void init();

  void setMuteOsc1(bool state) {
    GPIOD->BSRR = state ? GPIO_PIN_11 << 16 : GPIO_PIN_11;
  }

  void setMuteOsc2(bool state) {
    GPIOD->BSRR = state ? GPIO_PIN_10 << 16 : GPIO_PIN_10;
  }

  void setAmEnable(bool state) {
    GPIOD->BSRR = state ? GPIO_PIN_12 : GPIO_PIN_12 << 16;
  }

  void setFmEnable(bool state) {
    GPIOD->BSRR = state ? GPIO_PIN_9 : GPIO_PIN_9 << 16;
  }

  void setFmFilter1Enable(bool state) {
    GPIOA->BSRR = state ? GPIO_PIN_10 : GPIO_PIN_10 << 16;
  }

  void setFmFilter2Enable(bool state) {
    GPIOA->BSRR = state ? GPIO_PIN_8 : GPIO_PIN_8 << 16;
  }

  void setSync(bool state) {
    GPIOB->BSRR = state ? GPIO_PIN_14 : GPIO_PIN_14 << 16;
  }

  void setVcoModSource(int source) {
    GPIOB->BSRR = source == 0 ? GPIO_PIN_13 << 16 : GPIO_PIN_13;
  }

  void setSelectedFilter(Filter::Type type, Filter::Routing routing) {
    uint32_t data = 0;
    uint32_t value = (type * 2) + routing;
    data |= (value & 1) ? GPIO_PIN_14 : GPIO_PIN_14 << 16;
    data |= (value & 2) ? GPIO_PIN_15 : GPIO_PIN_15 << 16;
    data |= (value & 4) ? GPIO_PIN_13 : GPIO_PIN_13 << 16;
    GPIOD->BSRR = data;
  }

  void setOsc1(Oscillator::Type1 type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_15 : GPIO_PIN_15 << 16;
    data |= (type & 2) ? GPIO_PIN_14 : GPIO_PIN_14 << 16;
    GPIOE->BSRR = data;
  }

  void setOsc2(Oscillator::Type2 type) {
    uint32_t data = 0;
    data |= (type & 1) ? GPIO_PIN_15 : GPIO_PIN_15 << 16;
    data |= (type & 2) ? GPIO_PIN_12 : GPIO_PIN_12 << 16;
    GPIOB->BSRR = data;
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
