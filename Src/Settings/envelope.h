#ifndef Envelope_h
#define Envelope_h

#include "fileReader.h"
#include "fileWriter.h"
#include "midiSync.h"
#include "settingsText.h"
#include "settingsUtils.h"
#include "lookupTablesUtils.h"


class Envelope {
 public:
  enum Mode {
    GATE,
    TRIGGER,

    NUM_MODES
  };

  static const char* modeText(uint8_t value) {
    switch (value) {
      case GATE:    return "GATE";
      case TRIGGER: return "TRIGGER";
      default:
        break;
    }
    return nullptr;
  }

  void init() {
    setClockSync(false);
    setMode(GATE);
    setAttackTime(0);
    setAttackShape(0.5f);
    setDecayTime(0);
    setDecayShape(0.5f);
    setHoldTime(0);
    setSustainLevel(0.5f);
    setReleaseTime(0);
    setReleaseShape(0.5f);
    setLoop(false);
    setInvert(false);
  }

  // Clock sync
  bool clockSync() {
    return clockSync_;
  }

  void setClockSync(bool value) {
    clockSync_ = value;
  }

  const char* clockSyncText() {
    return SettingsText::boolToOnOff(clockSync());
  }

  // Mode
  int mode() {
    return mode_;
  }

  void setMode(int value) {
    mode_ = SettingsUtils::clip(0, NUM_MODES - 1, value);
  }

  const char* modeText() {
    return modeText(mode());
  }

  // Attack time
  float attackTime() {
    return decayTime_;
  }

  void setAttackTime(float value) {
    decayTime_ = SettingsUtils::clipFloat(value);
  }

  const char* attackTimeText() {
    if (clockSync()) {
      return MidiSync::tempoText(attackTime());
    } else {
      return SettingsText::phaseIncToMilis(attackInc());
    }
  }

  float attackInc() {
    if (clockSync()) {
      return MidiSync::readInc(attackTime());
    } else {
      return LookupTablesUtils::read(lut_phase_inc, attackTime());
    }
  }

  // Attack shape
  float attackShape() {
    return attackShape_;
  }

  void setAttackShape(float value) {
    attackShape_ = SettingsUtils::clipFloat(value);
  }

  const char* attackShapeText() {
    return SettingsText::floatToText(attackShape());
  }

  // Decay time
  float decayTime() {
    return decayTime_;
  }

  void setDecayTime(float value) {
    decayTime_ = SettingsUtils::clipFloat(value);
  }

  const char* decayTimeText() {
    if (clockSync()) {
      return MidiSync::tempoText(decayTime());
    } else {
      return SettingsText::phaseIncToMilis(decayInc());
    }
  }

  float decayInc() {
    if (clockSync()) {
      return MidiSync::readInc(decayTime());
    } else {
      return LookupTablesUtils::read(lut_phase_inc, decayTime());
    }
  }

  // Decay shape
  float decayShape() {
    return decayShape_;
  }

  void setDecayShape(float value) {
    decayShape_ = SettingsUtils::clipFloat(value);
  }

  const char* decayShapeText() {
    return SettingsText::floatToText(decayShape());
  }

  // Hold time
  float holdTime() {
    return holdTime_;
  }

  void setHoldTime(float value) {
    if (mode() == TRIGGER) {
      holdTime_ = SettingsUtils::clipFloat(value);
    }
  }

  const char* holdTimeText() {
    if (mode() == GATE) {
      return "-";
    } else if (clockSync()) {
      return MidiSync::tempoText(holdTime());
    } else {
      return SettingsText::phaseIncToMilis(holdInc());
    }
  }

  float holdInc() {
    if (clockSync()) {
      return MidiSync::readInc(holdTime());
    } else {
      return LookupTablesUtils::read(lut_phase_inc, holdTime());
    }
  }

  // Sustain level
  float sustainLevel() {
    return sustainLevel_;
  }

  void setSustainLevel(float value) {
    sustainLevel_ = SettingsUtils::clipFloat(value);
  }

  const char* sustainLevelText() {
    return SettingsText::floatToText(sustainLevel());
  }

  // Release time
  float releaseTime() {
    return releaseTime_;
  }

  void setReleaseTime(float value) {
    releaseTime_ = SettingsUtils::clipFloat(value);
  }

  const char* releaseTimeText() {
    if (clockSync()) {
      return MidiSync::tempoText(releaseTime());
    } else {
      return SettingsText::phaseIncToMilis(releaseInc());
    }
  }

  float releaseInc() {
    if (clockSync()) {
      return MidiSync::readInc(releaseTime());
    } else {
      return LookupTablesUtils::read(lut_phase_inc, releaseTime());
    }
  }

  // Release shape
  float releaseShape() {
    return releaseShape_;
  }

  void setReleaseShape(float value) {
    releaseShape_ = SettingsUtils::clipFloat(value);
  }

  const char* releaseShapeText() {
    return SettingsText::floatToText(releaseShape());
  }

  // Loop
  bool loop() {
    return loop_;
  }

  void setLoop(bool value) {
    loop_ = value;
  }

  const char* loopText() {
    return SettingsText::boolToOnOff(loop());
  }

  // Invert
  bool invert() {
    return invert_;
  }

  void setInvert(bool value) {
    invert_ = value;
  }

  const char* invertText() {
    return SettingsText::boolToOnOff(invert());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(clockSync_);
    fileWriter.write(mode_);
    fileWriter.write(decayTime_);
    fileWriter.write(attackShape_);
    fileWriter.write(attackTime_);
    fileWriter.write(decayShape_);
    fileWriter.write(holdTime_);
    fileWriter.write(sustainLevel_);
    fileWriter.write(releaseTime_);
    fileWriter.write(releaseShape_);
    fileWriter.write(loop_);
    fileWriter.write(invert_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(clockSync_);
    fileReader.read(mode_);
    fileReader.read(decayTime_);
    fileReader.read(attackShape_);
    fileReader.read(attackTime_);
    fileReader.read(decayShape_);
    fileReader.read(holdTime_);
    fileReader.read(sustainLevel_);
    fileReader.read(releaseTime_);
    fileReader.read(releaseShape_);
    fileReader.read(loop_);
    fileReader.read(invert_);
  }

  void paste(Envelope* envelope) {
    clockSync_ = envelope->clockSync();
    mode_ = envelope->mode();
    decayTime_ = envelope->attackTime();
    attackShape_ = envelope->attackShape();
    attackTime_ = envelope->decayTime();
    decayShape_ = envelope->decayShape();
    holdTime_ = envelope->holdTime();
    sustainLevel_ = envelope->sustainLevel();
    releaseTime_ = envelope->releaseTime();
    releaseShape_ = envelope->releaseShape();
    loop_ = envelope->loop();
    invert_ = envelope->invert();
  }

  void writeHash(Hash& hash) {
    hash.write(clockSync_);
    hash.write(mode_);
    hash.write(decayTime_);
    hash.write(attackShape_);
    hash.write(attackTime_);
    hash.write(decayShape_);
    hash.write(holdTime_);
    hash.write(sustainLevel_);
    hash.write(releaseTime_);
    hash.write(releaseShape_);
    hash.write(loop_);
    hash.write(invert_);
  }

 private:
  int mode_;
  bool loop_;
  bool invert_;
  bool clockSync_;
  float attackTime_;
  float attackShape_;
  float decayTime_;
  float decayShape_;
  float holdTime_;
  float sustainLevel_;
  float releaseTime_;
  float releaseShape_;
};

#endif
