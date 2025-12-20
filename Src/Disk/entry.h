#ifndef Entry_h
#define Entry_h

#include "ff.h"
#include "stringUtils.h"

// entry's corresponding to the directory set in directory.h

class Entry {

public:

	enum Filter {
		NONE,
		PROJECT,
		WAV,
		FOLDER
	};

	struct List {
		StringBuilderBase<16>name;
		uint32_t size;
		bool isDir;
	};

	static const int kMaxListSize = 8;
	List list[kMaxListSize];

	void init(DIR* dir, FILINFO* fil_info, StringBuilder* dir_path) {
		dir_ = dir;
		fil_info_ = fil_info;
		dirPath_ = dir_path;
	}

	bool isDir() {
		return fil_info_->fattrib & AM_DIR;
	}

	bool isReadOnly() {
		return fil_info_->fattrib & AM_RDO;
	}

	bool isArchive() {
		return fil_info_->fattrib & AM_ARC;
	}

	bool hasExtension(const char* extension) {
		return StringUtils::has_extension(name(), extension);
	}

	const char* name() {
		return fil_info_->fname;
	}

	uint32_t size() {
		return fil_info_->fsize;
	}

	const char* path() {
		return path_.write(dirPath_->read(), "/", name());
	}

	const char* path(const char* entry_name) {
		return path_.write(dirPath_->read(), "/", entry_name);
	}

	bool isVisible() {
		if ((fil_info_->fattrib & AM_HID) || (fil_info_->fattrib & AM_SYS)) {
			return false;
		}

		switch (filter_)
		{
		case NONE:
			return true;
		case FOLDER:
			return isDir();
		case PROJECT:
			return isDir() || hasExtension(".PRJ");
		case WAV:
			return isDir() || hasExtension(".WAV");
		default:
			break;
		}

		return false;
	}

	bool rename(const char* old_name, const char* new_name) {
		const char* old_path = buffer_.write(dirPath_->read(), "/", old_name);
		const char* new_path = path_.write(dirPath_->read(), "/", new_name);
		return f_rename(old_path, new_path) == FR_OK;
	}

	bool excists(const char* name) {
		return f_stat(path(name), fil_info_) == FR_OK;
	}

	const char* generateDuplicateName(const char* name) {
		const int kMaxTries = 100;

		buffer_.write(name);
		if (excists(buffer_.read()) == false) {
			return buffer_.read();
		}

		for (int i = 2; i < kMaxTries; ++i) {
			buffer_.write(i, "_", name);
			if (excists(buffer_.read()) == false) {
				return buffer_.read();
			}
		}
		return "\0";
	}

	void rewind() {
		f_rewinddir(dir_);
	}

	bool next() {
		return (f_readdir(dir_, fil_info_) == FR_OK) && (fil_info_->fname[0] != '\0');
	}

	bool nextVisible() {
		while (next()) {
			if (isVisible()) {
				return true;
			}
		}
		return false;
	}

	void setListFilter(Filter filter) {
		filter_ = filter;
	}

	int numVisible() {
		int num_entries = 0;

		rewind();

		while (nextVisible() && (num_entries < 32767)) {
			++num_entries;
		}
		return num_entries;
	}

	int numVisibleFiles() {
		int num_entries = 0;

		rewind();

		while (nextVisible() && (num_entries < 32767)) {
			if (isDir() == false) {
				++num_entries;
			}
		}
		return num_entries;
	}

	void makeList(int offset, int size) {
		listSize_ = 0;

		if (size > kMaxListSize) {
			size = kMaxListSize;
		}

		rewind();

		while (offset && nextVisible()) {
			--offset;
		}

		int i = 0;
		while (nextVisible() && size--) {
			list[i].name.write(fil_info_->fname);
			list[i].size = fil_info_->fsize;
			list[i].isDir = fil_info_->fattrib & AM_DIR;
			++i;
		}
		listSize_ = i;
	}

	int listSize() {
		return listSize_;
	}

	List* readList(int idx) {
		return idx >= listSize_ ? nullptr : &list[idx];
	}

	void reset() {
		listSize_ = 0;
		filter_ = NONE;
	}

private:
	DIR* dir_;
	FILINFO* fil_info_;
	StringBuilder* dirPath_;
	StringBuilderBase<64>path_;
	StringBuilderBase<64>buffer_;
	int listSize_ = 0;
	Filter filter_ = NONE;
};

#endif
