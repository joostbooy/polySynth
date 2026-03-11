#ifndef Voice_h
#define Voice_h

#include "dac.h"
#include "envelopeEngine.h"
#include "lfoEngine.h"
#include "modMatrixEngine.h"

class Voice {
 public:
  enum State {
    AVAILABLE = 0,
    ACTIVE = 1,
  };

  void init(int index, Settings* settings, ModMatrixEngine* modMatrixEngine, Dac *dac) {
    index_ = index;
   
    dac_ = dac;
    settings_ = settings;
    modMatrixEngine_ = modMatrixEngine;
    state_ = AVAILABLE;
    keyPressed_ = false;
    stopRequested_ = false;
    note_ = 60;
    lastNote_ = 60;

    envelopeEngine_[0].init(&settings->envelope(0));
    envelopeEngine_[1].init(&settings->envelope(1));

    lfoEngine_[0].init(&settings->lfo(0));
    lfoEngine_[1].init(&settings->lfo(1));
  }

  uint8_t port() {
    return port_;
  }

  uint8_t note() {
    return note_;
  }

  uint8_t channel() {
    return channel_;
  }

  bool keyPressed() {
    return keyPressed_;
  }

  EnvelopeEngine& envelopeEngine(int index_) {
    return envelopeEngine_[index_];
  }

  LfoEngine& lfoEngine(int index_) {
    return lfoEngine_[index_];
  }

  void requestStop() {
    stopRequested_ = true;
  }

  bool isAvailable() {
    return state_ == AVAILABLE;
  }

  void noteOn(MidiEngine::Event& e) {
    lastNote_ = note_;
    port_ = e.port;
    note_ = e.data[0];
    channel_ = e.message & 0x0F;
    velocity_ = e.data[1] * (1.f / 127.f);

    keyPressed_ = true;
    stopRequested_ = false;
    fadePhase_ = 1.f;
    slidePhase1_ = 0.f;
    slidePhase2_ = 0.f;

    lfoEngine_[0].retrigger();
    lfoEngine_[1].retrigger();

    envelopeEngine_[0].attack();
    envelopeEngine_[1].attack();

    state_ = ACTIVE;
  }

  void noteOff() {
    keyPressed_ = false;
    envelopeEngine_[0].release();
    envelopeEngine_[1].release();
  }

  void setPlayOrder(int playOrder) {
    playOrder_ = playOrder;
    lfoEngine_[0].setPlayOrder(playOrder);
    lfoEngine_[1].setPlayOrder(playOrder);
  }

  void update() {
    if (stopRequested_ == true && fadePhase_ > 0.f) {
      fadePhase_ -= 1000.f / (SAMPLE_RATE * 24.f);
      if (fadePhase_ < 0.f) {
        fadePhase_ = 0.f;
      }
    }

    modMatrixEngine_->setMidiNote(note_);
    modMatrixEngine_->setMidiVelocity(velocity_);
    modMatrixEngine_->setEnvelope(0, envelopeEngine_[0].next());
    modMatrixEngine_->setEnvelope(1, envelopeEngine_[1].next());
    modMatrixEngine_->setLfo(0, lfoEngine_[0].next());
    modMatrixEngine_->setLfo(1, lfoEngine_[1].next());
    float* data = modMatrixEngine_->process();

    dac_->set(index_, 0, data[ModMatrix::SHAPE_2] * 65535);
    dac_->set(index_, 1, data[ModMatrix::VCO_MOD_DEPTH] * 65535);
    dac_->set(index_, 2, data[ModMatrix::SHAPE_1] * 65535);
    dac_->set(index_, 3, calculatePitchOsc1(data[ModMatrix::TUNE_1]));
    dac_->set(index_, 4, data[ModMatrix::DRIVE] * 65535);
    dac_->set(index_, 5, data[ModMatrix::GAIN] * fadePhase_ * 65535);
    dac_->set(index_, 6, data[ModMatrix::RESONANCE_2] * 65535);
    dac_->set(index_, 7, calculatePitchOsc2(data[ModMatrix::TUNE_2]));
    dac_->set(index_, 8, data[ModMatrix::CUTOFF_1] * 65535);
    dac_->set(index_, 9, data[ModMatrix::CUTOFF_2] * 65535);
    dac_->set(index_, 10, calculatePan(data[ModMatrix::PAN]));
    dac_->set(index_, 11, data[ModMatrix::RESONANCE_1] * 65535);

    if (fadePhase_ == 0.f || envelopeEngine_[0].stage() == EnvelopeEngine::IDLE) {
      state_ = AVAILABLE;
    }

    if (settings_->calibration().enabled()) {
      uint16_t gain = (index_ == settings_->calibration().selectedVoice()) ? 65535 : 0;
      dac_->set(index_, 5, gain);
      dac_->set(index_, 3, settings_->calibration().min());
      dac_->set(index_, 7, settings_->calibration().max());
    }
  }

 private:
  int playOrder_ = 0;
  bool keyPressed_;
  bool stopRequested_;
  uint8_t note_;
  uint8_t lastNote_;
  uint8_t port_;
  uint8_t channel_;
  State state_;
  int index_;
  float velocity_;
  float fadePhase_;
  float slidePhase1_;
  float slidePhase2_;
  Dac* dac_;
  Settings* settings_;
  ModMatrixEngine* modMatrixEngine_;
  EnvelopeEngine envelopeEngine_[2];
  LfoEngine lfoEngine_[Settings::kNumLfos];

  uint16_t calculatePan(float pan) {
    return EngineUtils::spread(pan, settings_->amp().panSpread(), playOrder_) * 65535;
  }

  uint16_t calculatePitchOsc1(float modValue) {
    Oscillator& osc = settings_->oscillator();
    Calibration& cal = settings_->calibration();

    int noteValue = cal.noteToValue(24 + osc.octaveOffset1());
    int tuneValue = ((2.f * modValue) - 1.f) * cal.semiNoteValue() * 11;

    if (settings_->oscillator().trackNote1()) {
      noteValue = cal.noteToValue(note_ + osc.octaveOffset1());
      if (osc.slideEnable1()) {
        int lastNoteValue = cal.noteToValue(lastNote_ + osc.octaveOffset1());
        noteValue = Dsp::cross_fade(lastNoteValue, noteValue, slidePhase1_);
        slidePhase1_ += inc(osc.slideAmmount1());
        if (slidePhase1_ >= 1.f) {
          slidePhase1_ = 1.f;
        }
      }
    }
    return SettingsUtils::clip(cal.min(), cal.max(), tuneValue + noteValue);
  }

  uint16_t calculatePitchOsc2(float modValue) {
    Oscillator& osc = settings_->oscillator();
    Calibration& cal = settings_->calibration();

    int noteValue = cal.noteToValue(24 + osc.octaveOffset2());
    int tuneValue = ((2.f * modValue) - 1.f) * cal.semiNoteValue() * 11;

    if (settings_->oscillator().trackNote2()) {
      noteValue = cal.noteToValue(note_ + osc.octaveOffset2());
      if (osc.slideEnable2()) {
        int lastNoteValue = cal.noteToValue(lastNote_ + osc.octaveOffset2());
        noteValue = Dsp::cross_fade(lastNoteValue, noteValue, slidePhase1_);
        slidePhase1_ += inc(osc.slideAmmount2());
        if (slidePhase1_ >= 1.f) {
          slidePhase1_ = 1.f;
        }
      }
    }
    return SettingsUtils::clip(cal.min(), cal.max(), tuneValue + noteValue);
  }

  float inc(float value) {
    return LookupTablesUtils::read(lut_phase_inc, value);
  }
};

#endif
