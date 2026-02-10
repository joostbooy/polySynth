#include "system.h"
#include "micros.h"
#include "debug.h"
#include "matrix.h"
#include "adc.h"
#include "dac.h"
#include "display.h"
#include "disk.h"
#include "sdio.h"
#include "switches.h"
#include "timer.h"

#include "lookupTables.h"

#include "disk.h"
#include "settings.h"
#include "engine.h"
#include "ui.h"

Adc adc;
Dac dac;
Usb usb;
Uart uart;
Sys sys;
Timer timer;
Sdio sdio;
Matrix matrix;
Display display;
Switches switches;

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

	//	Debug::write(1);
		engine.tick();
	//	Debug::write(0);
	}

	// 1Khz
	void TIM2_IRQHandler(void) {
		if (!(TIM2->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM2->SR = ~TIM_IT_UPDATE;

		// Debug::write(1);
		 ui.poll();
		 engine.update();
		// Debug::write(0);
	}
} //extern "C"

int main(void)
{
	// Init drivers
	sys.init();

	Debug::init();
	Micros::init();

	// usb.init();
	 dac.init();
	 uart.init();
	 switches.init();
	 adc.init();
	 matrix.init();
	 display.init();
	 sdio.init();

	 disk.init(&sdio);
	 settings.init(&disk);
	 //settings.load();
	 engine.init(&settings, &uart, &usb, &dac, &switches);
	 ui.init(&settings, &engine, &matrix, &display, &switches, &adc);

	// Start timers
	timer.start3(UPDATE_FREQ);
	timer.start2(SAMPLE_RATE);

	while (1) {
		ui.process();
	}
}
