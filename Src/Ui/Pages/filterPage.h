#ifndef FilterPage_h
#define FilterPage_h

#include "filter.h"
#include "filterList.h"
#include "listPage.h"

namespace FilterPage {

  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Filter filter_;
  FilterList filterList_;

  void clear() {
    settings_->filter().init();
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
  }

  const size_t targetFps() {
    return 1000 / 16;
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
