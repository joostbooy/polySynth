#ifndef FileWriter_h
#define FileWriter_h

// based on westlicht's fileWriter

#include "eeprom.h"
#include "hash.h"

class FileWriter {
 public:
  void init(Eeprom* eeprom) {
    eeprom_ = eeprom;
  }

  void start(size_t address, size_t version) {
    address_ = address;
    buffPos_ = 0;
    hash_.init();
    write(version);
  }

  void stop() {
    size_t hash = hash_.read();
    writeBuffer(&hash, sizeof(hash));
    if (buffPos_ > 0) {
      sendBuffer();
    }
  }

  template <typename T>
  void write(T& data) {
    writeBuffer(&data, sizeof(T));
    hash_.write(&data, sizeof(T));
  }

  bool writeOk() {
	return true;
  }

 private:
  Hash hash_;
  Eeprom* eeprom_;
  static constexpr size_t kBufferSize = 128;
  size_t address_;
  size_t buffPos_;
  uint8_t buffer_[kBufferSize];

  void writeBuffer(void* data, size_t size) {
    uint8_t* data_ = reinterpret_cast<uint8_t*>(data);

    while (size > 0) {
      if (buffPos_ < kBufferSize) {
        buffer_[buffPos_++] = *data_++;
        --size;
      } else {
        sendBuffer();
      }
    }
  }

  void sendBuffer() {
    eeprom_->write(address_, buffer_, buffPos_);
    address_ += buffPos_;
    buffPos_ = 0;
  }
};

#endif
