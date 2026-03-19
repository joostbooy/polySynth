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
  const int w = 100;

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
    envelopeList_.init(engine_, settings_, ui_);

    // find best increment value for envelope drawing
    float value = 1.f;
    float targetInc = 1.f / (w / 4);
    size_t size = LookupTablesUtils::size(lut_phase_inc);
    
    for (size_t i = 0; i < size; i++) {
      float currentValue = SettingsUtils::difference(lut_phase_inc[i], targetInc);
      if (currentValue < value) {
        value = currentValue;
        time = (1.f / size) * (size - i);
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

  void onButton(int id, int state) {
    ListPage::onButton(id, state);
  }

  void onEncoder(int id, int state) {
    ListPage::onEncoder(id, state);
  }

  void refreshLeds() {
    ListPage::refreshLeds();
  }

  void draw() {
    ListPage::draw();

    Envelope envelope;
    EnvelopeEngine envelopeEngine;
    envelope.paste(&settings_->selectedEnvelope());

    envelope.setClockSync(false);

    float holdTime;
    if (envelope.mode() == Envelope::TRIGGER) {
      holdTime = time * envelope.holdTime();
    } else {
      holdTime = time;
    }

    envelope.setMode(Envelope::TRIGGER);
    envelope.setAttackTime(time * envelope.attackTime());
    envelope.setDecayTime(time * envelope.decayTime());
    envelope.setHoldTime(holdTime);
    envelope.setSustainLevel(envelope.sustainLevel());
    envelope.setReleaseTime(time * envelope.releaseTime());

    envelopeEngine.init(&envelope);
    envelopeEngine.attack();

    const int x = 128;
    const int y = 10;
    const int h = 32;

    int lastX;
    int lastY;

    for (int x2 = 0; x2 < w; ++x2) {
      int x_ = x + x2;
      int y_ = y + (h * (1.f - envelopeEngine.next()));
      if (x2 > 0) {
        canvas_->drawLine(lastX, lastY, x_, y_, Canvas::BLACK);
      }
      lastX = x_;
      lastY = y_;
    }

    int index = settings_->envelopeIndex();
    float phase = engine_->voiceEngine().mostRecentVoice().envelopeEngine(index).phase();
    canvas_->verticalLine(x + (phase * w), y, h, Canvas::BLACK);
  }

  const size_t targetFps() {
    return 1000 / 24;
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

};  // namespace EnvelopePage

#endif
