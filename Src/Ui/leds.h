#ifndef Leds_h
#define Leds_h

#include "pots.h"
#include "modMatrix.h"

class Leds {
 public:
  enum Color { BLACK, RED };

  void init() {
    setAll(BLACK);
  }

  uint16_t* data() {
    return &data_[0];
  }

  void setAll(Color color) {
    uint16_t value = color == RED ? 0xFFFF : 0x0000;
    for (size_t i = 0; i < kNumOfCollumns; ++i) {
      data_[i] = value;
    }
  }

  // Amp
  void setAm(bool state) {
    set(7, 1, state);
  }

  // Vco mod
  void setOscModSource(int source) {
    switch (source) {
      case 0:
        set(4, 4, RED);
        set(5, 3, BLACK);
        break;
      case 1:
        set(4, 4, BLACK);
        set(5, 3, RED);
        break;
      default:
        break;
    }
  }

  // Slide
  void setSlideVco1(bool state) {
    set(6, 3, state);
  }

   void setSlideVco2(bool state) {
    set(7, 4, state);
  }

  // Pots
  void setPot(int id, Color color) {
    switch (id) {
      case Pots::RESONANCE_B:
        set(2, 4, color);
        break;
      case Pots::RESONANCE_A:
        set(0, 5, color);
        break;
      case Pots::TUNE_A:
        set(0, 14, color);
        break;
      case Pots::TUNE_B:
        set(2, 14, color);
        break;
      case Pots::SHAPE_A:
        set(0, 9, color);
        break;
      case Pots::SHAPE_B:
        set(2, 9, color);
        break;
      case Pots::CUTOFF_A:
        set(0, 7, color);
        break;
      case Pots::CUTOFF_B:
        set(2, 6, color);
        break;
      case Pots::PAN:
        set(6, 1, color);
        break;
      case Pots::MOD_DEPTH:
        set(4, 3, color);
        break;
      case Pots::DRIVE:
        set(5, 1, color);
        break;
      case Pots::SLIDE_AMMOUNT:
        set(6, 4, color);
        break;
      case Pots::LFO_SKEW_1:
        set(4, 12, color);
        break;
      case Pots::LFO_SPEED_2:
        set(6, 13, color);
        break;
      case Pots::LFO_SKEW_2:
        set(6, 12, color);
        break;
      case Pots::LFO_SPEED_1:
        set(4, 13, color);
        break;
      case Pots::A_TIME_1:
        set(5, 11, color);
        break;
      case Pots::A_SHAPE_1:
        set(7, 11, color);
        break;
      case Pots::D_TIME_1:
        set(5, 10, color);
        break;
      case Pots::D_SHAPE_1:
        set(7, 10, color);
        break;
      case Pots::S_LEVEL_1:
        set(5, 9, color);
        break;
      case Pots::S_HOLD_1:
        set(7, 9, color);
        break;
      case Pots::R_TIME_1:
        set(5, 8, color);
        break;
      case Pots::R_SHAPE_1:
        set(7, 8, color);
        break;
      case Pots::A_TIME_2:
        set(5, 7, color);
        break;
      case Pots::A_SHAPE_2:
        set(7, 7, color);
        break;
      case Pots::D_TIME_2:
        set(5, 6, color);
        break;
      case Pots::D_SHAPE_2:
        set(7, 6, color);
        break;
      case Pots::S_LEVEL_2:
        set(5, 5, color);
        break;
      case Pots::S_HOLD_2:
        set(7, 5, color);
        break;
      case Pots::R_TIME_2:
        set(5, 8, color);
        break;
      case Pots::R_SHAPE_2:
        set(7, 4, color);
        break;
      default:
        break;
    }
  }

  // Filter
  void setFilter1Fm(bool state) {
    set(1, 5, state);
  }

  void setFilter2Fm(bool state) {
    set(3, 4, state);
  }

  void setFilterType(Filter::Type type) {
    switch (type) {
      case Filter::HP:
        set(0, 0, RED);
        set(1, 0, BLACK);
        set(2, 0, BLACK);
        break;
      case Filter::BP:
        set(0, 0, BLACK);
        set(1, 0, RED);
        set(2, 0, BLACK);
        break;
      case Filter::LP2P:
        set(0, 0, BLACK);
        set(1, 0, BLACK);
        set(2, 0, RED);
        break;
      default:
        break;
    }
  }

