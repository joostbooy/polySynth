#ifndef LfoPage_h
#define LfoPage_h

#include "lfo.h"
#include "lfoList.h"
#include "listPage.h"

namespace LfoPage {

  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Lfo lfo_;
  LfoList lfoList_;

  float speed = 1.f;
  const int w = 128;

  void clear() {
    settings_->selectedLfo().init();
    ui_->resetAllPots();
  }

  void copy() {
    lfo_.paste(&settings_->selectedLfo());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->selectedLfo().paste(&lfo_);
      ui_->resetAllPots();
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    lfo_.init();
    lfoList_.init(engine_, settings_);

    // find best increment value for lfo drawing
    float value = 1.f;
    float targetInc = 1.f / w;
    size_t size = LookupTablesUtils::size(lut_phase_inc);

    for (size_t i = 0; i < size; i++) {
      float currentValue = SettingsUtils::difference(lut_phase_inc[i], targetInc);
      if (currentValue < value) {
        value = currentValue;
        speed = (1.f / size) * i;
      }
    }
  }

  void enter() {
    ListPage::setList(&lfoList_);
    ListPage::setClearCallback(&clear);
    ListPage::setCopyCallback(&copy);
    ListPage::setPasteCallback(&paste);
    ListPage::enter();
  }

  void exit() {
    ListPage::exit();
  }

  void on_button(int id, int state) {
    ListPage::on_button(id, state);
  }

  void on_encoder(int id, int state) {
    ListPage::on_encoder(id, state);
  }

  void refresh_leds() {
    ListPage::refresh_leds();
  }

  void draw() {
    ListPage::draw();

    Lfo lfo;
    LfoEngine lfoEngine;

    lfo.paste(&settings_->selectedLfo());
    lfo.setClockSync(false);
    lfo.setRandomise(false);
    lfo.setSpeed(speed);  
    lfoEngine.init(&lfo);

    const int x = 64;
    const int y = 5;
    const int h = 32;

    for (int x2 = 0; x2 < w; ++x2) {
      int y2 = h * (1.f - lfoEngine.next());
      canvas_->drawPixel(x + x2, y + y2, Canvas::BLACK);
    }

    int index = settings_->lfoIndex();
    float phase = engine_->voiceEngine().mostRecentVoice().lfoEngine(index).phase();
    canvas_->verticalLine(x + (phase * w), y, h, Canvas::BLACK);
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

};  // namespace LfoPage

#endif
