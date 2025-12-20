#ifndef Display_h
#define Display_h

#include "debug.h"
#include "stm32f4xx.h"

class Display {
 public:
  void init();
  
  static Display* display_;

  inline void select() {
    GPIOB->BSRR = GPIO_PIN_5 << 16;
  }

  inline void deselect() {
    GPIOB->BSRR = GPIO_PIN_5;
  }

  inline void cd_high() {
    GPIOD->BSRR = GPIO_PIN_7;
  }

  inline void cd_low() {
    GPIOD->BSRR = GPIO_PIN_7 << 16;
  }

  void sendCommand(uint8_t cmd) {
    cd_low();
    spiWrite(cmd);
  }

  void sendData(uint8_t data) {
    cd_high();
    spiWrite(data);
  }

  void sendCommand(uint8_t cmd, uint8_t data) {
    sendCommand(cmd);
    sendData(data);
  }

  void sendCommand(uint8_t cmd, uint8_t data_a, uint8_t data_b) {
    sendCommand(cmd);
    sendData(data_a);
    sendData(data_b);
  }

  void setColAdress(uint8_t a, uint8_t b) {
    sendCommand(0x15, a, b);
  }

  void setRowAdress(uint8_t a, uint8_t b) {
    sendCommand(0x75, a, b);
  }

  void enableWrite() {
    sendCommand(0x5C);
  }

  void unlockDma() {
    SPI2->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    dma_busy_ = false;
  }

  volatile bool dmaBusy() {
    return dma_busy_;
  }

  void sendBuffer(uint8_t* data, uint32_t size) {
    while (dma_busy_) {
    };
    dummy = SPI2->DR;

    setColAdress(0x1C, 0x5B);
    setRowAdress(0x00, 0x3F);
    enableWrite();

    cd_high();

    DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;
    DMA1_Stream4->PAR = reinterpret_cast<uint32_t>(&SPI2->DR);
    DMA1_Stream4->M0AR = reinterpret_cast<uint32_t>(&data[0]);
    DMA1_Stream4->NDTR = size;
    DMA1_Stream4->CR |= DMA_SxCR_EN;
    SPI2->CR2 |= SPI_CR2_TXDMAEN;

    dma_busy_ = true;
  }

 private:
  volatile uint8_t dummy;
  volatile bool dma_busy_;

  void spiWrite(uint8_t data) {
    while (!(SPI2->SR & SPI_FLAG_TXE));
    SPI2->DR = data;

    while (!(SPI2->SR & SPI_FLAG_RXNE));
    dummy = SPI2->DR;
  }
};

#endif
