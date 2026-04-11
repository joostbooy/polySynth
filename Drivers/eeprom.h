#if !defined(Eeprom_h)
#define Eeprom_h

#include "stm32f4xx.h"

class Eeprom {
 public:
  enum Command {
    READ = 0x03,
    WRITE = 0x02,
    WRITE_ENABLE = 0x06,
    WRITE_DISABLE = 0x04,
    READ_STATUS = 0x05,
    WRITE_STATUS = 0x01,
    ERASE = 0xC7,
    POWER_DOWN = 0xB9,
    WAKEUP = 0xAb,
  };

  void init();

  void write(uint16_t address, uint8_t* data, size_t size) {
    while (size) {
      select();
      spiTransfer(WRITE);
      spiTransfer(address);

      while (size--) {
        spiTransfer(*data++);
        if ((++address % 128) == 0) {
          break;
        }
      }
      deselect();
    }
  }

  void read(uint16_t address, uint8_t* data, size_t size) {
    select();
    spiTransfer(READ);
    spiTransfer(address);

    while (size--) {
      *data++ = spiTransfer();
    }
    deselect();
  }

 private:
  void select() {
    GPIOC->BSRR = GPIO_PIN_15 << 16;
  }

  void deselect() {
    GPIOC->BSRR = GPIO_PIN_15;
  }

  uint8_t spiTransfer(uint8_t data = 0) {
    while (!(SPI3->SR & SPI_FLAG_TXE));
    *(volatile uint8_t*)&SPI3->DR = data;

    while (!(SPI3->SR & SPI_FLAG_RXNE));
    return *(volatile uint8_t*)&SPI3->DR;
  }
};

#endif  // Eeprom_h
