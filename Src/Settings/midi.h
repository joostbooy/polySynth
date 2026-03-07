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

  void init() {
    setPortReceive(UART);
    setChannelReceive(16);
    setGateToNote1(-1);
    setGateToNote2(-1);

    // dont use setters for these 2, becasuse they depend on each other
    keyRangeLow_ = 0;
    keyRangeHigh_ = 127;
  }

  // channel receive
  int channelReceive() {
    return channelReceive_;
  }

  void setChannelReceive(int value) {
    channelReceive_ = SettingsUtils::clip(-1, 16, value);
  }

  const char* channelReceiveText() {
    if (channelReceive() < 0) {
      return "NONE";
    } else if (channelReceive() >= 16) {
      return "ALL";
    }
    return SettingsText::intToText(channelReceive() + 1);
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
  int gateToNote1() {
    return gateToNote1_;
  }

  void setGateToNote1(int value) {
    gateToNote1_ = SettingsUtils::clip(-1, 127, value);
  }

  const char* gateToNote1Text() {
    if (gateToNote1() >= 0) {
      return SettingsText::noteToText(gateToNote1());
    } else {
      return "NONE";
    }
  }

  // Gate to note 2
  int gateToNote2() {
    return gateToNote2_;
  }

  void setGateToNote2(int value) {
    gateToNote2_ = SettingsUtils::clip(-1, 127, value);
  }

   const char* gateToNote2Text() {
    if (gateToNote2() >= 0) {
      return SettingsText::noteToText(gateToNote2());
    } else {
      return "NONE";
    }
  }

  int gateToNote(int index) {
    return index == 0 ? gateToNote1() : gateToNote2();
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(channelReceive_);
    fileWriter.write(portReceive_);
    fileWriter.write(keyRangeLow_);
    fileWriter.write(keyRangeHigh_);
    fileWriter.write(gateToNote1_);
    fileWriter.write(gateToNote2_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(channelReceive_);
    fileReader.read(portReceive_);
    fileReader.read(keyRangeLow_);
    fileReader.read(keyRangeHigh_);
    fileReader.read(gateToNote1_);
    fileReader.read(gateToNote2_);
  }

  void paste(Midi* midi) {
    channelReceive_ = midi->channelReceive();
    portReceive_ = midi->portReceive();
    keyRangeLow_ = midi->keyRangeLow();
    keyRangeHigh_ = midi->keyRangeHigh();
    gateToNote1_ = midi->gateToNote1();
    gateToNote2_ = midi->gateToNote2();
  }

  void writeHash(Hash& hash) {
    hash.write(channelReceive_);
    hash.write(portReceive_);
    hash.write(keyRangeLow_);
    hash.write(keyRangeHigh_);
    hash.write(gateToNote1_);
    hash.write(gateToNote2_);
  }

 private:
  int gateToNote1_;
  int gateToNote2_;
  int channelReceive_;
  int portReceive_;
  int keyRangeLow_;
  int keyRangeHigh_;
};

#endif
