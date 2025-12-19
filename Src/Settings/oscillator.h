#if !defined(Oscillator_h)
#define Oscillator_h

class Oscillator {
 public:
  enum Type { SAW, TRIANGLE, SQUARE, SINE, NOISE };

  void init() {
    //
  }

  Type selectedOsc1() {
    return selectedOsc1_;
  }

  Type selectedOsc2() {
    return selectedOsc2_;
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

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(fmEnable_);
    fileWriter.write(amEnable_);
    fileWriter.write(muteOsc1_);
    fileWriter.write(muteOsc2_);
    fileWriter.write(shape1_);
    fileWriter.write(shape2_);
    fileWriter.write(selectedOsc1_);
    fileWriter.write(selectedOsc2_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(fmEnable_);
    fileReader.read(amEnable_);
    fileReader.read(muteOsc1_);
    fileReader.read(muteOsc2_);
    fileReader.read(shape1_);
    fileReader.read(shape2_);
    fileReader.read(selectedOsc1_);
    fileReader.read(selectedOsc2_);
  }

  void paste(Oscillator* oscillator) {
    fmEnable_ = oscillator->fmEnable();
    amEnable_ = oscillator->amEnable();
    muteOsc1_ = oscillator->muteOsc1();
    muteOsc2_ = oscillator->muteOsc2();
    shape1_ = oscillator->shape1();
    shape2_ = oscillator->shape2();
    selectedOsc1_ = oscillator->selectedOsc1();
    selectedOsc1_ = oscillator->selectedOsc2();
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
