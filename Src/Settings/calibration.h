#ifndef Calibration_h
#define Calibration_h

#include "SettingsUtils.h"
#include "dsp.h"
#include "fileReader.h"
#include "fileWriter.h"

class Calibration {
 public:
  void init() {
    min_ = 0;
    max_ = 65535;
    enabled_ = false;
  }

  void start() {
    enabled_ = true;
  }

  void stop() {
    enabled_ = false;
  }

  bool enabled() {
    return enabled_;
  }

  // Min
  int min() {
    return min_;
  }

  // should result in -5V
  void setMin(int value) {
    min_ = SettingsUtils::clip(0, max_, value);
  }

  // Max
  int max() {
    return max_;
  }

  // should result in +5V
  void setMax(int value) {
    max_ = SettingsUtils::clip(min_, 65535, value);
  }

  uint16_t read(uint16_t value) {
    float x = (1.f / 65535.f) * value;
    return Dsp::cross_fade(max_, min_, x);
  }

  uint16_t noteToValue(int note) {
    float x = (1.f / kMaxNotes) * SettingsUtils::clip(0, kMaxNotes - 1, note);
    return Dsp::cross_fade(max_, min_, x);
  }

  uint16_t semiNoteValue() {
    return noteToValue(0) - noteToValue(1);
  }

  // storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(min_);
    fileWriter.write(max_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(min_);
    fileReader.read(max_);
  }

 private:
  bool enabled_;
  static const int kMaxVolts = 10;  // 5V p.p
  static const int kMaxNotes = kMaxVolts * 12;
  uint16_t min_ = 0;
  uint16_t max_ = 65535;
};

#endif
