#ifndef TopPage_h
#define TopPage_h

#include "buttons.h"
#include "canvas.h"
#include "engine.h"
#include "leds.h"
#include "messagePainter.h"
#include "pages.h"
#include "settings.h"
#include "textBufferPainter.h"
#include "windowPainter.h"

namespace TopPage {

Settings* settings_;
Engine* engine_;
Ui* ui_;
Disk* disk_;
Canvas* canvas_;
Pages* pages_;
Leds* leds_;
Buttons* buttons_;
Pots* pots_;

StringBuilderBase<63> str_;

void init(Settings* settings, Engine* engine, Ui* ui) {
  settings_ = settings;
  engine_ = engine;
  ui_ = ui;
  canvas_ = &ui->canvas();
  leds_ = &ui->leds();
  pots_ = &ui->pots();
  pages_ = &ui->pages();
  buttons_ = &ui->buttons();
  disk_ = settings_->disk();

  MessagePainter::init(canvas_);
  TextBufferPainter::init(canvas_);
  WindowPainter::init(canvas_);
}

void enter() {
}

void exit() {
}

void on_button(int id, int state) {
  if (!state) {
    return;
  }

  Patch& p = settings_->selectedPatch();

  switch (id) {
    case Buttons::TUNE_1_OCT_UP:
      p.oscillator().setOctaveOffset1(p.oscillator().octaveOffset1() + 12);
      break;
    case Buttons::TUNE_1_OCT_DOWN:
      p.oscillator().setOctaveOffset1(p.oscillator().octaveOffset1() - 12);
      break;
    case Buttons::TUNE_2_OCT_UP:
      p.oscillator().setOctaveOffset2(p.oscillator().octaveOffset2() + 12);
      break;
    case Buttons::TUNE_2_OCT_DOWN:
      p.oscillator().setOctaveOffset2(p.oscillator().octaveOffset2() - 12);
      break;
    case Buttons::LFO_1_TYPE:
      p.lfo(0).setType(p.lfo(0).type() + 1);
      break;
    case Buttons::LFO_2_TYPE:
      p.lfo(1).setType(p.lfo(1).type() + 1);
      break;
    case Buttons::TUNE_1_VCO_MOD:
      p.oscillator().setFmEnable(!p.oscillator().fmEnable());
      break;
    case Buttons::TUNE_1_MOD_TYPE:
      //
      break;
    case Buttons::VCO_2_SYNC:
      p.oscillator().setSyncEnable(!p.oscillator().syncEnable());
      break;
    case Buttons::TUNE_2_MOD_TYPE:
      //
      break;
    case Buttons::SHAPE_1_MOD_TYPE:
      //
      break;
    case Buttons::VCO_1_OSC_TYPE:
      p.oscillator().setType1(p.oscillator().type1() + 1);
      break;
    case Buttons::SHAPE_2_MOD_TYPE:
      //
      break;
    case Buttons::VCO_2_OSC_TYPE:
      p.oscillator().setType2(p.oscillator().type2() + 1);
      break;
    case Buttons::VCO_MOD_SOURCE:
      p.oscillator().setModSource(p.oscillator().modSource() + 1);
      break;
    case Buttons::SLIDE_VCO_SELECT:
      //
      break;
    case Buttons::CUTOFF_1_MOD_TYPE:
      //
      break;
    case Buttons::VCO_1_MUTE:
      p.oscillator().setMuteOsc1(!p.oscillator().muteOsc1());
      break;
    case Buttons::CUTOFF_2_MOD_TYPE:
      //
      break;
    case Buttons::VCO_2_MUTE:
      p.oscillator().setMuteOsc2(!p.oscillator().muteOsc2());
      break;
    case Buttons::VCO_MOD_DEPTH_MOD_TYPE:
      //
      break;
    case Buttons::VOLUME_MOD_TYPE:
      //
      break;
    case Buttons::VCF_1_TYPE:
      p.filter().setType(p.filter().type() + 1);
      break;
    case Buttons::CUTOFF_1_VCO_MOD:
      p.filter().setFmEnable1(!p.filter().fmEnable1());
      break;
    case Buttons::VCF_2_ROUTING:
      p.filter().setRouting(p.filter().routing() + 1);
      break;
    case Buttons::CUTOFF_2_VCO_MOD:
      p.filter().setFmEnable2(!p.filter().fmEnable2());
      break;
    case Buttons::VOLUME_VCO_MOD:
      p.amp().setAmEnable(!p.amp().amEnable());
      break;
    case Buttons::PAN_MOD_TYPE:
      //
      break;
    default:
      break;
  }
}

void on_encoder(int id, int state) {

}

void refresh_leds() {
  Patch& p = settings_->selectedPatch();

  leds_->setAm(p.amp().amEnable());
  leds_->setOsc2Sync(p.oscillator().syncEnable());
  leds_->setOsc1Type(p.oscillator().type1());
  leds_->setOsc2Type(p.oscillator().type2());
  leds_->setOsc1Fm(p.oscillator().fmEnable());
  leds_->setMute1(p.oscillator().muteOsc1());
  leds_->setMute2(p.oscillator().muteOsc2());
  leds_->setOscModSource(p.oscillator().modSource());

  leds_->setFilter1Fm(p.filter().fmEnable1());
  leds_->setFilter2Fm(p.filter().fmEnable2());
  leds_->setFilterType(p.filter().type());
  leds_->setFilterRouting(p.filter().routing());

  leds_->setLfo1Type(p.lfo(0).type());
  leds_->setLfo2Type(p.lfo(1).type());
  leds_->setLfo1Tempo(engine_->voiceEngine().mostRecentVoice().lfoEngine(0).phase() < 0.5f);
  leds_->setLfo2Tempo(engine_->voiceEngine().mostRecentVoice().lfoEngine(1).phase() < 0.5f);

  leds_->setModMatrix(&p.modMatrix());

  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    if (ui_->potIsLocked(i)) {
      leds_->setPot(i, Leds::BLACK);
    } else {
      leds_->setPot(i, Leds::RED);
    }
  }
}

void draw() {
  MessagePainter::draw(pages_->target_fps());
}

};  // namespace TopPage

#endif
