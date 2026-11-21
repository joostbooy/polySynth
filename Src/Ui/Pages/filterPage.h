#ifndef FilterPage_h
#define FilterPage_h

#include "filter.h"
#include "filterList.h"
#include "listPage.h"

namespace FilterPage {

  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;
  using TopPage::canvas_;

  bool pasteable_;
  Filter filter_;
  FilterList filterList_;

  void clear() {
    settings_->filter().init();
    ui_->resetAllPots();
  }

  void randomise() {
    settings_->filter().randomise();
    ui_->resetAllPots();
  }

  void copy() {
    filter_.paste(&settings_->filter());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->filter().paste(&filter_);
      ui_->resetAllPots();
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    filter_.init();
    filterList_.init(engine_, settings_, ui_);
  }

  void enter() {
    ListPage::setList(&filterList_);
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

  void draw() {
    ListPage::draw();
/*
    const int x = 128;
    const int y = 12;
    const int h = 30;
    const int w = 100;

    int lastX = 0;
    int lastY = 0;

    for (int i = 0; i < w; i++) {
      int x_ = i + x;
      int y_ = 0;
      canvas_->drawLine(lastX, lastY, x_, y_, Canvas::BLACK);
      lastX = x_;
      lastY = y_;
    }
*/
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

};  // namespace FilterPage

#endif
