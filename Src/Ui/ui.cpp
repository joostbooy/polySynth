#include "ui.h"

#include "que.h"

void Ui::addEvent(ControlType type, uint8_t id, int8_t value) {
  Ui::Event e;
  e.type = type;
  e.id = id;
  e.value = value;
  uiQue.write(e);
}

void Ui::init(Settings* settings, Engine* engine, Matrix* matrix, Display* display, Switches* switches, Adc* adc) {
  engine_ = engine;
  settings_ = settings;
  matrix_ = matrix;
  display_ = display;
  switches_ = switches;
  adc_ = adc;

  lastInterval_ = 0;
  displayInterval_ = 0;

  std::fill(&lastButtonState_[0], &lastButtonState_[8 * 6], 0);

  canvas_.init();
  pots_.init();
  leds_.init();
  buttons_.init();
  pages_.init(settings, engine, this);

  resetAllPots();

  uiQue.clear();
}

void Ui::poll() {
  // Buttons
  uint8_t reading;
  matrix_->refresh(&reading);

  int collOffset = matrix_->currentCollumn() * 6;

  for (int i = 0; i < 6; i++) {
    int index = i + collOffset;
    bool state = reading & (1 << i);
    if (state != lastButtonState_[index]) {
      lastButtonState_[index] = state;
      addEvent(Ui::BUTTON, index, !state);
    }
  }

  // Encoder
  for (int i = 0; i < 4; ++i) {
    encoderRaw_[i] <<= 1;
    if (switches_->readEncoders(i)) {
      encoderRaw_[i] |= 1;
    }
  }

  for (size_t i = 0; i < 2; i++) {
    int a = (i * 2);
    int b = (i * 2) + 1;

    if ((encoderRaw_[a] & 0x03) == 0x02 && (encoderRaw_[b] & 0x03) == 0x00) {
      addEvent(Ui::ENCODER, i, -1);
    } else if ((encoderRaw_[b] & 0x03) == 0x02 && (encoderRaw_[a] & 0x03) == 0x00) {
      addEvent(Ui::ENCODER, i, 1);
    }
  }

  // Gates
  for (size_t i = 0; i < 2; i++) {
    // engine_->setGate(i, !switches_->readGate(i));
  }

  // Pots
  if (adc_->potReady()) {
    float value = adc_->readPot() * (1.f / 1023.f);
    pots_.write(adc_->currentPot(), value);
    pots_.filterAll();
    adc_->convertNextPot();
  }

  // CV
  if (adc_->cvReady()) {
    float value = adc_->readCv() * (1.f / 1023.f);
    engine_->modMatrixEngine().setCv(adc_->currentCv(), value);
    adc_->convertNextCv();
  }
}

void Ui::process() {
  while (uiQue.readable()) {
    Ui::Event e = uiQue.read();
    switch (e.type) {
      case Ui::BUTTON:
        buttons_.write(e.id, e.value);
        pages_.on_button(e.id, e.value);
        break;
      case Ui::ENCODER:
        pages_.on_encoder(e.id, e.value);
        break;
      default:
        break;
    }
  }

  uint32_t interval = (Micros::read() / 1000) - lastInterval_;

  if (interval >= 1) {
    lastInterval_ += interval;
    processLeds();
    processPots();
    processSwitches();
  }

  displayInterval_ += interval;
  if (displayInterval_ >= pages_.target_fps()) {
    processDisplay();
  }
}

void Ui::processLeds() {
  leds_.setAll(Leds::BLACK);
  pages_.refresh_leds();
  matrix_->setLeds(leds_.data());
}

void Ui::processDisplay() {
  while (display_->dmaBusy());
  displayInterval_ = 0;
  canvas_.clear();
  pages_.draw();
  display_->sendBuffer(canvas_.data(), canvas_.size());
}

void Ui::processSwitches() {
  Patch& p = settings_->selectedPatch();
  switches_->setAmEnable(p.amp().amEnable());
  switches_->setSync(p.oscillator().syncEnable());
  switches_->setOsc1(p.oscillator().type1());
  switches_->setOsc2(p.oscillator().type2());
  switches_->setFmEnable(p.oscillator().fmEnable());
  switches_->setMuteOsc1(p.oscillator().muteOsc1());
  switches_->setMuteOsc2(p.oscillator().muteOsc2());
  switches_->setVcoModSource(p.oscillator().modSource());
  switches_->setFmFilter1Enable(p.filter().fmEnable1());
  switches_->setFmFilter2Enable(p.filter().fmEnable2());
  switches_->setSelectedFilter(p.filter().type(), p.filter().routing());
}

