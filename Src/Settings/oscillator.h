#if !defined(Oscillator_h)
#define Oscillator_h

#include "gpio.h"

class Oscillator {
 public:
  void init() {
    //
  }

  bool fmEnable() {
    return fmEnable_;
  }

  bool amEnable() {
    return amEnable_;
  }

  bool muteOsc1() {
    return muteOsc1_;
  }

  bool muteOsc2() {
    return muteOsc2_;
  }

  Gpio::OscType selectedOsc1() {
    return selectedOsc1_;
  }

  Gpio::OscType selectedOsc2() {
    return selectedOsc2_;
  }

  float shape1() {
    return shape1_;
  }

  float shape2() {
    return shape2_;
  }

 private:
  bool fmEnable_;
  bool amEnable_;
  bool muteOsc1_;
  bool muteOsc2_;
  float shape1_;
  float shape2_;
  Gpio::OscType selectedOsc1_;
  Gpio::OscType selectedOsc2_;
};

#endif  // Oscilator_h
