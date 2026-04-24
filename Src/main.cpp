#include "system.h"
#include "micros.h"
#include "debug.h"
#include "matrix.h"
#include "adc.h"
#include "dac.h"
#include "display.h"
#include "eeprom.h"
#include "switches.h"
#include "timer.h"
#include "lookupTables.h"
#include "settings.h"
#include "engine.h"
#include "ui.h"

Adc adc;
Dac dac;
Usb usb;
Uart uart;
Timer timer;
Display display;
Switches switches;
Sys sys;
Eeprom eeprom;
Matrix matrix;

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
} //extern "C"

void update() {
  engine.update();
}

void render() {
  ui.poll();
  engine.render();
}

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
	display.init();
	matrix.init();
	eeprom.init();

	settings.init(&eeprom);
	engine.init(&settings, &uart, &usb, &dac);
	ui.init(&settings, &engine, &matrix, &display, &switches, &adc);

	// Start timers
	timer.init();
	timer.start3(UPDATE_FREQ, &update);
	timer.start2(SAMPLE_RATE, &render);

	while (1) {
		ui.process();
	}
}
