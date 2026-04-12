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
Sys sys;
Timer timer;
Eeprom eeprom;
Matrix matrix;
Display display;
Switches switches;

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

bool testEeprom() {
  static const size_t kDataSize = 256;
  uint8_t wData[kDataSize];
  uint8_t rData[kDataSize];

  for (size_t i = 0; i < kDataSize; i++) {
    wData[i] = Rng::u16() >> 8;
  }

  eeprom.write(0, wData, kDataSize);
  eeprom.read(0, rData, kDataSize);

  for (size_t i = 0; i < kDataSize; i++) {
    if (rData[i] != wData[i]) {
      return false;
    }
  }

  return true;
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
	matrix.init();
	display.init();
	eeprom.init();

	settings.init(&eeprom);
	settings.load();
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
