#ifndef MidiEngine_h
#define MidiEngine_h

#include "midi.h"
#include "que.h"
#include "uart.h"
#include "usb.h"

class MidiEngine {
 public:
  enum MessageType {
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    KEY_PRESSURE = 0xA0,
    CONTROLLER_CHANGE = 0xB0,
    PROGRAM_CHANGE = 0xC0,
    CHANNEL_PRESSURE = 0xD0,
    PITCH_BEND = 0xE0,
    CLOCK_PULSE = 0xF8,
    CLOCK_START = 0xFA,
    CLOCK_CONTINUE = 0xFB,
    CLOCK_STOP = 0xFC,
    SONG_POSITION = 0xF2,
  };

  struct Event {
    uint8_t port;
    uint8_t message;
    uint8_t data[2];
  };

  Que<uint8_t, 32> inputQue;
  Que<uint8_t, 32> outputQue[Midi::NUM_PORTS];
  Que<uint8_t, 8> clockOutputQue[Midi::NUM_PORTS];

  void init(Uart* uart, Usb* usb, Midi* midi) {
    usb_ = usb;
    uart_ = uart;
    midi_ = midi;

    lastReceived_ = 0;
    numDataBytes_ = 0;
    lastReceivedReady_ = false;
  }

  void poll() {
    if (uart_->readable() && inputQue.writeable()) {
      lastReceived_ = uart_->read();
      inputQue.write(lastReceived_);
      lastReceivedReady_ = true;
    }

    if (uart_->writeable()) {
      if (clockOutputQue[Midi::UART].readable()) {
        uart_->write(clockOutputQue[Midi::UART].read());
      } else if (outputQue[Midi::UART].readable()) {
        uart_->write(outputQue[Midi::UART].read());
      }
    }
  }

  void getLastReceived(uint8_t* ptr) {
      *ptr = lastReceivedReady_ ? lastReceived_ : 0x00;
      lastReceivedReady_ = false;
  }

  bool pull(Event& e) {
    while (inputQue.readable()) {
      if (parse(inputQue.read())) {
        e = event_;
        return true;
      }
    }
    return false;
  }

  bool messageAccepted(Event& e) {
    return (e.message & 0x0F) == midi_->channelReceive() && e.port == midi_->portReceive();
  }

  bool withinKeyRange(Event& e) {
    return e.data[0] >= midi_->keyRangeLow() && e.data[0] <= midi_->keyRangeHigh();
  }

  static inline uint16_t read14Bit(Event& e) {
    return (e.data[0] & 0x7F) | (e.data[1] << 7);
  }

  static inline bool isClockMessage(Event& e) {
    return e.message >= 0xF8;
  }

  static inline uint8_t readMessage(Event& e) {
    return isClockMessage(e) ? e.message : e.message & 0xF0;
  }

  bool writeOutput(Event& e) {
    uint8_t size = e.message != lastMessage_[e.port] ? 3 : 2;

    if (outputQue[e.port].available_size() >= size) {
      if (size == 3) {
        outputQue[e.port].write(e.message);
        lastMessage_[e.port] = e.message;
      }
      outputQue[e.port].write(e.data[0]);
      outputQue[e.port].write(e.data[1]);

      return true;
    }

    return false;
  }

  void writeClock(int port, uint8_t message) {
      if (clockOutputQue[port].writeable()) {
        clockOutputQue[port].write(message);
    }
  }

 private:
  Usb* usb_;
  Uart* uart_;
  Midi* midi_;

  Event event_;
  uint8_t lastReceived_;
  uint8_t lastMessage_[Midi::NUM_PORTS];
  uint8_t numDataBytes_;

  bool lastReceivedReady_;

  bool parse(uint8_t reading) {
    if (reading >= 0x80) {
      event_.message = reading;
      numDataBytes_ = 0;
    } else {
      event_.data[numDataBytes_] = reading;
      ++numDataBytes_;

      if (numDataBytes_ >= 2) {
        numDataBytes_ = 0;
        return true;
      }
    }
    return false;
  }
};

#endif
