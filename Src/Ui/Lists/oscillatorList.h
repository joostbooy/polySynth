#ifndef OscillatorList_h
#define AmpLOscillatorList_hist_h

#include "settings.h"
#include "settingsText.h"

class OscillatorList : public SettingsList {
 public:
  enum Item {
    TRACK_NOTE_1,
    TRACK_NOTE_2,
    SYNC,
    FM,
    MUTE_1,
    MUTE_2,
    SHAPE_1,
    SHAPE_2,
    MOD_DEPTH,
    MOD_SOURCE,
    SLIDE_1,
    SLIDE_2,
    LINK_SLIDE_AMMOUNT,
    SLIDE_AMMOUNT_1,
    SLIDE_AMMOUNT_2,
    OCTAVE_OFFSET_1,
    OCTAVE_OFFSET_2,
    TUNE_1,
    TUNE_2,
    TYPE_1,
    TYPE_2,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case TRACK_NOTE_1:
        return "TRACK NOTE 1";
      case TRACK_NOTE_2:
        return "TRACK NOTE 2";
      case SYNC:
        return "SYNC";
      case FM:
        return "FM";
      case MUTE_1:
        return "MUTE 1";
      case MUTE_2:
        return "MUTE 2";
      case SHAPE_1:
        return "SHAPE 1";
      case SHAPE_2:
        return "SHAPE 2";
      case MOD_DEPTH:
        return "MOD DEPTH";
      case MOD_SOURCE:
        return "MOD SOUCRE";
      case SLIDE_1:
        return "SLIDE 1";
      case SLIDE_2:
        return "SLIDE 2";
      case LINK_SLIDE_AMMOUNT:
        return "LINK SLIDE AMMOUNT";
      case SLIDE_AMMOUNT_1:
        return "SLIDE AMMOUNT 1";
      case SLIDE_AMMOUNT_2:
        return "SLIDE AMMOUNT 2";
      case OCTAVE_OFFSET_1:
        return "OCTAVE OFFSET 1";
      case OCTAVE_OFFSET_2:
        return "OCTAVE OFFSET 2";
      case TUNE_1:
        return "TUNE 1";
      case TUNE_2:
        return "TUNE 2";
      case TYPE_1:
        return "TYPE 1";
      case TYPE_2:
        return "TYPE 2";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Oscillator& oscillator = settings_->oscillator();

    switch (item) {
      case TRACK_NOTE_1:
        return oscillator.trackNote1Text();
      case TRACK_NOTE_2:
        return oscillator.trackNote2Text();
      case SYNC:
        return oscillator.syncEnableText();
      case FM:
        return oscillator.fmEnableText();
      case MUTE_1:
        return oscillator.muteOsc1Text();
      case MUTE_2:
        return oscillator.muteOsc2Text();
      case SHAPE_1:
        return oscillator.shape1Text();
      case SHAPE_2:
        return oscillator.shape2Text();
      case MOD_DEPTH:
        return oscillator.modDepthText();
      case MOD_SOURCE:
        return oscillator.modSourceText();
      case SLIDE_1:
        return oscillator.slideEnable1Text();
      case SLIDE_2:
        return oscillator.slideEnable2Text();
      case LINK_SLIDE_AMMOUNT:
        return oscillator.linkSlideAmmountText();
      case SLIDE_AMMOUNT_1:
        return oscillator.slideAmmount1Text();
      case SLIDE_AMMOUNT_2:
        return oscillator.slideAmmount2Text();
      case OCTAVE_OFFSET_1:
        return oscillator.octaveOffset1Text();
      case OCTAVE_OFFSET_2:
        return oscillator.octaveOffset2Text();
      case TUNE_1:
        return oscillator.tune1Text();
      case TUNE_2:
        return oscillator.tune2Text();
      case TYPE_1:
        return oscillator.type1Text();
      case TYPE_2:
        return oscillator.type2Text();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Oscillator& oscillator = settings_->oscillator();

    switch (item) {
      case TRACK_NOTE_1:
        return oscillator.setTrackNote1(inc > 0);
      case TRACK_NOTE_2:
        return oscillator.setTrackNote2(inc > 0);
      case SYNC:
        return oscillator.setSyncEnable(inc > 0);
      case FM:
        return oscillator.setFmEnable(inc > 0);
      case MUTE_1:
        return oscillator.setMuteOsc1(inc > 0);
      case MUTE_2:
        return oscillator.setMuteOsc2(inc > 0);
      case SHAPE_1:
        return oscillator.setShape1(oscillator.shape1() + SettingsUtils::fInc(inc, shifted));
      case SHAPE_2:
        return oscillator.setShape2(oscillator.shape2() + SettingsUtils::fInc(inc, shifted));
      case MOD_DEPTH:
        return oscillator.setModDepth(oscillator.modDepth() + SettingsUtils::fInc(inc, shifted));
      case MOD_SOURCE:
        return oscillator.setModSource(oscillator.modSource() + inc);
      case SLIDE_1:
        return oscillator.setSlideEnable1(inc > 0);
      case SLIDE_2:
        return oscillator.setSlideEnable2(inc > 0);
      case LINK_SLIDE_AMMOUNT:
        return oscillator.setLinkSlideAmmount(inc > 0);
      case SLIDE_AMMOUNT_1:
        return oscillator.setSlideAmmount1(oscillator.slideAmmount1() + SettingsUtils::fInc(inc, shifted));
      case SLIDE_AMMOUNT_2:
        return oscillator.setSlideAmmount2(oscillator.slideAmmount2() + SettingsUtils::fInc(inc, shifted));
      case OCTAVE_OFFSET_1:
        return oscillator.setOctaveOffset1(oscillator.octaveOffset1() + (inc * 12));
      case OCTAVE_OFFSET_2:
        return oscillator.setOctaveOffset2(oscillator.octaveOffset2() + (inc * 12));
      case TUNE_1:
        return oscillator.setTune1(oscillator.tune1() + SettingsUtils::fInc(inc, shifted));
      case TUNE_2:
        return oscillator.setTune2(oscillator.tune2() + SettingsUtils::fInc(inc, shifted));
      case TYPE_1:
        return oscillator.setType1(oscillator.type1() + inc);
      case TYPE_2:
        return oscillator.setType2(oscillator.type2() + inc);
      default:
        break;
    }
  }

 private:
};

#endif
