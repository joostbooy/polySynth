#ifndef FilterPage_h
#define FilterPage_h

#include "filter.h"
#include "filterList.h"
#include "listPage.h"

namespace FilterPage {

  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;
  using TopPage::canvas_;

  bool pasteable_;
  Filter filter_;
  FilterList filterList_;

  void clear() {
    settings_->filter().init();
    ui_->resetAllPots();
  }

  void randomise() {
    settings_->filter().randomise();
    ui_->resetAllPots();
  }

  void copy_() {
    filter_.paste(&settings_->filter());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->filter().paste(&filter_);
      ui_->resetAllPots();
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    filter_.init();
    filterList_.init(engine_, settings_, ui_);
  }

  void enter() {
    ListPage::setList(&filterList_);
    ListPage::setClearCallback(&clear);
    ListPage::setCopyCallback(&copy_);
    ListPage::setPasteCallback(&paste);
    ListPage::setRandomiseCallback(&randomise);
    ListPage::enter();
  }

  void exit() {
    ListPage::exit();
  }

  void onButton(int id, int state) {
    ListPage::onButton(id, state);
  }

  void onEncoder(int id, int state) {
    ListPage::onEncoder(id, state);
  }

  void refreshLeds() {
    ListPage::refreshLeds();
  }

  float gainDb(Filter::Type type, float freqHz, float cutoffHz, float resonance) {
    float r = freqHz / cutoffHz;
    float a = 1.0f - r * r;
    float b = r / resonance;
    float twoPole = a * a + b * b;  // shared denominator of the 2-pole filters

    switch (type) {
      case Filter::LOW_PASS_2_POLE: return -10.0f * std::log10(twoPole);
      case Filter::HIGH_PASS:       return 10.0f * std::log10((r * r) / (1.0f + r * r));
      case Filter::BAND_PASS:       return 10.0f * std::log10((b * b) / twoPole);
      //     case Filter::LowPass4: {
      //       return -20.0f * std::log10(twoPole);  // two identical 2-pole stages in series
      //    }
      default:
        break;
    }
    return 0.0f;
  }

  void draw() {
    ListPage::draw();

    const int x = 128;
    const int y = 12;
    const int h = 30;
    const int w = 100;

    const float minDb = -48.0f;
    const float maxDb = 24.0f;

    int lastX = 0;
    int lastY = 0;

    Filter& f = settings_->filter();

    for (int i = 0; i < w; ++i) {
      float freq = 20.0f * std::pow(1000.0f, (float)i / (w - 1));  // 20 Hz .. 20 kHz
      float res = gainDb(f.type(), freq, f.cutoff1() * 60.f, f.resonance1() * 4.f);
      float db = std::clamp(res, minDb, maxDb);

      int x_ = x + i;
      int y_ = y + (int)(h * (maxDb - db) / (maxDb - minDb));  // top = +24 dB

      if (i > 0) {
        canvas_->drawLine(lastX, lastY, x_, y_, Canvas::BLACK);
      }

      lastX = x_;
      lastY = y_;
    }
  }

  const size_t targetFps() {
    return 16;
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

};  // namespace FilterPage

#endif
