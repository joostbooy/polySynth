#ifndef debug_h
#define debug_h

#include "stm32f4xx.h"

class Debug {

public:
    static void init();

	static inline void write(bool state) {
        GPIOA->BSRR = state ? GPIO_PIN_15 : GPIO_PIN_15 << 16;
    }

    static inline void toggle() {
		(toggle_state ^= 1) ? write(1) : write(0);
    }

private:
	static uint8_t toggle_state;
};

#endif
