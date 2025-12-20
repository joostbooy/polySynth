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

	int topItem() {
		return topItem_;
	}

	void setTopItem(int item) {
		topItem_ = SettingsUtils::clip(0, numItems() - 1, item);
	}

	void onUpButton() {
		int item = topItem() - collumns();
		if (item >= 0)  {
			setTopItem(item);
		}
	}

	void onDownButton() {
		int item = topItem() + collumns();
		if (item < numItems())  {
			setTopItem(item);
		}
	}

	void onEncoder(int index, int inc, bool shifted) {
		edit(topItem() + index, inc, shifted);
	}

	virtual const int numItems();
	virtual const char* itemText(int item);
	virtual const char* valueText(int item);
	virtual void edit(int item, int inc, bool shifted);

protected:
	Engine *engine_;
	Settings *settings_;

private:
	int topItem_ = 0;
};

#endif
