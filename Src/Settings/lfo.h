#ifndef Lfo_h
#define Lfo_h

#include "fileReader.h"
#include "fileWriter.h"
#include "midiClock.h"
#include "settingsText.h"
#include "settingsUtils.h"
#include "lookupTablesUtils.h"
#include "rng.h"

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
    setSpeed(0.1f);
    setSkew(0.5f);
    setSkewSpread(0.f);
    setMin(0.f);
    setMax(1.f);
    setPhaseOffset(0.f);
    setPhaseOffsetSpread(0.f);
    setClockSync(false);
    setRandomMax(false);
    setRetrigger(true);
    setOneShot(false);
    setFold(0.f);
    setDelay(0.f);
  }

  // Type
  Type type() {
    return type_;
  }

  void setType(size_t value) {
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
      return MidiClock::tempoText(speed());
    } else {
      return SettingsText::phaseIncToHertz(LookupTablesUtils::read(lut_phase_inc, speed()));
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
    return SettingsText::floatToText((skew() * 2.f) - 1.f);
  }

    // Skew
  float skewSpread() {
    return skewSpread_;
  }

  void setSkewSpread(float value) {
    skewSpread_ = SettingsUtils::clipFloat(value);
  }

  const char* skewSpreadText() {
    return SettingsText::floatToText(skewSpread());
  }

  // Min
  float min() {
    return min_;
  }

  void setMin(float value) {
    min_ = SettingsUtils::clipFloat(value);
  }

  const char* minText() {
    return SettingsText::floatToText(min());
  }

  // Max
  float max() {
    return max_;
  }

  void setMax(float value) {
    max_ = SettingsUtils::clipFloat(value);
  }

  const char* maxText() {
    return SettingsText::floatToText(max());
  }

  // Phase offset
  float phaseOffset() {
    return phaseOffset_;
  }

  void setPhaseOffset(float value) {
    phaseOffset_ = SettingsUtils::clipFloat(value);
  }

  const char* phaseOffsetText() {
    return SettingsText::floatToText(phaseOffset());
  }

    // Phase offset spread
  float phaseOffsetSpread() {
    return phaseOffsetSpread_;
  }

  void setPhaseOffsetSpread(float value) {
    phaseOffsetSpread_ = SettingsUtils::clipFloat(value);
  }

  const char* phaseOffsetSpreadText() {
    return SettingsText::floatToText(phaseOffsetSpread());
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
  bool randomMax() {
    return randomMax_;
  }

  void setRandomMax(bool value) {
    randomMax_ = value;
  }

  const char* randomMaxText() {
    return SettingsText::boolToOnOff(randomMax());
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

  // Fold
  float fold() {
    return fold_;
  }

  void setFold(float value) {
    fold_ = SettingsUtils::clipFloat(value);
  }

  const char* foldText() {
    return SettingsText::floatToText(fold());
  }

  // Delay
  float delay() {
    return delay_;
  }

  void setDelay(float value) {
    delay_ = SettingsUtils::clipFloat(value);
  }

  const char* delayText() {
    if (clockSync()) {
      return MidiClock::tempoText(delay());
    } else {
      return SettingsText::phaseIncToMilis(LookupTablesUtils::read(lut_phase_inc, delay()));
    }
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(skew_);
    fileWriter.write(skewSpread_);
    fileWriter.write(speed_);
    fileWriter.write(min_);
    fileWriter.write(max_);
    fileWriter.write(phaseOffset_);
    fileWriter.write(phaseOffsetSpread_);
    fileWriter.write(randomMax_);
    fileWriter.write(clockSync_);
    fileWriter.write(retrigger_);
    fileWriter.write(oneShot_);
    fileWriter.write(type_);
    fileWriter.write(fold_);
    fileWriter.write(delay_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(skew_);
    fileReader.read(skewSpread_);
    fileReader.read(speed_);
    fileReader.read(min_);
    fileReader.read(max_);
    fileReader.read(phaseOffset_);
    fileReader.read(phaseOffsetSpread_);
    fileReader.read(randomMax_);
    fileReader.read(clockSync_);
    fileReader.read(retrigger_);
    fileReader.read(oneShot_);
    fileReader.read(type_);

    if (fileReader.version() >= 3) {
      fileReader.read(fold_);
      fileReader.read(delay_);
    }
  }

  void paste(Lfo* lfo) {
    skew_ = lfo->skew();
    skewSpread_ = lfo->skewSpread();
    speed_ = lfo->speed();
    min_ = lfo->min();
    max_ = lfo->max();
    phaseOffset_ = lfo->phaseOffset();
    phaseOffsetSpread_ = lfo->phaseOffsetSpread();
    randomMax_ = lfo->randomMax();
    clockSync_ = lfo->clockSync();
    retrigger_ = lfo->retrigger();
    oneShot_ = lfo->oneShot();
    type_ = lfo->type();
    fold_ = lfo->fold();
    delay_ = lfo->delay();
  }

  void writeHash(Hash& hash) {
    hash.write(type_);
    hash.write(skew_);
    hash.write(skewSpread_);
    hash.write(speed_);
    hash.write(min_);
    hash.write(max_);
    hash.write(phaseOffset_);
    hash.write(phaseOffsetSpread_);
    hash.write(randomMax_);
    hash.write(clockSync_);
    hash.write(retrigger_);
    hash.write(oneShot_);
    hash.write(fold_);
    hash.write(delay_);
  }

  void randomise() {
    skew_ = Rng::reciprocal();
    skewSpread_ = Rng::reciprocal();
    speed_ = Rng::reciprocal();
    min_ = Rng::reciprocal();
    max_ = Rng::reciprocal();
    phaseOffset_ = Rng::reciprocal();
    phaseOffsetSpread_ = Rng::reciprocal();
    randomMax_ = Rng::u16(0, 1);
    clockSync_ = Rng::u16(0, 1);
    retrigger_ = Rng::u16(0, 1);
    oneShot_ = Rng::u16(0, 1);
    type_ = Type(Rng::u16(0, NUM_TYPES - 1));
    fold_ = Rng::reciprocal();
    delay_ = Rng::reciprocal();
  }

 private:
  Type type_;
  float skew_;
  float skewSpread_;
  float speed_;
  float min_;
  float max_;
  float phaseOffset_;
  float phaseOffsetSpread_;
  float fold_;
  float delay_;
  bool randomMax_;
  bool clockSync_;
  bool retrigger_;
  bool oneShot_;
};

#endif
