#ifndef uart_h
#define uart_h

#include "stm32f4xx.h"

class Uart {
 public:
  void init();

  // MIDI 1 - USART 6
  void write(uint8_t data) {
    USART6->DR = data;
  }

  bool writeable() {
    return USART6->SR & UART_FLAG_TXE;
  }

  uint8_t read() {
    return USART6->DR;
  }

  bool readable() {
    return USART6->SR & UART_FLAG_RXNE;
  }

 private:
};

#endif
