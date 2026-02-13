#ifndef PatchPage_h
#define PatchPage_h

#include "patch.h"

namespace PatchPage {

  using TopPage::buttons_;
  using TopPage::engine_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Patch patch_;

  enum FooterOption {
    SAVE,
    LOAD,
    CLEAR,
    MENU,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"SAVE", "LOAD", "CLEAR", "MENU"};

  void clear() {
    settings_->selectedPatch().init();
    ui_->unlockAllPots();
  }

  void copy() {
    patch_.paste(&settings_->selectedPatch());
    pasteable_ = true;
  }

  bool paste() {
    if (pasteable_) {
      settings_->selectedPatch().paste(&patch_);
      return true;
    }
    return false;
  }

  void init() {
    pasteable_ = false;
    patch_.init();
  }

  void enter() {
  }

  void exit() {
  }

  void on_button(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case SAVE:
          if (settings_->savePatch()) {
            MessagePainter::show("PATCH SAVED");
          } else {
            MessagePainter::show("FAILED");
          }
          break;
        case LOAD:
          settings_->loadPatch();
          MessagePainter::show("PATCH LOADED");
          break;
        default:
          break;
      }
    }
  }

  void on_encoder(int id, int inc) {
    settings_->selectPatchIndex(settings_->patchIndex() + inc);
  }

  void refresh_leds() {
  }

  void draw() {
    // settings_->patch(i).name();
    WindowPainter::draw_footer(footerOptionText, NUM_FOOTER_OPTIONS);
  }

  const size_t target_fps() {
    return 1000 / 16;
  }

  Pages::Page page = {
      &init,
      &enter,
      &exit,
      &draw,
      &refresh_leds,
      &on_button,
      &on_encoder,
      &target_fps,
  };

};  // namespace PatchPage

#endif
