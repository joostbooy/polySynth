#include "eeprom.h"

void Eeprom::init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /**GPIO Configuration
  PC11     ------> CS
  */
  GPIO_InitStruct.Pin =  GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}