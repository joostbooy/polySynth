#if !defined(Oscillator_h)
#define Oscillator_h

class Oscillator {
 public:
   enum Type { SAW, TRIANGLE, SQUARE, SINE, NOISE };

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

  Type type1() {
    return selectedOsc1_;
  }

int type1SwitchValue() {
  return 0;
}

  Type type2() {
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
  Type selectedOsc1_;
  Type selectedOsc2_;
};

#endif  // Oscilator_h
