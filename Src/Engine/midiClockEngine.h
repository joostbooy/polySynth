#ifndef MidiClockEngine_h
#define MidiClockEngine_h

#include "lookupTables.h"
#include "midiClock.h"

class MidiClockEngine {
 public:
  void init(MidiClock* midiClock) {
    midiClock_ = midiClock;
    bpm_ = 120;
    extBpm_ = 120;
    reset();
  }

  void reset() {
    ticks = 0;
    isrTicks_ = 0;
    isrAverage_ = 0;
    tempoPhase_ = 0;
  }

  static uint16_t bpm() {
    return bpm_;
  }

  static uint16_t bpmFractional() {
    return bpmFractional_;
  }

  bool tick() {
    if (midiClock_->clockSource() == MidiClock::INTERNAL) {
      bpm_ = midiClock_->bpm();
      bpmFractional_ = midiClock_->bpmFractional();
    } else {
      bpm_ = extBpm_;
      bpmFractional_ = 0;
    }

    ++isrTicks_;
    tempoPhase_ += (lut_bpm_inc[bpm_ - MIN_BPM] + lut_bpm_fractional_inc[bpmFractional_]);
    return tempoPhase_ < tempoInc_;
  }

  void syncBpm() {
    isrAverage_ += isrTicks_;

    if (((++numReadings_ % 4) == 0) && isrAverage_ > 0) {
      extBpm_ = uint32_t(45000000UL * 60UL / 24UL / (kUpdatePeriod * (isrAverage_ / 4)));
      extBpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, extBpm_);
      isrAverage_ = 0;
    }

    isrTicks_ = 0;
  }

  static float readInc(float value) {
    return readInc(int(value * (MidiClock::NUM_TEMPOS - 1)));
  }

 private:
  MidiClock* midiClock_;
  uint8_t syncPort_;
  static inline uint16_t bpm_;
  static inline uint16_t bpmFractional_;

  volatile uint16_t extBpm_;
  volatile uint32_t tempoInc_, tempoPhase_, ticks, isrTicks_, isrAverage_;
  volatile uint8_t numReadings_ = 0;
  static const uint32_t kUpdatePeriod = 45000000UL / UPDATE_FREQ;

  static float readInc(int value) {
    return lut_beat_length_inc[bpm_ - MIN_BPM] * incMultiplier(value);
  }

  static float incMultiplier(int value) {
    switch (value) {
      case MidiClock::_1_1_DOTTED:    return 0.25f / 1.5f;
      case MidiClock::_1_1_WHOLE:     return 0.25f;
      case MidiClock::_1_1_TRIPLET:   return 0.375f;
      case MidiClock::_1_2_DOTTED:    return 0.5f / 1.5f;
      case MidiClock::_1_2_WHOLE:     return 0.5f;
      case MidiClock::_1_2_TRIPLET:   return 0.75f;
      case MidiClock::_1_4_DOTTED:    return 1.f / 1.5f;
      case MidiClock::_1_4_WHOLE:     return 1.f;
      case MidiClock::_1_4_TRIPLET:   return 1.5f;
      case MidiClock::_1_8_DOTTED:    return 2.f / 1.5f;
      case MidiClock::_1_8_WHOLE:     return 2.f;
      case MidiClock::_1_8_TRIPLET:   return 3.f;
      case MidiClock::_1_16_DOTTED:   return 4.f / 1.5f;
      case MidiClock::_1_16_WHOLE:    return 4.f;
      case MidiClock::_1_16_TRIPLET:  return 6.f;
      case MidiClock::_1_32_DOTTED:   return 8.f / 1.5f;
      case MidiClock::_1_32_WHOLE:    return 8.f;
      case MidiClock::_1_32_TRIPLET:  return 12.f;
      case MidiClock::_1_64_DOTTED:   return 16.f / 1.5f;
      case MidiClock::_1_64_WHOLE:    return 16.f;
      case MidiClock::_1_64_TRIPLET:  return 24.f;
      default:
        break;
    }
    return 0.f;
  }
};

#endif
