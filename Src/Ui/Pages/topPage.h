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
Canvas* canvas_;
Pages* pages_;
Leds* leds_;
Buttons* buttons_;
Pots* pots_;

Ui* ui_;
Disk* disk_;

StringBuilderBase<63> str_;

int chapter_top_page_ = 0;
const int kMaxPageRows = 6;
Chapter chapter_;

void scroll_to_chapter_page() {
  int page = chapter_.entry()->page_index();

  if (page < chapter_top_page_) {
    chapter_top_page_ = page;
  } else if (page >= (chapter_top_page_ + kMaxPageRows)) {
    chapter_top_page_ = page - (kMaxPageRows - 1);
  }
}

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

  chapter_.init();
  chapter_.set(Chapter::INSTRUMENT);
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
  if (pages_->num_open() == 0) {
    int index = chapter_.entry()->page_index();
    chapter_.entry()->set_page_index(index + state);
    scroll_to_chapter_page();
  }
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

  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    if (ui_->potIsLocked(i)) {
      leds_->setPot(i, Leds::BLACK);
    } else {
      leds_->setPot(i, Leds::RED);
    }
  }
}

void draw() {
  if (pages_->num_open() == 0) {
    const int row_h = 8;
    const int w = 128;
    const int h = row_h * kMaxPageRows;
    const int x = (canvas_->width() - w) / 2;
    const int y = (canvas_->height() - h) / 2;

    canvas_->drawText(x, 0, w, row_h, chapter_.name(), Canvas::CENTER, Canvas::CENTER);

    for (int i = 0; i < kMaxPageRows; ++i) {
      int page = i + chapter_top_page_;
      int page_y = (i * row_h) + y;

      if (page < chapter_.entry()->size()) {
        canvas_->drawText(x + 4, page_y, chapter_.entry()->name(page));
        if (page == chapter_.entry()->page_index()) {
          canvas_->fill(x, page_y, w, row_h, Canvas::INVERTED);
        }
      }
    }

    const int bar_w = 8;
    const int bar_x = x + (w - bar_w);
    WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, chapter_top_page_, chapter_.entry()->size(), kMaxPageRows);
  }

  MessagePainter::draw(pages_->target_fps());
}

};  // namespace TopPage

#endif
