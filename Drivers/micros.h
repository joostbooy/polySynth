#ifndef Micros_h
#define Micros_h

#include "stm32f4xx.h"

class Micros {

public:
	static void init();

	static inline volatile uint32_t read() {
		return TIM5->CNT;
	}

	static void delay(uint32_t us) {
		uint32_t start = read();
		while (static_cast<uint32_t>(read() - start) < us);
	}

private:

};

#endif
