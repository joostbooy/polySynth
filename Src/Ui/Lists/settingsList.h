#ifndef SettingsList_h
#define SettingsList_h

#include "settingsText.h"

class SettingsList {

public:

	void init(Engine *engine, Settings *settings) {
		engine_ = engine;
		settings_ = settings;
	}

	enum Mode {
		SELECT,
		EDIT
	};

	Mode mode() {
		return mode_;
	}

	void setMode(Mode mode) {
		mode_ = mode;
	}

	int selectedItem() {
		return selectedItem_;
	}

	void selectItem(int item) {
		selectedItem_ = SettingsUtils::clip(item, 0, numItems() - 1);
	}

	void onButton() {
		if (mode() == SELECT) {
			setMode(EDIT);
		} else {
			setMode(SELECT);
		}
	}

	void onEncoder(int inc, bool shifted) {
		if (mode_ == SELECT) {
			selectItem(selectedItem() + inc);
		} else {
			edit(selectedItem(), inc, shifted);
		}
	}

	virtual const int numItems();
	virtual const char* itemText(int item);
	virtual const char* valueText(int item);
	virtual void edit(int item, int inc, bool shifted);

protected:
	Engine *engine_;
	Settings *settings_;

private:
	Mode mode_;
	int selectedItem_ = 0;
};

#endif
