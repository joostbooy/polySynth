#ifndef ModMatrixPage_h
#define ModMatrixPage_h

#include "topPage.h"
#include "modMatrixDepthPage.h"

namespace ModMatrixPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::leds_;
  using TopPage::pages_;
  using TopPage::settings_;

  int src_ = 0;
  int top_src_ = 0;
  const int kMaxVisibleSources = 4;

  int dest_ = 0;
  int top_dest_ = 0;
  const int kMaxVisibleDestinations = 4;

  bool pasteable_ = false;
  ModMatrix modMatrix_;

  enum FooterOptions {
    COPY,
    PASTE,
    CLEAR,
    SET_DEPTH,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"COPY", "PASTE", "CLEAR", "SET DEPTH"};

  void scroll_to_source(int src) {
    src_ = SettingsUtils::clip(0, ModMatrix::NUM_SOURCES - 1, src);

    if (src_ < top_src_) {
      top_src_ = src_;
    } else if (src_ >= (top_src_ + kMaxVisibleSources)) {
      top_src_ = src_ - (kMaxVisibleSources - 1);
    }
  }

  void scroll_to_destination(int dest) {
    dest_ = SettingsUtils::clip(0, ModMatrix::NUM_DESTINATIONS - 1, dest);

    if (dest_ < top_dest_) {
      top_dest_ = dest_;
    } else if (dest_ >= (top_dest_ + kMaxVisibleDestinations)) {
      top_dest_ = dest_ - (kMaxVisibleDestinations - 1);
    }
  }

  void init() {
  }

  void enter() {
  }

  void exit() {
  }

  void on_encoder(int id, int inc) {
    if (id == 0) {
      scroll_to_source(src_ + inc);
    } else {
      scroll_to_destination(dest_ + inc);
    }
  }

  void on_button(int id, int state) {
    if (state) {
      if (id == Buttons::ENC_X || id == Buttons::ENC_Y) {
        settings_->modMatrix().toggle(src_, dest_);
        return;
      }

      switch (buttons_->toFunction(id)) {
        case COPY:
          modMatrix_.paste(&settings_->modMatrix());
          pasteable_ = true;
          break;
        case PASTE:
          if (pasteable_) {
            ConfirmationPage::set("OVERWRITE MATRIX ?", [](int option) {
              if (option == ConfirmationPage::CONFIRM) {
                settings_->modMatrix().paste(&modMatrix_);
              }
            });
            pages_->open(Pages::CONFIRMATION_PAGE);
          }
          break;
        case CLEAR:
          ConfirmationPage::set("CLEAR MATRIX ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              settings_->modMatrix().clear();
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
          break;
          case SET_DEPTH:
          ModMatrixDepthPage::setDestination(dest_);
          pages_->open(Pages::MOD_MATRIX_DEPTH_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void refresh_leds() {
  }

  void draw_sources_text(int x, int y, int w, int h, int coll_w, int row_h) {
    for (int i = 0; i < kMaxVisibleSources; ++i) {
      int src = i + top_src_;
      int src_y = (i * row_h) + y;

      if (src < ModMatrix::NUM_SOURCES) {
        canvas_->drawText(x + 4, src_y, coll_w, row_h, settings_->modMatrix().source_text(src), Canvas::CENTER, Canvas::CENTER);
        if (src == src_) {
          canvas_->fill(coll_w + 1, src_y + 1, w - 2, row_h - 2, Canvas::LIGHT_GRAY);
        }
      }
    }
  }

  void draw_destination_text(int x, int y, int w, int h, int coll_w, int row_h) {
    for (int i = 0; i < kMaxVisibleDestinations; ++i) {
      int dest = i + top_dest_;
      int dest_x = (i * coll_w) + x;

      if (dest < ModMatrix::NUM_DESTINATIONS) {
        canvas_->drawText(dest_x, y, coll_w, row_h, ModMatrix::destination_text(dest), Canvas::CENTER, Canvas::CENTER);
        if (dest == dest_) {
          canvas_->fill(dest_x + 1, row_h + 1, coll_w - 2, h - 2, Canvas::LIGHT_GRAY);
        }
      }
    }
  }

  void draw_matrix(int x, int y, int w, int h, int coll_w, int row_h) {
    for (int i = 0; i < kMaxVisibleDestinations; ++i) {
      int dest = i + top_dest_;
      int dest_x = (i * coll_w) + x;

      for (int j = 0; j < kMaxVisibleSources; ++j) {
        int src = j + top_src_;
        int src_y = (j * row_h) + y;

        if (settings_->modMatrix().read(src, dest)) {
          canvas_->fill(dest_x + 1, src_y + 1, coll_w - 2, row_h - 2, Canvas::BLACK);
        }
      }
    }

    WindowPainter::draw_vertical_scollbar(256 - 8, y, 6, h, top_src_, ModMatrix::NUM_SOURCES, kMaxVisibleSources);
    WindowPainter::draw_horizontal_scollbar(coll_w, 64 - 8, w - coll_w, 6, top_dest_, ModMatrix::NUM_DESTINATIONS, kMaxVisibleDestinations);
  }

  void draw() {
    const int x = 0;
    const int y = 0;
    const int w = canvas_->width() - 10;
    const int h = canvas_->height() - 10;
    const int coll_w = w / (kMaxVisibleSources + 1);
    const int row_h = h / (kMaxVisibleDestinations + 1);

    canvas_->clear();

    draw_sources_text(x, row_h, w, h, coll_w, row_h);
    draw_destination_text(coll_w, y, w, h, coll_w, row_h);
    draw_matrix(x + coll_w, y + row_h, w - coll_w, h - row_h, coll_w, row_h);

    WindowPainter::draw_footer(footerOptionText, NUM_FOOTER_OPTIONS);
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

};  // namespace ModMatrixPage

#endif
