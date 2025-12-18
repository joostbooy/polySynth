#ifndef Disk_h
#define Disk_h

#include "sdio.h"
#include "ff.h"
#include "directory.h"
#include "entry.h"
#include "file.h"
#include "diskio.h"

class Disk {

public:

	static Sdio *sdio_;

	Directory &directory() {
		return directory_;
	}

	Entry &entry() {
		return entry_;
	}

	File &file() {
		return file_;
	}

	void init(Sdio *sdio) {
		sdio_ = sdio;

		const int root = 0;

		directory_.init(&fdir, root);
		entry_.init(&fdir, &fil_info, directory_.path_ptr());
		file_.init(&fil);

		mount();
	}

	bool mount() {
		if (!mounted()) {
			return f_mount(&fs, directory_.root(), 1) == FR_OK;
		} else {
			return true;
		}
	}

	bool mounted() {
		return sdio_->initialised();
	}

	void reset() {
		directory_.reset();
		directory_.close();
		entry_.reset();
		file_.close();
	}

	void available_kb(uint32_t *total_blocks, uint32_t *free_blocks) {
		FATFS* fs_ptr = &fs;
		uint32_t free_clusters;

		if (f_getfree(directory_.root(), &free_clusters, &fs_ptr) == FR_OK) {
			*total_blocks = ((fs_ptr->n_fatent - 2) * fs_ptr->csize) / 2;
			*free_blocks = (free_clusters * fs_ptr->csize) / 2;
		}
	}

private:
	FATFS fs;
	DIR fdir;
	FIL fil;
	FILINFO fil_info;

	Directory directory_;
	Entry entry_;
	File file_;
};

#endif
