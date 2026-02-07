#include "debug.h"

uint8_t Debug::toggle_state;

void Debug::init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/**UART8 GPIO Configuration
	PA15     ------> TX
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
