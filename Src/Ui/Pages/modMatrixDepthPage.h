#ifndef ModMatrixDepthPage_h
#define ModMatrixDepthPage_h

#include "topPage.h"

namespace ModMatrixDepthPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::pages_;
  using TopPage::settings_;

  size_t dest_;
  float originalDepth_;

  enum FooterOptions {
    CONFIRM,
    CANCEL,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionsText[NUM_FOOTER_OPTIONS] = {"CONFIRM", "CANCEL"};

  void setDestination(size_t dest) {
    dest_ = dest;
    originalDepth_ = settings_->modMatrix().destinationDepth(dest_);
  }

  void init() {
  }

  void enter() {
  }

  void exit() {
  }

  void onButton(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case CONFIRM:
          pages_->close(Pages::MOD_MATRIX_DEPTH_PAGE);
          break;
        case CANCEL:
          settings_->modMatrix().setDestinationDepth(dest_, originalDepth_);
          pages_->close(Pages::MOD_MATRIX_DEPTH_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void onEncoder(int id, int state) {
    Patch& p = settings_->selectedPatch();

    bool shift = buttons_->isPressed(Buttons::SHIFT);
    float value = p.modMatrix().destinationDepth(dest_);
    p.modMatrix().setDestinationDepth(dest_, value + SettingsUtils::fInc(state, shift));
  }

  void refreshLeds() {
  }

  void draw() {
    const int w = canvas_->width() / 4;
    const int h = canvas_->height() / 2;
    const int x = (canvas_->width() - w) / 2;
    const int y = (canvas_->height() - h) / 2;
    const int rowH = h / 4;

    WindowPainter::drawBox(x, y, w, h);
    canvas_->setFont(Font::SMALL);
    canvas_->drawText(x, y + (rowH * 1), w, h - 2, ModMatrix::destination_text(dest_), Canvas::CENTER, Canvas::TOP, Canvas::WHITE);
    canvas_->drawText(x, y + (rowH * 2), w, h, settings_->modMatrix().destinationDepthText(dest_), Canvas::CENTER, Canvas::TOP, Canvas::WHITE);
 
    WindowPainter::drawFooter(footerOptionsText, NUM_FOOTER_OPTIONS);
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

};  // namespace ModMatrixDepthPage

#endif
