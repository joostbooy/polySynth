#ifndef Engine_h
#define Engine_h

#include "micros.h"
#include "midiClockEngine.h"
#include "midiEngine.h"
#include "modMatrixEngine.h"
#include "settings.h"
#include "switches.h"
#include "uart.h"
#include "voice.h"
#include "voiceEngine.h"

class Engine {
 public:
  enum State {
    STOPPED,
    RUNNING,
  };

  enum Request {
    STOP = (1 << 0),
    START = (1 << 1),
  };

  void init(Settings*, Uart*, Usb*, Dac*);
  void tick();
  void update();
  void setGate(int index, bool state);

  void addReqestBlocking(Request type) {
    addRequest(type);
    while (requests_ & type);
  }

  uint32_t processingTimeUs() {
    return processingTimeUs_;
  }

  ModMatrixEngine& modMatrixEngine() {
    return modMatrixEngine_;
  }

  MidiEngine& midiEngine() {
    return midiEngine_;
  }

  MidiClockEngine midiClockEngine() {
    return midiClockEngine_;
  }

  VoiceEngine& voiceEngine() {
    return voiceEngine_;
  }

 private:
  State state_;
  volatile uint8_t requests_ = 0x00;

  uint32_t processingTimeUs_;

  Dac* dac_;
  Settings* settings_;
  MidiEngine midiEngine_;
  VoiceEngine voiceEngine_;
  MidiClockEngine midiClockEngine_;
  ModMatrixEngine modMatrixEngine_;
  Que<MidiEngine::Event, Settings::kNumVoices * 2> noteQue_; // dont increase! we can assign 8 voices and request 8 others
  bool gate_[2];
  bool lastGate_[2];
  MidiEngine::Event gateToNote_[2];


  void start();
  void stop();
  void processRequests();
  void processMidi();
  void processGates();
  void noteOn(MidiEngine::Event& e);
  void noteOff(MidiEngine::Event& e);
  void pitchBend(MidiEngine::Event& e);
  void cc(MidiEngine::Event& e);

  void sendMidiClockStart();
  void sendMidiClockStop();
  void sendMidiClockContinue();
  void sendMidi24PPQNClock_tick();

  void addRequest(Request type) {
    uint8_t flags = requests_;
    requests_ = flags | type;
  }

  void clearRequest(Request type) {
    uint8_t flags = requests_;
    requests_ = flags & ~type;
  }
};

#endif
