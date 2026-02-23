#ifndef Lfo_h
#define Lfo_h

#include "fileReader.h"
#include "fileWriter.h"
#include "midi.h"
#include "midiEngine.h"
#include "midiSync.h"
#include "settingsText.h"
#include "settingsUtils.h"

class Lfo {
 public:
  enum Type {
    LINEAR,
    LOG_EXP,
    EXP_LOG,
    SQUARE,

    NUM_TYPES
  };

  const char* typeText(Type type) {
    switch (type) {
      case LINEAR:  return "LINEAR";
      case LOG_EXP: return "LOG EXP";
      case EXP_LOG: return "EXP LOG";
      case SQUARE:  return "SQUARE";
      default:
        break;
    }
    return nullptr;
  }

  void init() {
    setType(LINEAR);
    setSpeed(0);
    setSkew(0.5f);
    setMin(0.0f);
    setMax(1.0f);
    setSyncPhase(0.0f);
    setClockSync(false);
    setRandomise(false);
    setRetrigger(true);
    setOneShot(false);
  }

  // Type
  Type type() {
    return type_;
  }

  void setType(int value) {
    type_ = Type(value % NUM_TYPES);
  }

  const char* typeText() {
    return typeText(type());
  }

  // speed
  float speed() {
    return speed_;
  }

  void setSpeed(float value) {
    speed_ = SettingsUtils::clipFloat(value);
  }

  const char* speedText() {
    if (clockSync()) {
      return MidiSync::tempoText(speed() * (MidiSync::NUM_TEMPOS - 1));
    } else {
      size_t index = speed() * (PHASE_TABLE_SIZE - 1);
      return SettingsText::phaseIncToHertz(lut_phase_inc[index]);
    }
  }
  
  float inc() {
    if (clockSync()) {
      return MidiSync::readInc(speed() * (MidiSync::NUM_TEMPOS - 1));
    } else {
      return lut_phase_inc[int(speed() * (PHASE_TABLE_SIZE - 1))];
    }
  }

  // Skew
  float skew() {
    return skew_;
  }

  void setSkew(float value) {
    skew_ = SettingsUtils::clipFloat(value);
  }

  const char* skewText() {
    return SettingsText::floatToText(skew(), -100, 100);
  }

  // Min
  float min() {
    return min_;
  }

  void setMin(float value) {
    min_ = SettingsUtils::clipFloat(value);
  }

  const char* minText() {
    return SettingsText::floatToText(min(), 0, 100);
  }

  // Max
  float max() {
    return max_;
  }

  void setMax(float value) {
    max_ = SettingsUtils::clipFloat(value);
  }

  const char* maxText() {
    return SettingsText::floatToText(max(), 0, 100);
  }

  // Sync phase
  float syncPhase() {
    return syncPhase_;
  }

  void setSyncPhase(float value) {
    syncPhase_ = SettingsUtils::clipFloat(value);
  }

  const char* syncPhaseText() {
    return SettingsText::percentageToText(syncPhase() * 100, 100);
  }

  // clock sync
  bool clockSync() {
    return clockSync_;
  }

  void setClockSync(bool value) {
    clockSync_ = value;
  }

  const char* clockSyncText() {
    return SettingsText::boolToOnOff(clockSync());
  }

  // Randomise
  bool randomise() {
    return randomise_;
  }

  void setRandomise(bool value) {
    randomise_ = value;
  }

  const char* randomiseText() {
    return SettingsText::boolToOnOff(randomise());
  }

  // Retrigger
  bool retrigger() {
    return retrigger_;
  }

  void setRetrigger(bool value) {
    retrigger_ = value;
  }

  const char* retriggerText() {
    return SettingsText::boolToOnOff(retrigger());
  }

  // Retrigger
  bool oneShot() {
    return oneShot_;
  }

  void setOneShot(bool value) {
    oneShot_ = value;
  }

  const char* oneShotText() {
    return SettingsText::boolToOnOff(oneShot());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(skew_);
    fileWriter.write(speed_);
    fileWriter.write(min_);
    fileWriter.write(max_);
    fileWriter.write(syncPhase_);
    fileWriter.write(randomise_);
    fileWriter.write(clockSync_);
    fileWriter.write(retrigger_);
    fileWriter.write(oneShot_);
    fileWriter.write(type_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(skew_);
    fileReader.read(speed_);
    fileReader.read(min_);
    fileReader.read(max_);
    fileReader.read(syncPhase_);
    fileReader.read(randomise_);
    fileReader.read(clockSync_);
    fileReader.read(retrigger_);
    fileReader.read(oneShot_);
    fileReader.read(type_);
  }

  void paste(Lfo* lfo) {
    skew_ = lfo->skew();
    speed_ = lfo->speed();
    min_ = lfo->min();
    max_ = lfo->max();
    syncPhase_ = lfo->syncPhase();
    randomise_ = lfo->randomise();
    clockSync_ = lfo->clockSync();
    retrigger_ = lfo->retrigger();
    oneShot_ = lfo->oneShot();
    type_ = lfo->type();
  }

  void writeHash(Hash& hash) {
    hash.write(type_);
    hash.write(skew_);
    hash.write(speed_);
    hash.write(min_);
    hash.write(max_);
    hash.write(syncPhase_);
    hash.write(randomise_);
    hash.write(clockSync_);
    hash.write(retrigger_);
    hash.write(oneShot_);
  }

 private:
  Type type_;
  float skew_;
  float speed_;
  float min_;
  float max_;
  float syncPhase_;
  bool randomise_;
  bool clockSync_;
  bool retrigger_;
  bool oneShot_;
};

#endif
