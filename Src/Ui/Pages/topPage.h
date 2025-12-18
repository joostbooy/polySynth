#ifndef TopPage_h
#define TopPage_h

#include "settings.h"
#include "engine.h"
#include "canvas.h"
#include "leds.h"
#include "pages.h"
#include "controller.h"
#include "messagePainter.h"
#include "textBufferPainter.h"
#include "windowPainter.h"

namespace TopPage {

	Settings *settings_;
	Engine *engine_;
	Canvas *canvas_;
	Pages *pages_;
	Leds *leds_;
	Ui *ui_;
	Disk *disk_;

	StringBuilderBase<63>str_;

	int chapter_top_page_ = 0;
	const int kMaxPageRows = 6;
	Chapter chapter_;


	void scroll_to_chapter_page() {
		int page = chapter_.entry()->page_index();

		if (page < chapter_top_page_) {
			chapter_top_page_ = page;
		} else if (page >= (chapter_top_page_ + kMaxPageRows)) {
			chapter_top_page_ = page - (kMaxPageRows - 1);
		}
	}

	void init(Settings *settings, Engine *engine, Ui *ui) {
		settings_ = settings;
		engine_ = engine;
		ui_ = ui;
		canvas_ = &ui->canvas();
		leds_ = &ui->leds();
		pages_ = &ui->pages();
		disk_ = settings_->disk();

		Controller::init();
		MessagePainter::init(canvas_);
		TextBufferPainter::init(canvas_);
		WindowPainter::init(canvas_);

		chapter_.init();
		chapter_.set(Chapter::INSTRUMENT);
	}

	void enter() {

	}

	void exit() {

	}

	void on_button(int id, int state) {
		Controller::on_button(id, state);

		int chapter_id = Controller::button_to_chapter(id);

		if (chapter_id >= 0) {
			if (state) {
				chapter_.set(chapter_id);
				pages_->close_all();
				scroll_to_chapter_page();
			} else {
				pages_->open(Pages::PageId(chapter_.entry()->selected_page()));
			}
		}
	}

	void on_encoder(int id, int state) {
		Controller::on_encoder(id);

		if (pages_->num_open() == 0) {
			int index = chapter_.entry()->page_index();
			chapter_.entry()->set_page_index(index + state);
			scroll_to_chapter_page();
		}
	}

	void refresh_leds() {
		leds_->set_chapter(chapter_.selected());
	}

	void draw() {
		if (pages_->num_open() == 0) {
			const int row_h = 8;
			const int w = 128;
			const int h = row_h * kMaxPageRows;
			const int x = (canvas_->width() - w) / 2;
			const int y = (canvas_->height() - h) / 2;

			canvas_->draw_text(x, 0, w, row_h, chapter_.name(), Canvas::CENTER, Canvas::CENTER);

			for (int i = 0; i < kMaxPageRows; ++i) {
				int page = i + chapter_top_page_;
				int page_y = (i * row_h) + y;

				if (page < chapter_.entry()->size()) {
					canvas_->draw_text(x + 4, page_y, chapter_.entry()->name(page));
					if (page == chapter_.entry()->page_index()) {
						canvas_->fill(x, page_y, w, row_h, Canvas::INVERTED);
					}
				}
			}

			const int bar_w = 8;
			const int bar_x = x + (w - bar_w);
			WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, chapter_top_page_, chapter_.entry()->size(), kMaxPageRows);
		}

		MessagePainter::draw(pages_->target_fps());
	}

};

#endif
