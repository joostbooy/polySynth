#ifndef WavFile_h
#define WavFile_h

#include "file.h"
#include "lookupTables.h"

class WavFile {

public:

	struct Data_chunk {
		uint32_t size;
		uint32_t offset;
	}data;

	struct Riff_chunk {
		uint32_t size;
		uint32_t format;
	}riff;

	struct Fmt_chunk {
		uint16_t audio_format;
		uint16_t num_of_channels;
		uint32_t sample_rate;
		uint32_t byte_rate;
		uint16_t block_allign;
		uint16_t bit_depth;
	}format;

	void init() {
		file_.init(&fil);
	}

	bool open(const char *path) {
		uint32_t chunk_id;
		uint32_t chunk_size;

		if (!file_.open(path)) {
			return false;
		}

		chunk_id = read_u32();
		if (chunk_id != RIFF_CHUNK_ID){
			return false;
		}

		riff.size = read_u32();
		riff.format = read_u32();
		if (riff.format != RIFF_FORMAT_WAVE) {
			return false;
		}

		while (chunk_id != DATA_CHUNK_ID) {
			if (file_.end()) {
				return false;
			}

			chunk_id = read_u32();
			chunk_size = read_u32();

			if (!parse_chunk(chunk_id, chunk_size)) {
				return false;
			}
		}

		return true;
	}

	bool read(uint8_t **data, uint32_t *num_read) {
		if (file_.read(buffer_, kBufferSize, num_read)) {
			*data = buffer_;
			return true;
		}
		return false;
	}

	bool close() {
		return file_.close();
	}

private:
	FIL fil;
	File file_;

	static const size_t kBufferSize = 512;
	//static uint8_t buffer_[kBufferSize];
	uint8_t buffer_[kBufferSize];

	static const uint32_t RIFF_CHUNK_ID		= 0x46464952;
	static const uint32_t FMT_CHUNK_ID		= 0x20746D66;
	static const uint32_t DATA_CHUNK_ID		= 0x61746164;
	static const uint32_t RIFF_FORMAT_WAVE	= 0x45564157;
	static const uint16_t WAVE_FORMAT_PCM	= 0x0001;

	uint32_t read_u32() {
		file_.read(buffer_, 4);
		return buffer_[0] | (buffer_[1] << 8) | (buffer_[2] << 16) | (buffer_[3] << 24);
	}

	bool parse_chunk(uint32_t chunk_id, uint32_t chunk_size) {
		switch (chunk_id)
		{
		case FMT_CHUNK_ID:
			if (file_.read(&format, 16) == false) {
				return false;
			}

			if (chunk_size > 16) {
				file_.advance(chunk_size - 16);
			}

			if (format.audio_format != WAVE_FORMAT_PCM) {
				return false;
			}

			if ((format.sample_rate % SAMPLE_RATE) != 0) {
				return false;
			}

			if (format.bit_depth < 16 || format.bit_depth > 32) {
				return false;
			}

			if ((format.num_of_channels < 1) || (format.num_of_channels > 2)) {
				return false;
			}
			break;
		case DATA_CHUNK_ID:
			data.size = chunk_size;
			data.offset = file_.position();
			break;
		default:
			file_.advance(chunk_size);
			break;
		}
		return true;
	}

};

#endif
