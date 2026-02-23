#ifndef PatchPage_h
#define PatchPage_h

#include "patch.h"

namespace PatchPage {

  using TopPage::buttons_;
  using TopPage::engine_;
  using TopPage::pages_;
  using TopPage::settings_;
  using TopPage::ui_;
  using TopPage::canvas_;
  using TopPage::str_;

  bool pasteable_;
  Patch patch_;

  enum FooterOption {
    SAVE,
    INIT,
    COPY,
    NEXT,
    PREV,
    PASTE,
    EDIT_NAME,

    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"SAVE", "INIT", "COPY", ">", "<", "PASTE", "EDIT NAME"};

  int footerOptionsOffset;
  int newIndex;

  void init() {
    pasteable_ = false;
    patch_.init();
  }

  void enter() {
    footerOptionsOffset = 0;
  }

  void exit() {
  }

  void on_button(int id, int state) {
    if (state) {

      int option = buttons_->toFunction(id);
      if (option >= 0) {
        option += footerOptionsOffset;
      }
      
      switch (option) {
        case SAVE:
          ConfirmationPage::set("OVERWRITE PATCH ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              if (settings_->savePatch()) {
                MessagePainter::show("PATCH SAVED");
              } else {
                MessagePainter::show("FAILED");
              }
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
          break;
        case COPY:
          patch_.paste(&settings_->selectedPatch());
          pasteable_ = true;
          MessagePainter::show("PATCH COPIED");
          break;
        case PASTE:
          if (pasteable_) {
            ConfirmationPage::set("OVERWRITE PATCH ?", [](int option) {
              if (option == ConfirmationPage::CONFIRM) {
                settings_->selectedPatch().paste(&patch_);
                ui_->resetAllPots();
                MessagePainter::show("PATCH PASTED");
              }
            });
            pages_->open(Pages::CONFIRMATION_PAGE);
          }
          break;
        case INIT:
          ConfirmationPage::set("INIT PATCH ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              settings_->selectedPatch().init();
              ui_->unlockAllPots();
            }
          });
          break;
        case EDIT_NAME:
         	TextInputPage::set(settings_->selectedPatch().name(), Patch::kMaxNameLength, "SET PATCH NAME");
          pages_->open(Pages::TEXT_INPUT_PAGE);
          break;
        case NEXT:
          footerOptionsOffset = 4;
          break;
        case PREV:
          footerOptionsOffset = 0;
          break;
        default:
          break;
      }
    }
  }

  void loadNewPatch() {
    settings_->loadPatch(newIndex);
    ui_->resetAllPots();
  }

  void on_encoder(int id, int state) {
    int inc = buttons_->isPressed(Buttons::SHIFT) ? state * 10 : state;
    int lastIndex = settings_->patchIndex();
    newIndex = SettingsUtils::clip(0, Settings::kNumPatches - 1, lastIndex + inc);

    if (lastIndex != newIndex) {
      if (settings_->patchHasUnsavedChanges()) {
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
    //
  }

  void draw() {
    canvas_->setFont(Font::LARGE);
    str_.write(settings_->patchIndex() + 1, " ", settings_->selectedPatch().name());
    if (settings_->patchHasUnsavedChanges()) {
      str_.append("*");
    }

    canvas_->drawText(0, 0, canvas_->width(), 54, str_.read(), Canvas::CENTER, Canvas::CENTER);


    canvas_->setFont(Font::SMALL);
    WindowPainter::draw_footer(footerOptionText, NUM_FOOTER_OPTIONS, footerOptionsOffset);
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
