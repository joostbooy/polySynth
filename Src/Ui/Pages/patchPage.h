#ifndef PatchPage_h
#define PatchPage_h

#include "patch.h"

namespace PatchPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::pages_;
  using TopPage::settings_;
  using TopPage::str_;
  using TopPage::ui_;

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
    NEXT_2,
    PREV_2,
    DISPLAY_OFF,
    AUDITION,

    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionText[NUM_FOOTER_OPTIONS] = {"SAVE", "INIT", "COPY", ">", "<", "PASTE", "EDIT NAME", ">", "<", "DISPLAY OFF", "AUDITION"};

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
    engine_->addReqestBlocking(Engine::STOP_AUDITION);
  }

  void onButton(int id, int state) {
    switch (buttons_->toFunction(id, footerOptionsOffset)) {
      case SAVE:
        if (state) {
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
        }
        break;
      case COPY:
        if (state) {
          patch_.paste(&settings_->selectedPatch());
          pasteable_ = true;
          MessagePainter::show("PATCH COPIED");
        }
        break;
      case PASTE:
        if (state && pasteable_) {
          ConfirmationPage::set("OVERWRITE PATCH ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              engine_->addReqestBlocking(Engine::STOP);

              settings_->selectedPatch().paste(&patch_);
              ui_->resetAllPots();

              engine_->addReqestBlocking(Engine::START);
              MessagePainter::show("PATCH PASTED");
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
        }
        break;
      case INIT:
        if (state) {
          ConfirmationPage::set("INIT PATCH ?", [](int option) {
            if (option == ConfirmationPage::CONFIRM) {
              engine_->addReqestBlocking(Engine::STOP);

              settings_->selectedPatch().init();
              ui_->unlockAllPots();

              engine_->addReqestBlocking(Engine::START);
            }
          });
          pages_->open(Pages::CONFIRMATION_PAGE);
        }
        break;
      case EDIT_NAME:
        if (state) {
          TextInputPage::set(settings_->selectedPatch().name(), Patch::kMaxNameLength, "SET PATCH NAME");
          pages_->open(Pages::TEXT_INPUT_PAGE);
        }
        break;
      case AUDITION:
        if (state) {
          engine_->addReqestBlocking(Engine::START_AUDITION);
        } else {
          engine_->addReqestBlocking(Engine::STOP_AUDITION);
        }
        break;
      case DISPLAY_OFF:
        if (!state) {
          ui_->displayOff();
        }
        break;
      case NEXT:
        if (state) {
          footerOptionsOffset = 4;
        }
        break;
      case PREV:
        // dont switch footer options if audition is pressed
        if (state == 1 && buttons_->isPressed(Buttons::DISPLAY_D) == false) {
          footerOptionsOffset = 0;
        }
        break;
      case NEXT_2:
        if (state) {
          footerOptionsOffset = 8;
        }
        break;
      case PREV_2:
        if (state) {
          footerOptionsOffset = 4;
        }
        break;
      default:
        break;
    }
  }

  void loadNewPatch() {
    engine_->addReqestBlocking(Engine::STOP);

    settings_->loadPatch(newIndex);
    ui_->resetAllPots();

    engine_->addReqestBlocking(Engine::START);
  }

  void onEncoder(int id, int state) {
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

  void refreshLeds() {
  }

  void draw() {
    canvas_->setFont(Font::SMALL);

    str_.write(settings_->patchIndex() + 1, " ", settings_->selectedPatch().name());
    canvas_->drawText(0, 0, canvas_->width(), 54, str_.read(), Canvas::CENTER, Canvas::CENTER);

    WindowPainter::drawFooter(footerOptionText, NUM_FOOTER_OPTIONS, footerOptionsOffset);
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
};  // namespace PatchPage

#endif