  void setFilterRouting(Filter::Routing routing) {
    switch (routing) {
      case Filter::SERIES:
        set(3, 1, RED);
        set(4, 1, BLACK);
        break;
      case Filter::PARALEL:
        set(3, 1, BLACK);
        set(4, 1, RED);
        break;
      default:
        break;
    }
  }

  // Osc 1
  void setOsc1Fm(bool state) {
    set(1, 7, state);
  }

  void setMute1(bool state) {
    set(1, 8, state);
  }

  void setOctaveOffset1(int offset) {
    switch (offset) {
      case -24:
        set(1, 15, RED);
        set(0, 15, BLACK);
        set(0, 13, BLACK);
        set(1, 13, BLACK);
        break;
      case -12:
        set(1, 15, BLACK);
        set(0, 15, RED);
        set(0, 13, BLACK);
        set(1, 13, BLACK);
        break;
      case 0:
        set(1, 15, BLACK);
        set(0, 15, BLACK);
        set(0, 13, BLACK);
        set(1, 13, BLACK);
        break;
      case 12:
        set(1, 15, BLACK);
        set(0, 15, BLACK);
        set(0, 13, RED);
        set(1, 13, BLACK);
        break;
      case 24:
        set(1, 15, BLACK);
        set(0, 15, BLACK);
        set(0, 13, BLACK);
        set(1, 13, RED);
        break;
      default:
        break;
    }
  }

  void setOsc1Type(Oscillator::Type1 type) {
    switch (type) {
      case Oscillator::SAW1:
        set(0, 10, RED);
        set(0, 11, BLACK);
        set(1, 11, BLACK);
        set(1, 10, BLACK);
        break;
      case Oscillator::TRIANGLE1:
        set(0, 10, BLACK);
        set(0, 11, RED);
        set(1, 11, BLACK);
        set(1, 10, BLACK);
        break;
      case Oscillator::SINE1:
        set(0, 10, BLACK);
        set(0, 11, BLACK);
        set(1, 11, RED);
        set(1, 10, BLACK);
        break;
      case Oscillator::SQUARE1:
        set(0, 10, BLACK);
        set(0, 11, BLACK);
        set(1, 11, BLACK);
        set(1, 10, RED);
        break;
      default:
        break;
    }
  }

  // Osc 2
  void setMute2(bool state) {
    set(2, 7, state);
  }

  void setOctaveOffset2(int offset) {
    switch (offset) {
      case -24:
        set(3, 15, RED);
        set(2, 15, BLACK);
        set(2, 13, BLACK);
        set(3, 13, BLACK);
        break;
      case -12:
        set(3, 15, BLACK);
        set(2, 15, RED);
        set(2, 13, BLACK);
        set(3, 13, BLACK);
        break;
      case 0:
        set(3, 15, BLACK);
        set(2, 15, BLACK);
        set(2, 13, BLACK);
        set(3, 13, BLACK);
        break;
      case 12:
        set(3, 15, BLACK);
        set(2, 15, BLACK);
        set(2, 13, RED);
        set(3, 13, BLACK);
        break;
      case 24:
        set(3, 15, BLACK);
        set(2, 15, BLACK);
        set(2, 13, BLACK);
        set(3, 13, RED);
        break;
      default:
        break;
    }
  }

  void setOsc2Sync(bool state) {
    set(1, 14, state);
  }

  void setOsc2Type(Oscillator::Type2 type) {
    switch (type) {
      case Oscillator::SAW2:
        set(2, 10, RED);
        set(2, 11, BLACK);
        set(3, 11, BLACK);
        set(3, 10, BLACK);
        break;
      case Oscillator::TRIANGLE2:
        set(2, 10, BLACK);
        set(2, 11, RED);
        set(3, 11, BLACK);
        set(3, 10, BLACK);
        break;
      case Oscillator::NOISE2:
        set(2, 10, BLACK);
        set(2, 11, BLACK);
        set(3, 11, RED);
        set(3, 10, BLACK);
        break;
      case Oscillator::SQUARE2:
        set(2, 10, BLACK);
        set(2, 11, BLACK);
        set(3, 11, BLACK);
        set(3, 10, RED);
        break;
      default:
        break;
    }
  }

