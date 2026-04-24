#if !defined(Eeprom_h)
#define Eeprom_h

#include "stm32f4xx.h"
#include "micros.h"

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
      writeEnable();

      select();
      spiTransfer(WRITE);
      spiTransfer(address >> 8);
      spiTransfer(address);

      while (size) {
        --size;
        spiTransfer(*data++);
        if ((++address % 128) == 0) {
          break;
        }
      }
      deselect();
      Micros::delay(5000);
      while (writeInProgress()) {}
    }
  }

  void read(uint16_t address, uint8_t* data, size_t size) {
    select();
    spiTransfer(READ);
    spiTransfer(address >> 8);
    spiTransfer(address);

    while (size--) {
      *data++ = spiTransfer();
    }
    deselect();
  }

 private:
  void select() {
    GPIOC->BSRR = GPIO_PIN_11 << 16;
  }

  void deselect() {
    GPIOC->BSRR = GPIO_PIN_11;
  }

  uint8_t spiTransfer(uint8_t data = 0) {
    while (!(SPI3->SR & SPI_FLAG_TXE));
    *(volatile uint8_t*)&SPI3->DR = data;

    while (!(SPI3->SR & SPI_FLAG_RXNE));
    return *(volatile uint8_t*)&SPI3->DR;
  }

  void writeEnable() {
    select();
    spiTransfer(WRITE_ENABLE);
    deselect();
    while (writeInProgress()) {}
  }

  bool writeInProgress() {
    bool state;

    select();
    state = spiTransfer(READ_STATUS) & 0x01;
    deselect();
    return state;
  }
};

#endif  // Eeprom_h
