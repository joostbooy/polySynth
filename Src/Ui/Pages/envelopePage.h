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

  void randomise() {
    settings_->selectedEnvelope().randomise();
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
        time = (1.f / size) * i;
      }
    }
  }

  void enter() {
    ListPage::setList(&envelopeList_);
    ListPage::setClearCallback(&clear);
    ListPage::setCopyCallback(&copy);
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

  float getTime(float value) {
    return time + (1.f - time) * value;
  }

  void draw() {
    ListPage::draw();

    Envelope envelope;
    EnvelopeEngine envelopeEngine;
    envelope.paste(&settings_->selectedEnvelope());

    envelope.setClockSync(false);

    float holdTime;
    if (envelope.mode() == Envelope::TRIGGER) {
      holdTime = getTime(envelope.holdTime());
    } else {
      holdTime = time;
    }

    envelope.setMode(Envelope::TRIGGER);
    envelope.setAttackTime(getTime(envelope.attackTime()));
    envelope.setDecayTime(getTime(envelope.decayTime()));
    envelope.setHoldTime(holdTime);
    envelope.setReleaseTime(getTime(envelope.releaseTime()));

    envelopeEngine.init(&envelope);
    envelopeEngine.attack();

    const int x = 128;
    const int y = 10;
    const int h = 32;

    int lastX;
    int lastY;

    int stageX[EnvelopeEngine::NUM_STAGES];
    EnvelopeEngine::Stage lastStage = EnvelopeEngine::ATTACK;

    // Draw envelope
    for (int x2 = 0; x2 < w; ++x2) {
      int x_ = x + x2;
      int y_ = y + (h * (1.f - envelopeEngine.next()));
      if (x2 > 0) {
        canvas_->drawLine(lastX, lastY, x_, y_, Canvas::BLACK);
      }
      lastX = x_;
      lastY = y_;

      if (envelopeEngine.stage() > lastStage) {
        stageX[envelopeEngine.stage()] = x_;
        lastStage = envelopeEngine.stage();
      }
    }

    // Draw phase
    Canvas::Color color;
    int index = settings_->envelopeIndex();

    for (size_t i = 0; i < Settings::kNumVoices; i++) {
      EnvelopeEngine& e = engine_->voiceEngine().voice(i).envelopeEngine(index);
      if (i == engine_->voiceEngine().mostRecentVoice().index()) {
        color = Canvas::BLACK;
      } else {
        color = Canvas::GRAY;
      }

      int x1 = x;
      int x2 = x;

      switch (e.stage()) {
        case EnvelopeEngine::ATTACK:
          x1 = x;
          x2 = stageX[EnvelopeEngine::DECAY];
          break;
        case EnvelopeEngine::DECAY:
          x1 = stageX[EnvelopeEngine::DECAY];
          x2 = stageX[EnvelopeEngine::HOLD];
          break;
        case EnvelopeEngine::SUSTAIN:
        case EnvelopeEngine::HOLD:
          x1 = stageX[EnvelopeEngine::HOLD];
          x2 = stageX[EnvelopeEngine::RELEASE];
          break;
        case EnvelopeEngine::RELEASE:
          x1 = stageX[EnvelopeEngine::RELEASE];
          x2 = stageX[EnvelopeEngine::IDLE];
          break;
        default:
          break;
      }

      canvas_->verticalLine(x1 + (x2 - x1) * e.phase(), y, h, color);
    }
  }

  const size_t targetFps() {
    return 32;
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
