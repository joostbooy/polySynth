#ifndef PatchPage_h
#define PatchPage_h

#include "patch.h"

namespace PatchPage {

  using TopPage::buttons_;
  using TopPage::engine_;
  using TopPage::pages_;
  using TopPage::settings_;
  using TopPage::ui_;

  bool pasteable_;
  Patch patch_;

  enum FooterOption {
    SAVE,
    CLEAR,
    COPY,
    PASTE,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"SAVE", "CLEAR", "COPY", "PASTE"};

  int newIndex;

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
          // open confirmation page
          if (settings_->savePatch()) {
            MessagePainter::show("PATCH SAVED");
          } else {
            MessagePainter::show("FAILED");
          }
          break;
        case COPY:
          copy();
          MessagePainter::show("PATCH COPIED");
          break;
        case PASTE:
          ConfirmationPage::set("OVERWRITE PATCH ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              if (paste()) {
                MessagePainter::show("PATCH PASTED");
              } else {
                MessagePainter::show("FAILED! CLIPBOARD EMPTY");
              }
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void loadNewPatch() {
    settings_->selectPatchIndex(newIndex);
    settings_->loadPatch();
  }

  void on_encoder(int id, int inc) {
    int lastIndex = settings_->patchIndex();
    newIndex = SettingsUtils::clip(0, Settings::kNumPatches - 1, lastIndex + inc);

    if (lastIndex != newIndex) {
      if (settings_->selectedPatch().readHash() != settings_->selectedPatchOrignalState().readHash()) {
        ConfirmationPage::set("UNSAVED CHANGES WILL BE LOST, CONTINUE?", [](int option) {
          if (option == ConfirmationPage::CONFIRM) {
            loadNewPatch();
          }
        });
        pages_->open(Pages::CONFIRMATION_PAGE);
      } else {
        loadNewPatch();
      }
    }
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
