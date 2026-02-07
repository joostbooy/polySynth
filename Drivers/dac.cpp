#include "dac.h"


void Dac::init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /**GPIO Configuration
  PC13     ------> CS octal
  PE4     ------> CS Quad
  PE12     ------> Mux A
  PE11     ------> Mux B
  PE13     ------> Mux C
  PB10     ------> Mux inhibit
  */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**SPI4 GPIO Configuration
  PE2     ------> SPI4_SCK
  PE5     ------> SPI4_MISO
  PE6     ------> SPI4_MOSI
  */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  SPI_HandleTypeDef hspi4;
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi4);
  __HAL_SPI_ENABLE(&hspi4);

  // cs pins HIGH
  GPIOC->BSRR = GPIO_PIN_13;
  GPIOE->BSRR = GPIO_PIN_4;
  Micros::delay(1);

  writeOctalDac(RESET_POWER_ON, 0, 0, 0);
  Micros::delay(50);
  writeOctalDac(LOAD_CLEAR_CODE_REGISTER, 0, 0, ClearIgnore);
  writeOctalDac(SETUP_INTERNAL_REF, 0, 0, true);
  writeOctalDac(POWER_DOWN_UP_DAC, 0, 0, 0xff);

  currMuxChannel_ = 0;

  for (size_t i = 0; i < kNumDacChannels; i++) {
    for (size_t j = 0; j < kNumMuxChannels; j++) {
      muxChannel_[j].value[i] = 0;
    }
  }
}
