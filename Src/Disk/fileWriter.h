#ifndef FileWriter_h
#define FileWriter_h

// based on westlicht's fileWriter

#include "file.h"
#include "hash.h"

class FileWriter {

public:

	bool writeOk() {
		return writeOk_;
	}

	void start(File *file, const char* path, uint32_t version) {
		buffPos_ = 0;

		file_ = file;
		writeOk_ = file_->open(path, File::WRITE | File::OPEN_ALWAYS);

		hash_.init();

		if (writeOk_) {
			write(version);
		}
	}

	void stop() {
		// send remaing buffer data
		if (writeOk_ == true && buffPos_ > 0) {
			writeOk_ = sendBuffer();
		}

		if (writeOk_) {
			uint32_t hash = hash_.read();
			writeOk_ = file_->write(&hash, sizeof(hash));
		}

		if (!writeOk_) {
			file_->close();
			return;
		}

		writeOk_ = file_->close();
	}

	template<typename T>
	void write(T &data) {
		writeBuffer(&data, sizeof(T));
		hash_.write(&data, sizeof(T));
	}

private:
	uint32_t buffPos_;

	File *file_;
	Hash hash_;
	bool writeOk_;

	static constexpr size_t kBufferSize = 512;
	uint8_t buffer_[kBufferSize];

	void writeBuffer(void *data, uint32_t size) {
		uint8_t *data_ = reinterpret_cast<uint8_t *>(data);

		while (writeOk_ && (size > 0)) {
			if (buffPos_ < kBufferSize) {
				buffer_[buffPos_++] = *data_++;
				--size;
			} else {
				writeOk_ = sendBuffer();
			}
		}
	}

	bool sendBuffer() {
		uint32_t size = buffPos_;
		buffPos_ = 0;
		return file_->write(buffer_, size);
	}

};

#endif
