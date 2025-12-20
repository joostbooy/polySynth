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
  void init() {
    setSpeed(0);
    setShape(0.5f);
    setSkew(0.5f);
    setMin(0.0f);
    setMax(1.0f);
    setSyncPhase(0.0f);
    setClockSync(false);
    setRandomise(false);
    setRetrigger(true);
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
      return MidiSync::tempo_text(speed() * (MidiSync::NUM_TEMPOS - 1));
    } else {
      size_t index = speed() * PHASE_TABLE_SIZE;
      return SettingsText::floatToText(lut_phase_length[index], "HZ");
    }
  }

  float inc() {
    if (clockSync()) {
      return MidiSync::read_inc(speed() * (MidiSync::NUM_TEMPOS - 1));
    } else {
      return lut_phase_inc[int(speed() * (PHASE_TABLE_SIZE - 1))];
    }
  }

  // shape
  float shape() {
    return shape_;
  }

  void setShape(float value) {
    shape_ = SettingsUtils::clipFloat(value);
  }

  const char* shapeText() {
    return SettingsText::floatToText(shape(), -100, 100);
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
    return sync_phase_;
  }

  void setSyncPhase(float value) {
    sync_phase_ = SettingsUtils::clipFloat(value);
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

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(skew_);
    fileWriter.write(shape_);
    fileWriter.write(speed_);
    fileWriter.write(min_);
    fileWriter.write(max_);
    fileWriter.write(sync_phase_);
    fileWriter.write(randomise_);
    fileWriter.write(clockSync_);
    fileWriter.write(retrigger_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(skew_);
    fileReader.read(shape_);
    fileReader.read(speed_);
    fileReader.read(min_);
    fileReader.read(max_);
    fileReader.read(sync_phase_);
    fileReader.read(randomise_);
    fileReader.read(clockSync_);
    fileReader.read(retrigger_);
  }

  void paste(Lfo* lfo) {
    skew_ = lfo->skew();
    shape_ = lfo->shape();
    speed_ = lfo->speed();
    min_ = lfo->min();
    max_ = lfo->max();
    sync_phase_ = lfo->syncPhase();
    randomise_ = lfo->randomise();
    clockSync_ = lfo->clockSync();
    retrigger_ = lfo->retrigger();
  }

 private:
  float skew_;
  float shape_;
  float speed_;
  float min_;
  float max_;
  float sync_phase_;
  bool randomise_;
  bool clockSync_;
  bool retrigger_;
};

#endif
