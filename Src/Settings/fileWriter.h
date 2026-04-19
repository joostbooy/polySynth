#ifndef FileWriter_h
#define FileWriter_h

#include "eeprom.h"
#include "hash.h"

class FileWriter {
 public:
  void init(Eeprom* eeprom) {
    eeprom_ = eeprom;
  }

  bool writeOk() {
    return writeOk_;
  }

  void start(size_t address, size_t version) {
    address_ = address;
    startingAdress_ = address;
    buffPos_ = 0;
    hash_.init();
    write(version);
  }

  void stop() {
    uint32_t hash = hash_.read();
    writeBuffer(&hash, sizeof(hash));
    sendBuffer();
    writeOk_ = checkHash();
  }

  template <typename T>
  void write(T& data) {
    writeBuffer(&data, sizeof(T));
    hash_.write(&data, sizeof(T));
  }

 private:
  Hash hash_;
  Eeprom* eeprom_;
  static constexpr size_t kBufferSize = 512;
  size_t address_;
  size_t startingAdress_;
  size_t buffPos_;
  bool writeOk_;
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
    size_t size = buffPos_;
    eeprom_->write(address_, buffer_, size);
    address_ += size;
    buffPos_ = 0;
  }

  bool checkHash() {
    Hash hash;

    hash.init();
    
    size_t size = address_ - startingAdress_ - 4;  // minus 4 hash bytes at the end
    address_ = startingAdress_;

    while (size >= kBufferSize) {
      eeprom_->read(address_, buffer_, kBufferSize);
      hash.write(buffer_, kBufferSize);
      address_ += kBufferSize;
      size -= kBufferSize;
    }

    eeprom_->read(address_, buffer_, size);
    hash.write(buffer_, size);

    return hash.read() == hash_.read();
  }
};

#endif
