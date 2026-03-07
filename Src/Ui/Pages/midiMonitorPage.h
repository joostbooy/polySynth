#ifndef MidiMonitorPage_h
#define MidiMonitorPage_h

#include "midiEngine.h"

namespace MidiMonitorPage {

  using TopPage::buttons_;
  using TopPage::engine_;
  using TopPage::pages_;
  using TopPage::str_;
  using TopPage::canvas_;

  enum FooterOptions {
    START_STOP,
	SHOW_HIDE_CLOCK,
    CLEAR,
    CLOSE,
    NUM_FOOTER_OPTIONS,
  };

  bool running_;
  bool showClock_;

  const char* footerText_[NUM_FOOTER_OPTIONS] = {"START_STOP", "SHOW_HIDE_CLOCK", "CLEAR", "CLOSE"};

  void clear() {
    engine_->midiEngine().clearMonitor();
    TextBufferPainter::clear();
  }

  void init() {
    running_ = true;
	showClock_ = true;
  }

  void enter() {
    engine_->midiEngine().clearMonitor();
  }

  void exit() {
  }

  void onButton(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case START_STOP:
          running_ ^= 1;
          break;
        case SHOW_HIDE_CLOCK:
          showClock_ ^= 1;
          break;
        case CLEAR:
          clear();
          break;
        case CLOSE:
          pages_->close(Pages::MIDI_MONITOR_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void onEncoder(int id, int state) {
  }

  void refreshLeds() {
  }

  const char* channelText(MidiEngine::Event& e) {
    return SettingsText::str.write("CHN ", (e.message & 0x0F) + 1);
  }

  const char* messageTypeText(MidiEngine::Event& e) {
    switch (engine_->midiEngine().readMessage(e)) {
      case MidiEngine::NOTE_OFF:          return "NOTE OFF ";
      case MidiEngine::NOTE_ON:           return "NOTE ON ";
      case MidiEngine::CONTROLLER_CHANGE: return "CC ";
      case MidiEngine::PITCH_BEND:        return "BEND ";
      case MidiEngine::CLOCK_PULSE:       return "CLOCK PULSE ";
      case MidiEngine::CLOCK_START:       return "CLOCK START ";
      case MidiEngine::CLOCK_STOP:        return "CLOCK STOP ";
      case MidiEngine::CLOCK_CONTINUE:    return "CLOCK CONTINUE ";
      default:
        break;
    }
    return "";
  }

  const char* dataText(MidiEngine::Event& e, int byte) {
    if (byte == 0) {
      switch (e.message & 0xF0) {
        case MidiEngine::NOTE_OFF:		    	return SettingsText::noteToText(e.data[0]);
        case MidiEngine::NOTE_ON:			      return SettingsText::noteToText(e.data[0]);
        case MidiEngine::CONTROLLER_CHANGE:	return SettingsText::intToText(e.data[0]);
        case MidiEngine::PITCH_BEND:		    return SettingsText::intToText(engine_->midiEngine().read14Bit(e));
        default:
          break;
      }
    } else {
      switch (e.message & 0xF0) {
        case MidiEngine::NOTE_OFF:		    	return SettingsText::intToText(e.data[1]);
        case MidiEngine::NOTE_ON:		        return SettingsText::intToText(e.data[1]);
        case MidiEngine::CONTROLLER_CHANGE:	return SettingsText::intToText(e.data[1]);
        case MidiEngine::PITCH_BEND:	    	return "";
        default:
          break;
      }
    }
    return "";
  }

  void writeMessage(MidiEngine::Event& e) {
    if (engine_->midiEngine().isClockMessage(e)) {
      if (showClock_) {
        str_.write(messageTypeText(e));
        TextBufferPainter::write(str_.read());
      }
    } else {
      str_.write(messageTypeText(e), dataText(e, 0), " ", dataText(e, 1), " ", channelText(e));
      TextBufferPainter::write(str_.read());
    }
  }

  void draw() {
    MidiEngine::Event e;

    canvas_->clear();

    footerText_[START_STOP] = running_ ? "STOP" : "START";
    footerText_[SHOW_HIDE_CLOCK] = showClock_ ? "HIDE CLOCK" : "SHOW CLOCK";

    if (running_) {
      while (engine_->midiEngine().pullMonitor(e)) {
        writeMessage(e);
      }
    }

    TextBufferPainter::draw();
    WindowPainter::drawFooter(footerText_, NUM_FOOTER_OPTIONS);
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

};  // namespace MidiMonitorPage

#endif
