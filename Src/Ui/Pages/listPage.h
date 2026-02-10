#ifndef ListPage_h
#define ListPage_h

#include "confirmationPage.h"
#include "settingsList.h"

namespace ListPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;
	using TopPage::buttons_;

	SettingsList *list_;
	void(*clearCallback_)() = nullptr;
	bool(*pasteCallback_)() = nullptr;
	void(*copyCallback_)() = nullptr;

	void setList(SettingsList *list) {
		list_ = list;
	}

	void setClearCallback(void(*callback)()) {
		clearCallback_ = callback;
	}

	void setPasteCallback(bool(*callback)()) {
		pasteCallback_ = callback;
	}

	void setCopyCallback(void(*callback)()) {
		copyCallback_ = callback;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {
		clearCallback_ = nullptr;
		pasteCallback_ = nullptr;
		copyCallback_ = nullptr;
	}

	void on_button(int id, int state) {
		/*
		if (!state) {
			return;
		}

		switch (id)
		{
		case Buttons::UP_BUTTON:
			list_->onUpButton();
			break;
		case Buttons::DOWN_BUTTON:
			list_->onDownButton();
			break;
		case Buttons::CLEAR_BUTTON:
			if (clearCallback_) {
				ConfirmationPage::set("CLEAR SETTINGS ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						clearCallback_();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
			}
			break;
		case Buttons::COPY_BUTTON:
			if (copyCallback_) {
				copyCallback_();
				MessagePainter::show("SETTINGS COPIED");
			}
			break;
		case Buttons::PASTE_BUTTON:
			if (pasteCallback_) {
				ConfirmationPage::set("OVERWRITE SETTINGS ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						if (pasteCallback_()) {
							MessagePainter::show("SETTINGS PASTED");
						} else {
							MessagePainter::show("FAILED! CLIPBOARD EMPTY");
						}
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
			}
			break;
		default:
			break;
		}
			*/
	}

	void on_encoder(int id, int state) {
		/*
		int index = Buttons::encoderToFunction(id);
		if (index >= 0) {
			bool pressed = Buttons::encoderIsPressed(id);
			bool shifted = buttons_->isPressed(Buttons::SHIFT);
			list_->onEncoder(index, state, pressed || shifted);
		}
			*/
	}

	void refresh_leds() {
		
	}

	void draw() {
		const int h = 20;
		const int y = canvas_->height() - h;
		const int w = canvas_->width() / list_->collumns();

		canvas_->setFont(Font::SMALL);

		for (int i = 0; i < list_->collumns(); ++i) {
			int item = i + list_->topItem();
			if (item < list_->numItems()) {
				int x = i * w;
				canvas_->drawText(x, y, w, h, list_->itemText(item), Canvas::CENTER, Canvas::CENTER);
				canvas_->drawText(x, y + 10, w, h, list_->valueText(item), Canvas::CENTER, Canvas::CENTER);
			}
		}

		const int bar_w = 6;
		const int bar_x = canvas_->width() - bar_w;
		WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, list_->topItem(), list_->numItems(), list_->collumns());
	}

	const size_t target_fps() {
		return 1000 / 16;
	}

	Pages::Page page = {
		&init,
		&enter,
		&exit,
		&draw,
		&refresh_leds,
		&on_button,
		&on_encoder,
		&target_fps
	};
};

#endif
