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
		LFO_PAGE,
		MIDI_PAGE,
		ENVELOPE_PAGE,
		AMP_PAGE,
		FILTER_PAGE,
		OSCILLATOR_PAGE,
		MOD_MATRIX_PAGE,

		NUM_PAGES
	};

	void init(Settings *settings, Engine *engine, Ui *ui);
	void open(int id);
	void close(int id);
	void close_all();
	void on_button(int id, int state);
	void on_encoder(int id, int state);
	void draw();
	void refresh_leds();
	const size_t target_fps();

	struct Page {
		void(*init)();
		void(*enter)();
		void(*exit)();
		void(*draw)();
		void(*refresh_leds)();
		void(*on_button)(int id, int state);
		void(*on_encoder)(int id, int state);
		const size_t (*target_fps)();
	};

	int num_open() {
		return page_stack_.size();
	}

private:
	int curr_page_;
	Stack<int, 8>page_stack_;
	//Page *page_[NUM_PAGES];
};

#endif
