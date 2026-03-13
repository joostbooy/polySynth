#ifndef HardwareTestPage_h
#define HardwareTestPage_h

#include "buttons.h"
#include "pots.h"
#include "textBufferPainter.h"

namespace HardwareTestPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::leds_;
  using TopPage::pages_;
  using TopPage::pots_;
  using TopPage::settings_;
  using TopPage::str_;

  bool showProcessingTime_;
  bool potsEnabled;
  uint8_t lastPotValue[Pots::NUM_POTS];

  enum FooterOptions {
    TOGGLE_LEDS,
    TEST_SD_CARD,
    ENABLE_POTS,
    CLOSE,
    NUM_OPTIONS,
  };

  const char* const footerOptionText_[NUM_OPTIONS] = {"TOGGLE LEDS", "TEST SD CARD", "ENABLE POTS", "CLOSE"};

  bool ledToggleState_;

  void testPots() {
    for (size_t i = 0; i < Pots::NUM_POTS; i++) {
      uint8_t value = pots_->read(i) * 255;
      if (value != lastPotValue[i]) {
        lastPotValue[i] = value;
        TextBufferPainter::write(str_.write(pots_->idText(i), " ", value));
      }
    }
  }

  void init() {
  }

  void enter() {
    potsEnabled = false;
    ledToggleState_ = true;
  }

  void exit() {
  }

  void onButton(int id, int state) {
    TextBufferPainter::write(str_.write(buttons_->idText(id), " ", state));

    if (state) {
      if (buttons_->isPressed(Buttons::SHIFT)) {
        switch (buttons_->toFunction(id)) {
          case TOGGLE_LEDS:
            ledToggleState_ ^= 1;
            break;
          case TEST_SD_CARD:
            if (settings_->disk()->mount()) {
              MessagePainter::show("SD MOUNTED");
            } else {
              MessagePainter::show("ERROR");
            }
            break;
          case ENABLE_POTS:
            potsEnabled ^= 1;
            if (potsEnabled) {
              for (size_t i = 0; i < Pots::NUM_POTS; i++) {
                lastPotValue[i] = pots_->read(i) * 255;
              }
            }
            break;
          case CLOSE:
            pages_->close(Pages::HARDWARE_TEST_PAGE);
            pages_->open(Pages::PATCH_PAGE);
            break;
          default:
            break;
        }
      }
    }
  }

  void onEncoder(int id, int state) {
    TextBufferPainter::write(str_.write(id == 0 ? "Y ENC " : "X ENC ", state));
  }

  void refreshLeds() {
    if (ledToggleState_) {
      leds_->setAll(Leds::RED);
    } else {
      leds_->setAll(Leds::BLACK);
    }
  }

  void drawProcessingTime() {
    const int w = canvas_->width();
    const int x = w - (w / 4);
    const int y = (canvas_->height() - 20) / 2;

    float percentage = (engine_->processingTimeUs() / SAMPLE_RATE) * 100.f;
    const char* text = SettingsText::floatToText(percentage, "%");
    canvas_->drawText(x, y, "PROCESSING TIME");
    canvas_->drawText(x, y + 10, text);
  }

  void draw() {
    if (potsEnabled) {
      testPots();
    }

    drawProcessingTime();
    TextBufferPainter::draw();
    WindowPainter::drawFooter(footerOptionText_, NUM_OPTIONS);
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

};  // namespace HardwareTestPage

#endif
