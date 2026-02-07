#include "switches.h"

void Switches::init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /**GPIO Configuration
    PE0     ------> Enc_Y_A
    PE1     ------> Enc_Y_B
    PE10    ------> Gate in 2
    */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**GPIO Configuration
    PD5     ------> Enc_X_A
    PD6     ------> Enc_X_B
    PD8     ------> Gate 1
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**GPIO Configuration
    PD11     ------> Mute A
    PD10     ------> Mute B
    PD14     ------> VCF select A
    PD15     ------> VCF select B
    PD13     ------> VCF select C
    PD12     ------> AM sync select A
    PD9     ------> AM sync select B
  */
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_12 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**GPIO Configuration
    PA10     ------> VCF mod select A
    PA8     ------> VCF mod select A
  */
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**GPIO Configuration
    PB14    ------> Mod source sync select A
    PB13    ------> Mod source sync select B
    PB15    ------> VCO select A2
    PB12    ------> VCO select B2
 */
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_15 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**GPIO Configuration
  PE15    ------> VCO select A1
  PE14    ------> VCO select B1
*/
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
};