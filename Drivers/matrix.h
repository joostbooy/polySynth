#ifndef Matrix_h
#define Matrix_h

#include "stm32f4xx.h"
#include "micros.h"

// Button led matrix 8x16

class Matrix {

public:

	void init();

	void set_leds(uint8_t *led_data) {
		for (int i = 0; i < kNumOfLedCollumns; ++i) {
			led_row_[i] = *led_data++;
		}
	}

	void refresh(uint8_t *sw_buffer) {
		// read switches
		for (int i = 0; i < kNumOfSwitchCollumns; ++i) {
			set_collumn(i);

			// latch switch rows
			GPIOB->BSRR = GPIO_PIN_3 << 16;
			Micros::delay(1);
			GPIOB->BSRR = GPIO_PIN_3;

			*sw_buffer++ = spi_transfer();
		}

		// write leds
		set_collumn(led_coll_);
		spi_transfer(led_row_[led_coll_]);

		++led_coll_;
		if (led_coll_ >= kNumOfLedCollumns) {
			led_coll_ = 0;
		}

		// latch led rows & collumn
		GPIOB->BSRR = GPIO_PIN_4 << 16;
		GPIOB->BSRR = GPIO_PIN_5 << 16;
		Micros::delay(1);
		GPIOB->BSRR = GPIO_PIN_4 | GPIO_PIN_5;
	}

private:
	static const uint8_t kNumOfSwitchCollumns = 8;
	static const uint8_t kNumOfLedCollumns = 7;

	uint8_t led_coll_ = 0;
	uint8_t led_row_[kNumOfLedCollumns];

	void set_collumn(int coll) {
		uint32_t reg = 0;
		coll & 0x01 ? reg |= GPIO_PIN_6 : reg |= GPIO_PIN_6 << 16;
		coll & 0x02 ? reg |= GPIO_PIN_7 : reg |= GPIO_PIN_7 << 16;
		coll & 0x04 ? reg |= GPIO_PIN_8 : reg |= GPIO_PIN_8 << 16;
		GPIOB->BSRR = reg;
		Micros::delay(1);
	}

	uint8_t spi_transfer(uint8_t send = 0xff) {
		volatile uint8_t recv;

		while (!(SPI4->SR & SPI_FLAG_TXE));
		*(volatile uint8_t*)&SPI4->DR = send;

		while (!(SPI4->SR & SPI_FLAG_RXNE));
		recv = *(volatile uint8_t*)&SPI4->DR;

		return recv;
	}
};

#endif
