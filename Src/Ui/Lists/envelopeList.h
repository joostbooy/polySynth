#ifndef EnvelopeList_h
#define EnvelopeList_h

#include "settings.h"
#include "settingsList.h"

class EnvelopeList : public SettingsList {
 public:
  enum Item {
    ENVELOPE,
    MODE,
    CLOCK_SYNC,
    ATTACK_TIME,
    ATTACK_SHAPE,
    DECAY_TIME,
    DECAY_SHAPE,
    SUSTAIN_LEVEL,
    HOLD_TIME,
    RELEASE_TIME,
    RELEASE_SHAPE,
    LOOP,
    INVERT,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case ENVELOPE:
        return "ENVELOPE";
      case MODE:
        return "MODE";
      case CLOCK_SYNC:
        return "CLOCK SYNC";
      case ATTACK_TIME:
        return "ATTACK TIME";
      case ATTACK_SHAPE:
        return "ATTACK SHAPE";
      case DECAY_TIME:
        return "DECAY TIME";
      case DECAY_SHAPE:
        return "DECAY SHAPE";
      case SUSTAIN_LEVEL:
        return "SUSTAIN LEVEL";
      case HOLD_TIME:
        return "HOLD TIME";
      case RELEASE_TIME:
        return "RELEASE TIME";
      case RELEASE_SHAPE:
        return "RELEASE SHAPE";
      case LOOP:
        return "LOOP";
      case INVERT:
        return "INVERT";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Envelope& envelope = settings_->selectedEnvelope();

    switch (item) {
      case ENVELOPE:
        return SettingsText::intToText(settings_->envelopeIndex() + 1);
      case MODE:
        return envelope.modeText();
      case CLOCK_SYNC:
        return envelope.clockSyncText();
      case ATTACK_TIME:
        return envelope.attackTimeText();
      case ATTACK_SHAPE:
        return envelope.attackShapeText();
      case DECAY_TIME:
        return envelope.decayTimeText();
      case DECAY_SHAPE:
        return envelope.decayShapeText();
      case SUSTAIN_LEVEL:
        return envelope.sustainLevelText();
      case HOLD_TIME:
        return envelope.holdTimeText();
      case RELEASE_TIME:
        return envelope.releaseTimeText();
      case RELEASE_SHAPE:
        return envelope.releaseShapeText();
      case LOOP:
        return envelope.loopText();
      case INVERT:
        return envelope.invertText();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Envelope& envelope = settings_->selectedEnvelope();

    switch (item) {
      case ENVELOPE:
        settings_->setEnvelopeIndex(settings_->envelopeIndex() + inc);
        break;
      case MODE:
        envelope.setMode(envelope.mode() + inc);
        break;
      case CLOCK_SYNC:
        envelope.setClockSync(inc > 0);
        break;
      case ATTACK_TIME:
        envelope.setAttackTime(envelope.attackTime() + SettingsUtils::fInc(inc, shifted));
        break;
      case ATTACK_SHAPE:
        envelope.setAttackShape(envelope.attackShape() + SettingsUtils::fInc(inc, shifted));
        break;
      case DECAY_TIME:
        envelope.setDecayTime(envelope.decayTime() + SettingsUtils::fInc(inc, shifted));
        break;
      case DECAY_SHAPE:
        envelope.setDecayShape(envelope.decayShape() + SettingsUtils::fInc(inc, shifted));
        break;
      case SUSTAIN_LEVEL:
        envelope.setSustainLevel(envelope.sustainLevel() + SettingsUtils::fInc(inc, shifted));
        break;
      case HOLD_TIME:
        envelope.setHoldTime(envelope.holdTime() + SettingsUtils::fInc(inc, shifted));
        break;
      case RELEASE_TIME:
        envelope.setReleaseTime(envelope.releaseTime() + SettingsUtils::fInc(inc, shifted));
        break;
      case RELEASE_SHAPE:
        envelope.setReleaseShape(envelope.releaseShape() + SettingsUtils::fInc(inc, shifted));
        break;
      case LOOP:
        envelope.setLoop(inc > 0);
        break;
      case INVERT:
        envelope.setLoop(inc > 0);
        break;
      default:
        break;
    }
  }

 private:
};

#endif
