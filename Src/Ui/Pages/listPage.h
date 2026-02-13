#ifndef ListPage_h
#define ListPage_h

#include "confirmationPage.h"
#include "settingsList.h"

namespace ListPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::leds_;
  using TopPage::pages_;

  SettingsList* list_;
  void (*clearCallback_)() = nullptr;
  bool (*pasteCallback_)() = nullptr;
  void (*copyCallback_)() = nullptr;

  int topRow_;
  const int kMaxVisibleRows_ = 6;

  enum Footeroptions { CLEAR, COPY, PASTE, NUM_FOOTER_OPTIONS };
  const char* const footerText[NUM_FOOTER_OPTIONS] = {"CLEAR", "COPY", "PASTE"};

  void setList(SettingsList* list) {
    list_ = list;
  }

  void setClearCallback(void (*callback)()) {
    clearCallback_ = callback;
  }

  void setPasteCallback(bool (*callback)()) {
    pasteCallback_ = callback;
  }

  void setCopyCallback(void (*callback)()) {
    copyCallback_ = callback;
  }

  void scroll_to_row(int row) {
    if (row < topRow_) {
      topRow_ = row;
    } else if (row >= (topRow_ + kMaxVisibleRows_)) {
      topRow_ = row - (kMaxVisibleRows_ - 1);
    }
  }

  void init() {
  }

  void enter() {
    topRow_ = 0;
    list_->selectItem(0);
    list_->setMode(SettingsList::SELECT);
  }

  void exit() {
    clearCallback_ = nullptr;
    pasteCallback_ = nullptr;
    copyCallback_ = nullptr;
  }

  void on_button(int id, int state) {
    if (!state) {
      return;
    }

    if (id == Buttons::ENC_X || id == Buttons::ENC_Y) {
      list_->onButton();
      return;
    }

    switch (buttons_->toFunction(id)) {
      case CLEAR:
        if (clearCallback_) {
          ConfirmationPage::set("CLEAR SETTINGS ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              clearCallback_();
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
        }
        break;
      case COPY:
        if (copyCallback_) {
          copyCallback_();
          MessagePainter::show("SETTINGS COPIED");
        }
        break;
      case PASTE:
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
  }

  void on_encoder(int id, int inc) {
    list_->onEncoder(inc, false);
  }

  void refresh_leds() {
  }

  void draw() {
    const int row_h = 8;
    const int w = canvas_->width();
    const int collWidth = w / 4;
    const int h = row_h * kMaxVisibleRows_;
    const int x = 0;
    const int y = 10;

    canvas_->setFont(Font::SMALL);

    for (int i = 0; i < kMaxVisibleRows_; ++i) {
      int row = i + topRow_;
      int row_y = (i * row_h) + y;

      if (row < list_->numItems()) {
        Canvas::Color color = (row == list_->selectedItem()) ? Canvas::BLACK : Canvas::GRAY;

        canvas_->drawText(x + 4, row_y, collWidth - 8, row_h, list_->itemText(row), Canvas::LEFT, Canvas::CENTER, color);
        canvas_->drawText(x + 4 + collWidth, row_y, collWidth - 8, row_h, list_->valueText(row), Canvas::LEFT, Canvas::CENTER, color);
        if (list_->mode() == SettingsList::EDIT && row == list_->selectedItem()) {
          canvas_->fill(x + 1, row_y + 1, w - 2, row_h - 2, Canvas::INVERTED);
        }
      }
    }

    const int bar_w = 8;
    const int bar_x = x + (w - bar_w);
    WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, topRow_, list_->numItems(), kMaxVisibleRows_);
    WindowPainter::draw_footer(footerText, NUM_FOOTER_OPTIONS);
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
      &target_fps,
  };
};  // namespace ListPage

#endif
