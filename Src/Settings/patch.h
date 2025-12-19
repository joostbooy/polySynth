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
    lfo(0).init();
    lfo(1).init();
    midi().init();
    oscillator().init();
    filter().init();
    ampEnvelope().init();
    modEnvelope().init();
    modMatrix().init();
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

  // Storage
  void save(FileWriter& fileWriter) {
    lfo_[0].save(fileWriter);
    lfo_[1].save(fileWriter);
    midi_.save(fileWriter);
    oscillator_.save(fileWriter);
    filter_.save(fileWriter);
    ampEnvelope_.save(fileWriter);
    modEnvelope_.save(fileWriter);
    modMatrix_.save(fileWriter);
  }

  void load(FileReader& fileReader) {
    lfo_[0].load(fileReader);
    lfo_[1].load(fileReader);
    midi_.load(fileReader);
    oscillator_.load(fileReader);
    filter_.load(fileReader);
    ampEnvelope_.load(fileReader);
    modEnvelope_.load(fileReader);
    modMatrix_.load(fileReader);
  }

  void paste(Patch* patch) {
    lfo_[0].paste(&patch->lfo(0));
    lfo_[1].paste(&patch->lfo(1));
    midi_.paste(&patch->midi());
    oscillator_.paste(&patch->oscillator());
    filter_.paste(&patch->filter());
    ampEnvelope_.paste(&patch->ampEnvelope());
    modEnvelope_.paste(&patch->modEnvelope());
    modMatrix_.paste(&patch->modMatrix());
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