  // Lfo 1
  void setLfo1Tempo(bool state) {
    set(5, 13, state);
  }

  void setLfo1Type(Lfo::Type type) {
    switch (type) {
      case Lfo::LINEAR:
        set(4, 15, RED);
        set(4, 14, BLACK);
        set(5, 14, BLACK);
        set(5, 15, BLACK);
        break;
      case Lfo::LOG_EXP:
        set(4, 15, BLACK);
        set(4, 14, RED);
        set(5, 14, BLACK);
        set(5, 15, BLACK);
        break;
      case Lfo::EXP_LOG:
        set(4, 15, BLACK);
        set(4, 14, BLACK);
        set(5, 14, RED);
        set(5, 15, BLACK);
        break;
      case Lfo::SQUARE:
        set(4, 15, BLACK);
        set(4, 14, BLACK);
        set(5, 14, BLACK);
        set(5, 15, RED);
        break;
      default:
        break;
    }
  }

  // Lfo 2
  void setLfo2Tempo(bool state) {
    set(7, 13, state);
  }

  void setLfo2Type(Lfo::Type type) {
    switch (type) {
      case Lfo::LINEAR:
        set(6, 15, RED);
        set(6, 14, BLACK);
        set(7, 14, BLACK);
        set(7, 15, BLACK);
        break;
      case Lfo::LOG_EXP:
        set(6, 15, BLACK);
        set(6, 14, RED);
        set(7, 14, BLACK);
        set(7, 15, BLACK);
        break;
      case Lfo::EXP_LOG:
        set(6, 15, BLACK);
        set(6, 14, BLACK);
        set(7, 14, RED);
        set(7, 15, BLACK);
        break;
      case Lfo::SQUARE:
        set(6, 15, BLACK);
        set(6, 14, BLACK);
        set(7, 14, BLACK);
        set(7, 15, RED);
        break;
      default:
        break;
    }
  }

  // Mod matrix shortcuts
  void setModMatrix(ModMatrix* matrix) {
    set(0, 12, matrix->read(ModMatrix::LFO_1, ModMatrix::TUNE_1));
    set(1, 12, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::TUNE_1));
    set(2, 12, matrix->read(ModMatrix::LFO_1, ModMatrix::TUNE_2));
    set(3, 12, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::TUNE_2));
    set(0, 8, matrix->read(ModMatrix::LFO_1, ModMatrix::SHAPE_1));
    set(1, 9, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::SHAPE_1));
    set(2, 8, matrix->read(ModMatrix::LFO_1, ModMatrix::SHAPE_2));
    set(3, 8, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::SHAPE_2));
    set(0, 6, matrix->read(ModMatrix::LFO_1, ModMatrix::CUTOFF_1));
    set(1, 6, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::CUTOFF_1));
    set(2, 5, matrix->read(ModMatrix::LFO_1, ModMatrix::CUTOFF_2));
    set(3, 5, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::CUTOFF_2));
    set(4, 2, matrix->read(ModMatrix::LFO_1, ModMatrix::VCO_MOD_DEPTH));
    set(5, 2, matrix->read(ModMatrix::ENVELOPE_2, ModMatrix::VCO_MOD_DEPTH));
    set(6, 2, matrix->read(ModMatrix::LFO_1, ModMatrix::GAIN));
    set(7, 2, matrix->read(ModMatrix::ENVELOPE_1, ModMatrix::GAIN));
    set(6, 0, matrix->read(ModMatrix::LFO_1, ModMatrix::PAN));
    set(7, 0, matrix->read(ModMatrix::ENVELOPE_1, ModMatrix::PAN));
  }

 private:
  static const size_t kNumOfCollumns = 8;
  uint16_t data_[kNumOfCollumns];

  void set(int x, int y, Color color) {
    uint16_t value = data_[x] & ~(1 << y);
    data_[x] = value | (color << y);
  }

  void set(int x, int y, bool state) {
    set(x, y, state ? RED : BLACK);
  }
};

#endif
