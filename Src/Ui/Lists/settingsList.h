#ifndef SettingsList_h
#define SettingsList_h

#include "settingsText.h"

class SettingsList {

public:

	void init(Engine *engine, Settings *settings) {
		engine_ = engine;
		settings_ = settings;
	}

	constexpr const int collumns() {
		return 4;
	}

	int top_item() {
		return top_item_;
	}

	void set_top_item(int item) {
		top_item_ = SettingsUtils::clip(0, num_items() - 1, item);
	}

	void on_up_button() {
		int item = top_item() - collumns();
		if (item >= 0)  {
			set_top_item(item);
		}
	}

	void on_down_button() {
		int item = top_item() + collumns();
		if (item < num_items())  {
			set_top_item(item);
		}
	}

	void on_encoder(int index, int inc, bool shifted) {
		edit(top_item() + index, inc, shifted);
	}

	virtual const int num_items();
	virtual const char* item_text(int item);
	virtual const char* value_text(int item);
	virtual void edit(int item, int inc, bool shifted);

protected:
	Engine *engine_;
	Settings *settings_;

private:
	int top_item_ = 0;
};

#endif
