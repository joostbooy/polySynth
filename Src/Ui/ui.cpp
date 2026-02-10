#include "ui.h"

#include "que.h"

static const int enc_a_row = 2;
static const int enc_b_row = 3;
static const int num_buttons_rows = 6;
static const int buttons_rows[num_buttons_rows] = {0, 1, 4, 5, 6, 7};

void Ui::addEvent(ControlType type, uint8_t id, int8_t value) {
  Ui::Event e;
  e.type = type;
  e.id = id;
  e.value = value;
  uiQue.write(e);
}

void Ui::init(Settings* settings, Engine* engine, Matrix* matrix, Display* display, Switches* switches, Adc *adc) {
  engine_ = engine;
  matrix_ = matrix;
  display_ = display;
  switches_ = switches;
  adc_ = adc;

  lastInterval_ = 0;
  displayInterval_ = 0;

  canvas_.init();
  pots_.init();
  leds_.init();
  buttons_.init();
  pages_.init(settings, engine, this);

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
    if (state != lastState_[index]) {
      lastState_[index] = state;
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
      addEvent(Ui::ENCODER, 0, -1);
    } else if ((encoderRaw_[b] & 0x03) == 0x02 && (encoderRaw_[a] & 0x03) == 0x00) {
      addEvent(Ui::ENCODER, 0, 1);
    }
  }

  // Pots
  if (adc_->potReady()) {
    float value = adc_->readPot() * (1.f / 1023.f);
    pots_.write(adc_->currentPot(), value);
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
    leds_.setAll(Leds::BLACK);
    pages_.refresh_leds();
    matrix_->setLeds(leds_.data());
  }

  displayInterval_ += interval;
  if (displayInterval_ >= pages_.target_fps()) {
    sendDisplay();
  }
}

void Ui::sendDisplay() {
  while (display_->dmaBusy());
  displayInterval_ = 0;
  canvas_.clear();
  pages_.draw();
  display_->sendBuffer(canvas_.data(), canvas_.size());
}