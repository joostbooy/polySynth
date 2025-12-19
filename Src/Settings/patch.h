#if !defined(Patch_h)
#define Patch_h

#include "envelope.h"
#include "filter.h"
#include "lfo.h"
#include "midi.h"
#include "modMatrix.h"
#include "oscillator.h"

class Patch {
 public:
 
  void init() {
    midi().init();
    oscillator().init();
    filter().init();
    ampEnvelope().init();
    modEnvelope().init();
    modMatrix().init();
    lfo(0).init();
    lfo(1).init();
  }

  Midi& midi() {
    return midi_;
  }

  Oscillator& oscillator() {
    return oscillator_;
  }

  Filter& filter() {
    return filter_;
  }

  Envelope& ampEnvelope() {
    return ampEnvelope_;
  }

  Envelope& modEnvelope() {
    return modEnvelope_;
  }

  ModMatrix& modMatrix() {
    return modMatrix_;
  }

  Lfo& lfo(int index) {
    return lfo_[index];
  }

 private:
  Midi midi_;
  Oscillator oscillator_;
  Filter filter_;
  Envelope ampEnvelope_;
  Envelope modEnvelope_;
  ModMatrix modMatrix_;
  Lfo lfo_[2];
};

#endif  // Patch_h
