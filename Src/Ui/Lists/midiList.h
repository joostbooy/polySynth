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
    MATRIX_CC_A,
    MATRIX_CC_B,
    MATRIX_CC_C,
    MATRIX_CC_D,

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
      case MATRIX_CC_A:     return "MATRIX CC A";
      case MATRIX_CC_B:     return "MATRIX CC B";
      case MATRIX_CC_C:     return "MATRIX CC C";
      case MATRIX_CC_D:     return "MATRIX CC D";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Midi& midi = settings_->midi();
    ModMatrix& modMatrix = settings_->modMatrix();

    switch (item) {
      case BPM:             return midi.bpmText();
      case CLOCK_SOURCE:    return midi.clockSourceText();
      case SEND_CLOCK_UART: return midi.sendClock_text(Midi::UART);
      case SEND_CLOCK_USB:  return midi.sendClock_text(Midi::USB);
      case KEY_RANGE_LOW:   return midi.keyRangeLowText();
      case KEY_RANGE_HIGH:  return midi.keyRangeHighText();
      case PORT_RECEIVE:    return midi.portReceiveText();
      case CHANNEL_RECEIVE: return midi.channelReceiveText();
      case MATRIX_CC_A:     return modMatrix.midiCcNumberText(0);
      case MATRIX_CC_B:     return modMatrix.midiCcNumberText(1);
      case MATRIX_CC_C:     return modMatrix.midiCcNumberText(2);
      case MATRIX_CC_D:     return modMatrix.midiCcNumberText(3);
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Midi& midi = settings_->midi();
    ModMatrix& modMatrix = settings_->modMatrix();

    switch (item) {
      case BPM:
        if (shifted) {
          midi.setBpmFractional(midi.bpmFractional() + inc);
        } else {
          midi.setBpm(midi.bpm() + inc);
        }
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
        engine_->addReqestBlocking(Engine::STOP);
        midi.setKeyRangeLow(midi.keyRangeLow() + (shifted ? inc * 12 : inc));
        engine_->addReqestBlocking(Engine::START);
        break;
      case KEY_RANGE_HIGH:
        engine_->addReqestBlocking(Engine::STOP);
        midi.setKeyRangeHigh(midi.keyRangeHigh() + (shifted ? inc * 12 : inc));
        engine_->addReqestBlocking(Engine::START);
        break;
      case PORT_RECEIVE:
        engine_->addReqestBlocking(Engine::STOP);
        midi.setPortReceive(midi.portReceive() + inc);
        engine_->addReqestBlocking(Engine::START);
        break;
      case CHANNEL_RECEIVE:
        engine_->addReqestBlocking(Engine::STOP);
        midi.setChannelReceive(midi.channelReceive() + inc);
        engine_->addReqestBlocking(Engine::START);
        break;
      case MATRIX_CC_A:
        modMatrix.setMidiCcNumber(0, modMatrix.midiCcNumber(0) + (shifted ? inc * 10 : inc));
        break;
      case MATRIX_CC_B:
        modMatrix.setMidiCcNumber(1, modMatrix.midiCcNumber(1) + (shifted ? inc * 10 : inc));
        break;
      case MATRIX_CC_C:
        modMatrix.setMidiCcNumber(2, modMatrix.midiCcNumber(2) + (shifted ? inc * 10 : inc));
        break;
      case MATRIX_CC_D:
        modMatrix.setMidiCcNumber(3, modMatrix.midiCcNumber(3) + (shifted ? inc * 10 : inc));
        break;
      default:
        break;
    }
  }

 private:
};

#endif
