#ifndef debug_h
#define debug_h

#include "stm32f4xx.h"

class Debug {

public:
    static void init();

	static inline bool read() {
		return GPIOB->IDR & GPIO_PIN_7;
	}

	static inline void write(bool state) {
        GPIOC->BSRR = state ? GPIO_PIN_2 : GPIO_PIN_2 << 16;
    }

    static inline void toggle() {
		(toggle_state ^= 1) ? write(1) : write(0);
    }

private:
	static uint8_t toggle_state;
};

#endif
