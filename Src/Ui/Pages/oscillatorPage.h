#ifndef OscillatorPage_h
#define OscillatorPage_h

#include "listPage.h"
#include "oscillator.h"
#include "oscillatorList.h"

namespace OscillatorPage {

  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Oscillator oscillator_;
  OscillatorList oscillatorList_;

  void clear() {
    settings_->oscillator().init();
  }

  void randomise() {
    settings_->oscillator().randomise();
    ui_->resetAllPots();
  }

  void copy() {
    oscillator_.paste(&settings_->oscillator());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->oscillator().paste(&oscillator_);
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    oscillator_.init();
    oscillatorList_.init(engine_, settings_, ui_);
  }

  void enter() {
    ListPage::setList(&oscillatorList_);
    ListPage::setClearCallback(&clear);
    ListPage::setCopyCallback(&copy);
    ListPage::setPasteCallback(&paste);
    ListPage::setRandomiseCallback(&randomise);
    ListPage::enter();
  }

  void exit() {
    ListPage::exit();
  }

  void onButton(int id, int state) {
    ListPage::onButton(id, state);
  }

  void onEncoder(int id, int state) {
    ListPage::onEncoder(id, state);
  }

  void refreshLeds() {
    ListPage::refreshLeds();
  }

  float readSquare(float phase, float shape) {
    return phase < shape ? 1.f : 0.f;
  }

  float readSaw(float phase, float shape) {
    return EngineUtils::foldWave(phase, shape);
  }

  float readTriangle(float phase, float shape) {
    float wave = phase < 0.5f ? phase * 2.f : (1.f - phase) * 2.f;
    return EngineUtils::foldWave(wave, shape);
  }

  float readSine(float phase, float shape) {
    float wave = (sinf(phase * 2.0f * float(M_PI)) + 1.0f) * 0.5f;
    return EngineUtils::foldWave(wave, shape);
  }

  float readNoise(float shape) {
    return EngineUtils::foldWave(Rng::reciprocal(), shape);
  }

  void draw() {
    ListPage::draw();

    const int x = 128;
    const int w = 100;
    const int h = 18;
    const int yWave1 = 12;
    const int yWave2 = 12 + h + 2;

    float wave = 0.f;
    float phase = 0.f;
    float shape1 = settings_->oscillator().shape1();
    float shape2 = settings_->oscillator().shape2();
    const float inc = 1.f / w;

    int lastX[2] = {x, x};
    int lastY[2] = {yWave1 + h, yWave2 + h};

    for (int i = 0; i < w; i++) {
      // Wave 1
      switch (settings_->oscillator().type1()) {
        case Oscillator::SAW1:
          wave = readSaw(phase, shape1);
          break;
        case Oscillator::SQUARE1:
          wave = readSquare(phase, shape1);
          break;
        case Oscillator::TRIANGLE1:
          wave = readTriangle(phase, shape1);
          break;
        case Oscillator::SINE1:
          wave = readSine(phase, shape1);
          break;
        default:
          break;   
      }

      int x_ = x + i;
      int y_ = yWave1 + (h - (h * wave));
      canvas_->drawLine(lastX[0], lastY[0], x_, y_, Canvas::BLACK);
      lastX[0] = x_;
      lastY[0] = y_;

      // Wave 2
      switch (settings_->oscillator().type2()) {
        case Oscillator::SAW2:
          wave = readSaw(phase, shape2);
          break;
        case Oscillator::SQUARE2:
          wave = readSquare(phase, shape2);
          break;
        case Oscillator::TRIANGLE2:
          wave = readTriangle(phase, shape2);
          break;
        case Oscillator::NOISE2:
          wave = readNoise(shape2);
          break;
        default:
          break;
      }

      y_ = yWave2 + (h - (h * wave));
      canvas_->drawLine(lastX[1], lastY[1], x_, y_, Canvas::BLACK);
      lastX[1] = x_; 
      lastY[1] = y_;

      phase += inc;
    }
  }

  const size_t targetFps() {
    return 16;
  }

  Pages::Page page = {
      &init,
      &enter,
      &exit,
      &draw,
      &refreshLeds,
      &onButton,
      &onEncoder,
      &targetFps,
  };

};  // namespace OscillatorPage

#endif