void Ui::processPots() {
  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    if (!potIsLocked(i)) {
      writePotToSetting(i);
    } else {
      if (potUnlockDirection_[i] == CW) {
        if (pots_.read(i) >= readPotToSetting(i)) {
          writePotToSetting(i);
          unlockPot(i);
        }
      } else if (potUnlockDirection_[i] == CCW) {
        if (pots_.read(i) <= readPotToSetting(i)) {
          writePotToSetting(i);
          unlockPot(i);
        }
      }
    }
  }
}

float Ui::readPotToSetting(Patch &p, int id) {
  switch (id) {
    case Pots::RESONANCE_B:   return p.filter().resonance2();
    case Pots::RESONANCE_A:   return p.filter().resonance1();
    case Pots::TUNE_A:        return p.oscillator().tune1();
    case Pots::TUNE_B:        return p.oscillator().tune2();
    case Pots::SHAPE_A:       return p.oscillator().shape1();
    case Pots::SHAPE_B:       return p.oscillator().shape2();
    case Pots::CUTOFF_A:      return p.filter().cutoff1();
    case Pots::CUTOFF_B:      return p.filter().cutoff2();
    case Pots::PAN:           return p.amp().pan();
    case Pots::MOD_DEPTH:     return p.oscillator().modDepth();
    case Pots::DRIVE:         return p.amp().drive();
    case Pots::SLIDE_AMMOUNT: return p.oscillator().slideAmmount1();
    case Pots::LFO_SKEW_1:    return p.lfo(0).skew();
    case Pots::LFO_SPEED_2:   return p.lfo(0).speed();
    case Pots::LFO_SKEW_2:    return p.lfo(1).skew();
    case Pots::LFO_SPEED_1:   return p.lfo(1).speed();
    case Pots::A_TIME_1:      return p.envelope(0).attackTime();
    case Pots::A_SHAPE_1:     return p.envelope(0).attackShape();
    case Pots::D_TIME_1:      return p.envelope(0).decayTime();
    case Pots::D_SHAPE_1:     return p.envelope(0).decayShape();
    case Pots::S_LEVEL_1:     return p.envelope(0).sustainLevel();
    case Pots::S_HOLD_1:      return p.envelope(0).holdTime();
    case Pots::R_TIME_1:      return p.envelope(0).releaseTime();
    case Pots::R_SHAPE_1:     return p.envelope(0).releaseShape();
    case Pots::A_TIME_2:      return p.envelope(1).attackTime();
    case Pots::A_SHAPE_2:     return p.envelope(1).attackShape();
    case Pots::D_TIME_2:      return p.envelope(1).decayTime();
    case Pots::D_SHAPE_2:     return p.envelope(1).decayShape();
    case Pots::S_LEVEL_2:     return p.envelope(1).sustainLevel();
    case Pots::S_HOLD_2:      return p.envelope(1).holdTime();
    case Pots::R_TIME_2:      return p.envelope(1).releaseTime();
    case Pots::R_SHAPE_2:     return p.envelope(1).releaseShape();
    default:
      break;
  }
  return 0.f;
}

