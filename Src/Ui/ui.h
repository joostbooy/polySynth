#ifndef Ui_h
#define Ui_h

#include <stdint.h>

#include "adc.h"
#include "buttons.h"
#include "canvas.h"
#include "display.h"
#include "engine.h"
#include "leds.h"
#include "matrix.h"
#include "pages.h"
#include "pots.h"
#include "que.h"
#include "settings.h"
#include "switches.h"

class Ui {
 public:
  Pots& pots() {
    return pots_;
  }
  Leds& leds() {
    return leds_;
  }
  Buttons& buttons() {
    return buttons_;
  }
  Pages& pages() {
    return pages_;
  }
  Canvas& canvas() {
    return canvas_;
  }

  void init(Settings*, Engine*, Matrix*, Display*, Switches*, Adc*);
  void poll();
  void process();
  void sendDisplay();
  void resetAllPots();
  void unlockAllPots();
  void resetPot(int id);

  float readPotToSetting(int id) {
    return readPotToSetting(settings_->selectedPatch(), id);
  }

  float readPotToOrignalSetting(int id) {
    return readPotToSetting(settings_->selectedPatchOrignalState(), id);
  }

  bool potIsLocked(int id) {
    return lockedPots_ & (1 << id);
  }

 private:
  enum UnlockDirection {
    CW,
    CCW,
  };

  enum ControlType {
    BUTTON,
    ENCODER,
  };

  struct Event {
    ControlType type;
    uint8_t id;
    int8_t value;
  };

  Canvas canvas_;
  Pages pages_;
  Leds leds_;
  Buttons buttons_;
  Pots pots_;

  Engine* engine_;
  Settings* settings_;
  Matrix* matrix_;
  Display* display_;
  Switches* switches_;
  Adc* adc_;

  uint32_t lastInterval_ = 0;
  uint32_t displayInterval_ = 0;
  uint32_t lockedPots_;
  uint8_t encoderRaw_[4];
  uint8_t potUnlockDirection_[Pots::NUM_POTS];
  bool lastButtonState_[8 * 6];

  Que<Ui::Event, 16> uiQue;
  void addEvent(ControlType, uint8_t, int8_t);
  void processSwitches();
  void processPots();
  void processLeds();
  void processDisplay();
  void writePotToSetting(int);
  float readPotToSetting(Patch&, int);

  void unlockPot(int id) {
    lockedPots_ &= ~(1 << id);
  }
};

#endif
