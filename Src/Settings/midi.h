#ifndef Midi_h
#define Midi_h

#include "fileReader.h"
#include "fileWriter.h"
#include "lookupTables.h"
#include "settingsText.h"
#include "settingsUtils.h"

class Midi {
 public:
  enum Port {
    UART,
    USB,

    NUM_PORTS
  };

  static const char* portText(int port) {
    switch (port) {
      case UART:  return "UART";
      case USB:   return "USB";
      default:
        break;
    }
    return nullptr;
  }

  enum ClockSource {
    EXTERNAL = NUM_PORTS - 1,
    INTERNAL,

    NUM_CLOCK_SOURCES
  };

  static const char* clockSourceText(int value) {
    if (value <= EXTERNAL) {
      return Midi::portText(value);
    } else {
      return "INTERNAL";
    }
  }

  void init() {
    setBpm(120);
    setBpmFractional(0);
    setClockSource(INTERNAL);

    // dont use setters for these 2, becasuse they depend on each other
    keyRangeLow_ = 0;
    keyRangeHigh_ = 127;

    for (int i = 0; i < NUM_PORTS; ++i) {
      setSendClock(i, true);
    }
    setPortReceive(UART);
    setChannelReceive(16);
    setGateToNote1(60);
    setGateToNote2(60);
  }

  // bpm
  uint16_t bpm() {
    return bpm_;
  }

  void setBpm(int value) {
    bpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, value);
  }

  // bpm fractional
  uint16_t bpmFractional() {
    return bpmFractional_;
  }

  void setBpmFractional(int value) {
    bpmFractional_ = SettingsUtils::clip(0, MAX_BPM_FRACTIONAL, value);
  }

  const char* bpmText() {
    return SettingsText::str.write(bpm(), ".", bpmFractional());
  }

  // clock source
  uint8_t clockSource() {
    return clockSource_;
  }

  void setClockSource(int value) {
    clockSource_ = SettingsUtils::clip(0, NUM_CLOCK_SOURCES - 1, value);
  }

  const char* clockSourceText() {
    return clockSourceText(clockSource());
  }

  // clock source
  bool sendClock(int port) {
    return sendClock_[port];
  }

  void setSendClock(int port, bool value) {
    sendClock_[port] = value;
  }

  const char* sendClock_text(int port) {
    return SettingsText::boolToOnOff(sendClock(port));
  }

  // channel receive
  int channelReceive() {
    return channelReceive_;
  }

  void setChannelReceive(int value) {
    channelReceive_ = SettingsUtils::clip(0, 16, value);
  }

  const char* channelReceiveText() {
    return SettingsText::midiChannelText(channelReceive());
  }

  // port receive
  int portReceive() {
    return portReceive_;
  }

  void setPortReceive(int value) {
    portReceive_ = SettingsUtils::clip(0, NUM_PORTS - 1, value);
  }

  const char* portReceiveText() {
    return portText(portReceive());
  }

  // key range low
  int keyRangeLow() {
    return keyRangeLow_;
  }

  void setKeyRangeLow(int value) {
    keyRangeLow_ = SettingsUtils::clip(0, keyRangeHigh(), value);
  }

  const char* keyRangeLowText() {
    return SettingsText::noteToText(keyRangeLow());
  }

  // key range low
  int keyRangeHigh() {
    return keyRangeHigh_;
  }

  void setKeyRangeHigh(int value) {
    keyRangeLow_ = SettingsUtils::clip(keyRangeLow(), 127, value);
  }

  const char* keyRangeHighText() {
    return SettingsText::noteToText(keyRangeHigh());
  }

  // Gate to note 1
  uint8_t gateToNote1() {
    return gateToNote1_;
  }

  void setGateToNote1(int value) {
    gateToNote1_ = SettingsUtils::clip(0, 127, value);
  }

  const char* gateToNote1Text() {
    return SettingsText::noteToText(gateToNote1());
  }

  // Gate to note 2
  uint8_t gateToNote2() {
    return gateToNote2_;
  }

  void setGateToNote2(int value) {
    gateToNote2_ = SettingsUtils::clip(0, 127, value);
  }

  const char* gateToNote2Text() {
    return SettingsText::noteToText(gateToNote2());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(bpm_);
    fileWriter.write(bpmFractional_);
    fileWriter.write(clockSource_);
    fileWriter.write(channelReceive_);
    fileWriter.write(portReceive_);
    fileWriter.write(keyRangeLow_);
    fileWriter.write(keyRangeHigh_);
    fileWriter.write(gateToNote1_);
    fileWriter.write(gateToNote2_);

    for (int i = 0; i < NUM_PORTS; ++i) {
      fileWriter.write(sendClock_[i]);
    }
  }

  void load(FileReader& fileReader) {
    fileReader.read(bpm_);
    fileReader.read(bpmFractional_);
    fileReader.read(clockSource_);
    fileReader.read(channelReceive_);
    fileReader.read(portReceive_);
    fileReader.read(keyRangeLow_);
    fileReader.read(keyRangeHigh_);
    fileReader.read(gateToNote1_);
    fileReader.read(gateToNote2_);

    for (int i = 0; i < NUM_PORTS; ++i) {
      fileReader.read(sendClock_[i]);
    }
  }

  void paste(Midi* midi) {
    bpm_ = midi->bpm();
    bpmFractional_ = midi->bpmFractional();
    clockSource_ = midi->clockSource();
    channelReceive_ = midi->channelReceive();
    portReceive_ = midi->portReceive();
    keyRangeLow_ = midi->keyRangeLow();
    keyRangeHigh_ = midi->keyRangeHigh();
    gateToNote1_ = midi->gateToNote1();
    gateToNote2_ = midi->gateToNote2();

    for (int i = 0; i < NUM_PORTS; ++i) {
      sendClock_[i] = midi->sendClock(i);
    }
  }

  void writeHash(Hash& hash) {
    hash.write(bpm_);
    hash.write(bpmFractional_);
    hash.write(clockSource_);
    hash.write(channelReceive_);
    hash.write(portReceive_);
    hash.write(keyRangeLow_);
    hash.write(keyRangeHigh_);
    hash.write(gateToNote1_);
    hash.write(gateToNote2_);

    for (int i = 0; i < NUM_PORTS; ++i) {
      hash.write(sendClock_[i]);
    }
  }

 private:
  uint16_t bpm_;
  uint16_t bpmFractional_;
  uint8_t clockSource_;
  uint8_t gateToNote1_;
  uint8_t gateToNote2_;
  int channelReceive_;
  int portReceive_;
  int keyRangeLow_;
  int keyRangeHigh_;
  bool sendClock_[NUM_PORTS];
};

#endif
