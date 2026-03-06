#ifndef Pages_h
#define Pages_h

#include "settings.h"
#include "engine.h"
#include "canvas.h"
#include "leds.h"
#include "stack.h"

class Ui;

class Pages {

public:

	enum PageId {
		EMPTY_PAGE,
		CONFIRMATION_PAGE,
		OPTION_LIST_PAGE,
		HARDWARE_TEST_PAGE,
		TEXT_INPUT_PAGE,
		CALIBRATION_PAGE,
		LFO_PAGE,
		MIDI_PAGE,
		MIDI_MONITOR_PAGE,
		ENVELOPE_PAGE,
		AMP_PAGE,
		FILTER_PAGE,
		OSCILLATOR_PAGE,
		MOD_MATRIX_PAGE,
		MOD_MATRIX_CC_PAGE,
		MOD_MATRIX_DEPTH_PAGE,
		PATCH_PAGE,

		NUM_PAGES
	};

	void init(Settings *settings, Engine *engine, Ui *ui);
	void open(int id);
	void close(int id);
	void closeAll();
	void onButton(int id, int state);
	void onEncoder(int id, int state);
	void draw();
	void refreshLeds();
	bool isOpen(int id);
	const size_t targetFps();

	struct Page {
		void(*init)();
		void(*enter)();
		void(*exit)();
		void(*draw)();
		void(*refreshLeds)();
		void(*onButton)(int id, int state);
		void(*onEncoder)(int id, int state);
		const size_t (*targetFps)();
	};

	int numOpen() {
		return pageStack_.size();
	}

private:
	int currentPage_;
	Stack<int, 8>pageStack_;
};

#endif
