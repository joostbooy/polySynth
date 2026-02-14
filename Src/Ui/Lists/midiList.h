#ifndef MidiList_h
#define MidiList_h

#include "settings.h"
#include "settingsText.h"

class MidiList : public SettingsList {
 public:
  enum Item {
    BPM,
    CLOCK_SOURCE,
    SEND_CLOCK_UART,
    SEND_CLOCK_USB,
    KEY_RANGE_LOW,
    KEY_RANGE_HIGH,
    PORT_RECEIVE,
    CHANNEL_RECEIVE,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case BPM:             return "BPM";
      case CLOCK_SOURCE:    return "CLOCK SOURCE";
      case SEND_CLOCK_UART: return "SEND CLOCK UART";
      case SEND_CLOCK_USB:  return "SEND CLOCK USB";
      case KEY_RANGE_LOW:   return "KEY RANGE LOW";
      case KEY_RANGE_HIGH:  return "KEY RANGE HIGH";
      case PORT_RECEIVE:    return "PORT RECEIVE";
      case CHANNEL_RECEIVE: return "CHANNEL RECEIVE";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Midi& midi = settings_->midi();

    switch (item) {
      case BPM:             return midi.bpmText();
      case CLOCK_SOURCE:    return midi.clockSourceText();
      case SEND_CLOCK_UART: return midi.sendClock_text(Midi::UART);
      case SEND_CLOCK_USB:  return midi.sendClock_text(Midi::USB);
      case KEY_RANGE_LOW:   return midi.keyRangeLowText();
      case KEY_RANGE_HIGH:  return midi.keyRangeHighText();
      case PORT_RECEIVE:    return midi.portReceiveText();
      case CHANNEL_RECEIVE: return midi.channelReceiveText();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Midi& midi = settings_->midi();

    switch (item) {
      case BPM:
        midi.setBpm(midi.bpm() + (inc * (shifted ? 10 : 1)));
        break;
      case CLOCK_SOURCE:
        midi.setClockSource(midi.clockSource() + inc);
        break;
      case SEND_CLOCK_UART:
        midi.setSendClock(Midi::UART, inc > 0);
        break;
      case SEND_CLOCK_USB:
        midi.setSendClock(Midi::USB, inc > 0);
        break;
      case KEY_RANGE_LOW:
        midi.setKeyRangeLow(midi.keyRangeLow() + inc);
        break;
      case KEY_RANGE_HIGH:
        midi.setKeyRangeHigh(midi.keyRangeHigh() + inc);
        break;
      case PORT_RECEIVE:
        midi.setPortReceive(midi.portReceive() + inc);
        break;
      case CHANNEL_RECEIVE:
        midi.setChannelReceive(midi.channelReceive() + inc);
        break;
      default:
        break;
    }
  }

 private:
};

#endif
