#include "matrix.h"

void Matrix::init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**GPIO Configuration
	PB6     ------> Col 0
	PB7     ------> Col 1
	PB8     ------> Col 2
	PB3     ------> Latch SW row
	PB4     ------> Latch LED row
	PB5     ------> Latch LED col
	*/
	GPIO_InitStruct.Pin =  GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIOB->BSRR = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;

	/**SPI4 GPIO Configuration
	PE2     ------> SPI4_SCK
	PE5     ------> SPI4_MISO
	PE6     ------> SPI4_MOSI
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// SPI4
	SPI_HandleTypeDef hspi4;

	hspi4.Instance = SPI4;
	hspi4.Init.Mode = SPI_MODE_MASTER;
	hspi4.Init.Direction = SPI_DIRECTION_2LINES;
	hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi4.Init.NSS = SPI_NSS_SOFT;
	hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi4.Init.CRCPolynomial = 10;
	//hspi4.Init.CRCPolynomial = 7;
	//hspi4.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	//hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	HAL_SPI_Init(&hspi4);
	__HAL_SPI_ENABLE(&hspi4);
}
