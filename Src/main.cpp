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

/*
bool testEeprom() {
  const size_t kBufferSize = 64;
  uint8_t wData[kBufferSize];
  uint8_t rData[kBufferSize];

  for (size_t i = 0; i < kBufferSize; i++) {
    wData[i] = Rng::u16() >> 8;
  }

  eeprom.write(0, wData, kBufferSize);
  eeprom.read(0, rData, kBufferSize);

  for (size_t i = 0; i < kBufferSize; i++) {
    if (wData[i] != rData[i]) {
      return false;
    }
  }

  return true;
}
*/

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
