#ifndef FileReader_h
#define FileReader_h

#include "eeprom.h"
#include "hash.h"

class FileReader {
 public:
  bool readOk() {
    return readOk_;
  }

  size_t version() {
    return version_;
  }

  void init(Eeprom* eeprom) {
    eeprom_ = eeprom;
  }

  void start(size_t address) {
    address_ = address;
    numRead_ = 0;
    hash_.init();

    fillBuffer();
    read(version_);
  }

  void stop() {
    uint32_t hash = 0;
    readBuffer(&hash, sizeof(hash));
    readOk_ = (hash == hash_.read());
  }

  template <typename T>
  void read(T& data) {
    readBuffer(&data, sizeof(T));
    hash_.write(&data, sizeof(T));
  }

  template <typename ReadT, typename T>
  void readAs(T& data) {
    ReadT temp = 0;
    read(temp);
    data = temp;
  }

  void skip(uint32_t size) {
    uint8_t dummy = 0;
    while (size--) {
      read(dummy);
    }
  }

 private:
  bool readOk_;
  Eeprom* eeprom_;
  uint32_t numRead_ = 0;
  size_t version_ = 0;
  size_t address_;
  Hash hash_;

  static constexpr size_t kBufferSize = 512;
  uint8_t buffer_[kBufferSize];

  void readBuffer(void* data, uint32_t size) {
    uint8_t* data_ = reinterpret_cast<uint8_t*>(data);

    while (size > 0) {
      if (numRead_ < kBufferSize) {
        *data_++ = buffer_[numRead_++];
        --size;
      } else {
        fillBuffer();
      }
    }
  }

  void fillBuffer() {
    numRead_ = 0;
    eeprom_->read(address_, buffer_, kBufferSize);
    address_ += kBufferSize;
  }
};

#endif