void Ui::writePotToSetting(int id) {
  Patch& p = settings_->selectedPatch();

  switch (id) {
    case Pots::RESONANCE_B:
      p.filter().setResonace2(pots_.read(Pots::RESONANCE_B));
      break;
    case Pots::RESONANCE_A:
      p.filter().setResonace1(pots_.read(Pots::RESONANCE_A));
      break;
    case Pots::TUNE_A:
      p.oscillator().setTune1(pots_.read(Pots::TUNE_A));
      break;
    case Pots::TUNE_B:
      p.oscillator().setTune2(pots_.read(Pots::Pots::TUNE_B));
      break;
    case Pots::SHAPE_A:
      p.oscillator().setShape1(pots_.read(Pots::SHAPE_A));
      break;
    case Pots::SHAPE_B:
      p.oscillator().setShape2(pots_.read(Pots::SHAPE_B));
      break;
    case Pots::CUTOFF_A:
      p.filter().setCutoff1(pots_.read(Pots::CUTOFF_A));
      break;
    case Pots::CUTOFF_B:
      p.filter().setCutoff2(pots_.read(Pots::CUTOFF_B));
      break;
    case Pots::PAN:
      p.amp().setPan(pots_.read(Pots::PAN));
      break;
    case Pots::MOD_DEPTH:
      p.oscillator().setModDepth(pots_.read(Pots::MOD_DEPTH));
      break;
    case Pots::DRIVE:
      p.amp().setDrive(pots_.read(Pots::DRIVE));
      break;
    case Pots::SLIDE_AMMOUNT:
      p.oscillator().setSlideAmmount1(pots_.read(Pots::SLIDE_AMMOUNT));
      break;
    case Pots::LFO_SKEW_1:
      p.lfo(0).setSkew(pots_.read(Pots::LFO_SKEW_1));
      break;
    case Pots::LFO_SPEED_2:
      p.lfo(1).setSpeed(pots_.read(Pots::LFO_SPEED_2));
      break;
    case Pots::LFO_SKEW_2:
      p.lfo(1).setSkew(pots_.read(Pots::LFO_SKEW_2));
      break;
    case Pots::LFO_SPEED_1:
      p.lfo(0).setSpeed(pots_.read(Pots::LFO_SPEED_1));
      break;
    case Pots::A_TIME_1:
      p.envelope(0).setAttackTime(pots_.read(Pots::A_TIME_1));
      break;
    case Pots::A_SHAPE_1:
      p.envelope(0).setAttackShape(pots_.read(Pots::A_SHAPE_1));
      break;
    case Pots::D_TIME_1:
      p.envelope(0).setDecayTime(pots_.read(Pots::D_TIME_1));
      break;
    case Pots::D_SHAPE_1:
      p.envelope(0).setDecayShape(pots_.read(Pots::D_SHAPE_1));
      break;
    case Pots::S_LEVEL_1:
      p.envelope(0).setSustainLevel(pots_.read(Pots::S_LEVEL_1));
      break;
    case Pots::S_HOLD_1:
      p.envelope(0).setHoldTime(pots_.read(Pots::S_HOLD_1));
      break;
    case Pots::R_TIME_1:
      p.envelope(0).setReleaseTime(pots_.read(Pots::R_TIME_1));
      break;
    case Pots::R_SHAPE_1:
      p.envelope(0).setReleaseShape(pots_.read(Pots::R_SHAPE_1));
      break;
    case Pots::A_TIME_2:
      p.envelope(1).setAttackTime(pots_.read(Pots::A_TIME_2));
      break;
    case Pots::A_SHAPE_2:
      p.envelope(1).setAttackShape(pots_.read(Pots::A_SHAPE_2));
      break;
    case Pots::D_TIME_2:
      p.envelope(1).setDecayTime(pots_.read(Pots::D_TIME_2));
      break;
    case Pots::D_SHAPE_2:
      p.envelope(1).setDecayShape(pots_.read(Pots::D_SHAPE_2));
      break;
    case Pots::S_LEVEL_2:
      p.envelope(1).setSustainLevel(pots_.read(Pots::S_LEVEL_2));
      break;
    case Pots::S_HOLD_2:
      p.envelope(1).setHoldTime(pots_.read(Pots::S_HOLD_2));
      break;
    case Pots::R_TIME_2:
      p.envelope(1).setReleaseTime(pots_.read(Pots::R_TIME_2));
      break;
    case Pots::R_SHAPE_2:
      p.envelope(1).setReleaseShape(pots_.read(Pots::R_SHAPE_2));
      break;
    default:
      break;
  }
}

void Ui::resetPot(int id) {
  uint32_t data = lockedPots_;
  lockedPots_ = data | (1 << id);

  float pot = pots_.read(id);
  float setting = readPotToSetting(id);
  if (pot == setting) {
    unlockPot(id);
  } else if (pot < setting) {
    potUnlockDirection_[id] = CW;
  } else {
    potUnlockDirection_[id] = CCW;
  }
}

void Ui::resetAllPots() {
  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    resetPot(i);
  }
}

void Ui::unlockAllPots() {
  lockedPots_ = 0;
  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    writePotToSetting(i);
  }
}