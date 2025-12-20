#include "system.h"
#include "micros.h"
#include "debug.h"
#include "matrix.h"
#include "adc.h"
#include "dac.h"
#include "display.h"
#include "disk.h"
#include "sdio.h"
#include "gpio.h"

#include "disk.h"
#include "settings.h"
#include "engine.h"
#include "ui.h"

Adc adc;
Dac dac;
Usb usb;
//Gate gate;
Uart uart;
Sys sys;
//Timer timer;
//Sdram sdram;
Sdio sdio;
Matrix matrix;
Display display;
Gpio gpio;

Disk disk;
Ui ui;
Engine engine;
Settings settings;

extern "C" {
	void NMI_Handler() { }
	void HardFault_Handler() { while (1); }
	void MemManage_Handler() { while (1); }
	void BusFault_Handler() { while (1); }
	void UsageFault_Handler() { while (1); }
	void SVC_Handler() { }
	void DebugMon_Handler() { }
	void PendSV_Handler() { }
	void SysTick_Handler(void) { }

	// 8Khz
	void TIM3_IRQHandler(void) {
		if (!(TIM3->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM3->SR = ~TIM_IT_UPDATE;
		engine.tick();
	}

	// 1Khz
	void TIM2_IRQHandler(void) {
		if (!(TIM2->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM2->SR = ~TIM_IT_UPDATE;
		ui.poll();
		engine.update();
	}
} //extern "C"

int main(void)
{
	// Init drivers
	// HAL_Init();
	sys.init();

	Debug::init();
	Micros::init();

	dac.init();
	uart.init();
	//gate.init();
	// usb.init();
	adc.init();
	matrix.init();
	display.init();
	sdio.init();

	disk.init(&sdio);
	settings.init(&disk);
	engine.init(&settings, &uart, &usb, &dac, &gpio);
	ui.init(&settings, &engine, &matrix, &display);

	// Start timers
	//dac.start(fill);
	//timer.start_3(CLOCK_ISR_FREQ);
	//timer.start_2(1000);

	while (1) {
		ui.process();
	}
}
