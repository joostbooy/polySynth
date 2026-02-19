#ifndef EnvelopePage_h
#define EnvelopePage_h

#include "envelope.h"
#include "envelopeList.h"
#include "listPage.h"

namespace EnvelopePage {

  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Envelope envelope_;
  EnvelopeList envelopeList_;

  float time = 1.f;
  const int w = 128;

  void clear() {
    settings_->selectedEnvelope().init();
    ui_->resetAllPots();
  }

  void copy() {
    envelope_.paste(&settings_->selectedEnvelope());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->selectedEnvelope().paste(&envelope_);
      ui_->resetAllPots();
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    envelope_.init();
    envelopeList_.init(engine_, settings_);

    // find best increment value for envelope drawing
    float value = 1.f;
    float targetInc = 1.f / (w / 4);
    for (size_t i = 0; i < PHASE_TABLE_SIZE; i++) {
      float currentValue = SettingsUtils::difference(lut_phase_inc[i], targetInc);
      if (currentValue < value) {
        value = currentValue;
        time = (1.f / PHASE_TABLE_SIZE) * i;
      }
    }
  }

  void enter() {
    ListPage::setList(&envelopeList_);
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

    Envelope envelope;
    EnvelopeEngine envelopeEngine;
    envelope.paste(&settings_->selectedEnvelope());

    envelope.setClockSync(false);

    float holdTime;
    if (envelope.mode() == Envelope::TRIGGER) {
      holdTime = Dsp::cross_fade(time, 1.0f, envelope.holdTime());
    } else {
      holdTime = time;
    }

    envelope.setMode(Envelope::TRIGGER);
    envelope.setAttackTime(Dsp::cross_fade(time, 1.0f, envelope.attackTime()));
    envelope.setDecayTime(Dsp::cross_fade(time, 1.0f, envelope.decayTime()));
    envelope.setHoldTime(holdTime);
    envelope.setSustainLevel(envelope.sustainLevel());
    envelope.setReleaseTime(Dsp::cross_fade(time, 1.0f, envelope.releaseTime()));

    envelopeEngine.init(&envelope);
    envelopeEngine.attack();

    const int x = 64;
    const int y = 5;
    const int h = 32;

    for (int x2 = 0; x2 < w; ++x2) {
      int y2 = h * (1.f - envelopeEngine.next());
      canvas_->drawPixel(x + x2, y + y2, Canvas::BLACK);
    }

    int index = settings_->envelopeIndex();
    float phase = engine_->voiceEngine().mostRecentVoice().envelopeEngine(index).phase();
    canvas_->verticalLine(x + (phase * w), y, h, Canvas::BLACK);
  }

  const size_t target_fps() {
    return 1000 / 24;
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

};  // namespace EnvelopePage

#endif
