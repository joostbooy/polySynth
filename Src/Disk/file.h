#ifndef File_h
#define File_h

#include "ff.h"
#include "stringBuilder.h"

class File {

public:

	enum Mode {
		OPEN_EXISTING	= 0x00,
		READ			= 0x01,
		WRITE			= 0x02,
		CREATE_NEW		= 0x04,	// Creates a new file, the function fails if the file is existing.
		CREATE_ALWAYS	= 0x08,	// Creates a new file, if the file is existing, it will be truncated and overwritten.
		OPEN_ALWAYS		= 0x10,	// Opens the file if it is existing. If not, a new file will be created.
		OPEN_APPEND		= 0x30	// Same as OPEN_ALWAYS except the read/write pointer is set end of the file.
	};

	void init(FIL* fil) {
		fil_ = fil;
	}

		bool open(const char* path_, uint8_t mode = READ) {
			return f_open(fil_, path_, mode) == FR_OK;
		}

//	bool open(const char* path_, uint8_t mode = READ) {
//		last_res_ = f_open(fil_, path_, mode);
//		return last_res_ == FR_OK;
//	}

	bool open(const char* dir, const char* name, uint8_t mode = READ) {
		return f_open(fil_, path.write(dir, "/", name), mode) == FR_OK;
	}

		bool close() {
			return f_close(fil_) == FR_OK;
		}

//	bool close() {
//		FRESULT last_res_ = f_close(fil_);
//		return last_res_ == FR_OK;
//	}


	bool sync() {
		return f_sync(fil_) == FR_OK;
	}

	//	bool write(const void *data, uint32_t size, uint32_t* num_written = nullptr) {
	//		UINT bw;
	//		FRESULT res = f_write(fil_, data, size, &bw);
	//		if (num_written) {
	//			*num_written = bw;
	//		}
	//		return (bw > 0) && (res == FR_OK);
	//	}

	bool write(const void *data, uint32_t size, uint32_t* num_written = nullptr) {
		UINT bw;
		last_res_ = f_write(fil_, data, size, &bw);
		if (num_written) {
			*num_written = bw;
		}
		return (bw > 0) && (last_res_ == FR_OK);
	}

	bool read(void *data, uint32_t size, uint32_t* num_read = nullptr) {
		UINT br;
		FRESULT res = f_read(fil_, data, size, &br);
		if (num_read) {
			*num_read = br;
		}
		return (br > 0) && (res == FR_OK);
	}

	bool end() {
		return f_eof(fil_);
	}

	uint32_t position() {
		return f_tell(fil_);
	}

	bool seek(uint32_t offset) {
		return f_lseek(fil_, offset) == FR_OK;
	}

	bool advance(uint32_t num_bytes) {
		uint32_t pos = f_tell(fil_) + num_bytes;
		return f_lseek(fil_, pos) == FR_OK;
	}

	bool rewind() {
		return f_rewind(fil_) == FR_OK;
	}



	FRESULT last_res() {
		return last_res_;
	}

private:
	FIL* fil_;
	StringBuilderBase<64>path;

	FRESULT last_res_;
};

#endif
