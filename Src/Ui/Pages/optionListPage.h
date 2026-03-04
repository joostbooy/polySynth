#ifndef OptionListPage_h
#define OptionListPage_h

#include "settingsUtils.h"
#include "topPage.h"

namespace OptionListPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::leds_;
  using TopPage::pages_;

  int count_;
  int selected_;
  int topRow_;
  const char* const* text_;
  void (*callback_)(int);
  const int kMaxRows = 4;

  enum FooterOptions {
    CONFIRM,
    CANCEL,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"CONFIRM", "CANCEL"};

  void setText(const char* const* text) {
    text_ = text;
  }

  void setCallback(void (*callback)(int)) {
    callback_ = callback;
  }

  void setCount(int count) {
    count_ = count;
  }

  void scrollToRow(int row) {
    if (row < topRow_) {
      topRow_ = row;
    } else if (row >= (topRow_ + kMaxRows)) {
      topRow_ = row - (kMaxRows - 1);
    }
  }

  void init() {
    callback_ = nullptr;
  }

  void enter() {
    selected_ = 0;
    scrollToRow(0);
  }

  void exit() {
    callback_ = nullptr;
  }

  void onEncoder(int id, int state) {
    selected_ = SettingsUtils::clip(0, count_ - 1, selected_ + state);
    scrollToRow(selected_);
  }

  void onButton(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case CONFIRM:
          if (callback_) {
            callback_(selected_);
          }
          pages_->close(Pages::OPTION_LIST_PAGE);
          break;
        case CANCEL:
          pages_->close(Pages::OPTION_LIST_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void refreshLeds() {
  }

  void draw() {
    const int row_h = 8;
    const int w = 128;
    const int h = row_h * kMaxRows;
    const int x = (canvas_->width() - w) / 2;
    const int y = (canvas_->height() - h) / 2;

    canvas_->setFont(Font::SMALL);
    canvas_->fill(x + 5, y + 5, w, h, Canvas::SUBTRACTED);

    canvas_->fill(x, y, w, h, Canvas::WHITE);
    canvas_->frame(x, y, w, h, Canvas::BLACK);

    for (int i = 0; i < kMaxRows; ++i) {
      int row = i + topRow_;
      int row_y = (i * row_h) + y;

      if (row < count_) {
        canvas_->drawText(x + 4, row_y, w - 8, row_h, text_[row], Canvas::LEFT, Canvas::CENTER);
        if (row == selected_) {
          canvas_->fill(x + 1, row_y + 1, w - 2, row_h - 2, Canvas::INVERTED);
        }
      }
    }

    const int bar_w = 8;
    const int bar_x = x + (w - bar_w);
    WindowPainter::drawVerticalScollbar(bar_x, y, bar_w, h, topRow_, count_, kMaxRows);

    WindowPainter::drawFooter(footerOptionText, NUM_FOOTER_OPTIONS);
  }

  const size_t targetFps() {
    return 1000 / 16;
  }

  Pages::Page page = {
      &init,
      &enter,
      &exit,
      &draw,
      &refreshLeds,
      &onButton,
      &onEncoder,
      &targetFps,
  };

};  // namespace OptionListPage

#endif
