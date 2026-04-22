#ifndef AmpPage_h
#define AmpPage_h

#include "amp.h"
#include "ampList.h"
#include "listPage.h"

namespace AmpPage {

  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Amp amp_;
  AmpList ampList_;

  void clear() {
    settings_->amp().init();
    ui_->resetAllPots();
  }

  void copy() {
    amp_.paste(&settings_->amp());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->amp().paste(&amp_);
      ui_->resetAllPots();
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    amp_.init();
    ampList_.init(engine_, settings_, ui_);
  }

  void enter() {
    ListPage::setList(&ampList_);
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

};  // namespace AmpPage

#endif
