#ifndef SettingsText_h
#define SettingsText_h

#include "dsp.h"
#include "font.h"
#include "lookupTables.h"
#include "stringBuilder.h"

class SettingsText {
 public:
  static inline StringBuilderBase<31> str;

  static const char* intToText(int value) {
    return str.write(value);
  }

  static const char* boolToOnOff(bool state) {
    return state ? "ON" : "OFF";
  }

  static const char* boolToCheckmark(bool state) {
    return state ? Font::glyph(Font::CHECKMARK) : " ";
  }

  static const char* percentageToText(uint32_t ammount, uint32_t total) {
    float percentage = (100.f / total) * ammount;
    return str.write(static_cast<int>(percentage), "%");
  }

  static const char* floatToText(float value, const char* unit = nullptr) {
    uint32_t intergral = value;
    uint32_t fractional = (value - intergral) * 100.f;
    return str.write(intergral, ".", fractional, " ", unit);
  }

  static const char* floatToText(float value, int min, int max) {
    return str.write(Dsp::cross_fade(min, max, value));
  }

  static const char* noteToText(uint8_t note) {
    uint8_t oct = 0;
    while (note >= 12) {
      note -= 12;
      ++oct;
    }
    return str.write(note_text_[note], oct);
  }

  static const char* midiChannelText(int channel) {
    if (channel < 0) {
      return "NONE";
    } else if (channel >= 16) {
      return "OMNI";
    }

    return str.write(channel + 1);
  }

  static const char* kbToMemSizeText(uint32_t kb) {
    uint32_t gb = kb / 1000000;
    uint32_t mb = kb / 1000;

    if (gb > 0) {
      uint32_t gb_frac = (kb - (gb * 1000000)) / 10000;
      return str.write(gb, ".", gb_frac, " GB");
    } else if (mb > 0) {
      uint32_t mb_frac = (kb - (mb * 1000)) / 10;
      return str.write(mb, ".", mb_frac, " MB");
    } else {
      return str.write(kb, " KB");
    }
  }

 private:
  static inline const char* note_text_[12] = {"C", "Cb", "D", "Db", "E", "F", "Fb", "G", "Gb", "A", "Ab", "B"};
};

#endif
