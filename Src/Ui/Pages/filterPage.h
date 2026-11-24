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

  float gainDb(int type, float freqHz, float cutoffHz, float resonance) {
    float r = freqHz / cutoffHz;
    float a = 1.0f - r * r;
    float b = r / resonance;
    float twoPole = a * a + b * b;  // shared denominator of the 2-pole filters

    switch (type) {
      case Filter::LOW_PASS_2_POLE: return -10.0f * std::log10(twoPole);
      case Filter::HIGH_PASS:       return 10.0f * std::log10((r * r) / (1.0f + r * r));
      case Filter::BAND_PASS:       return 10.0f * std::log10((b * b) / twoPole);
      case 3: /* 4PLP */            return -20.0f * std::log10(twoPole); 
      default:
        break;
    }
    return 0.0f;
  }

  void draw() {
    ListPage::draw();

    const int x = 128;
    const int w = 100;
    const int h = 18;
    const int yFilter1 = 12;
    const int yFilter2 = 12 + h + 2;

    int lastX[2] = {x, x};
    int lastY[2] = {yFilter1 + h, yFilter2 + h};

    const float minDb = -48.0f;
    const float maxDb = 24.0f;

    Filter& f = settings_->filter();

    for (int i = 0; i < w; ++i) {
      // Filter 1
      float freq = 20.0f * std::pow(1000.0f, (float)i / (w - 1));  // 20 Hz .. 20 kHz
      float res = gainDb(f.type(), freq, f.cutoff1() * 60.f, f.resonance1() * 4.f);
      float db = std::clamp(res, minDb, maxDb);

      int x_ = x + i;
      int y_ = yFilter1 + (int)(h * (maxDb - db) / (maxDb - minDb));  // top = +24 dB

      if (i > 0) {
        canvas_->drawLine(lastX[0], lastY[0], x_, y_, Canvas::BLACK);
      }

      lastX[0] = x_;
      lastY[0] = y_;

      // Filter 2
      res = gainDb(3, freq, f.cutoff2() * 60.f, f.resonance2() * 4.f);
      db = std::clamp(res, minDb, maxDb);

      y_ = yFilter2 + (int)(h * (maxDb - db) / (maxDb - minDb));  // top = +24 dB

      if (i > 0) {
        canvas_->drawLine(lastX[1], lastY[1], x_, y_, Canvas::BLACK);
      }

      lastX[1] = x_;
      lastY[1] = y_;
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
