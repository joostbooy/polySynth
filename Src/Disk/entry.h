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
		bool is_dir;
	};

	static const int kMaxListSize = 8;
	List list[kMaxListSize];

	void init(DIR* dir, FILINFO* fil_info, StringBuilder* dir_path) {
		dir_ = dir;
		fil_info_ = fil_info;
		dir_path_ = dir_path;
	}

	bool is_dir() {
		return fil_info_->fattrib & AM_DIR;
	}

	bool is_read_only() {
		return fil_info_->fattrib & AM_RDO;
	}

	bool is_archive() {
		return fil_info_->fattrib & AM_ARC;
	}

	bool has_extension(const char* extension) {
		return StringUtils::has_extension(name(), extension);
	}

	const char* name() {
		return fil_info_->fname;
	}

	uint32_t size() {
		return fil_info_->fsize;
	}

	const char* path() {
		return path_.write(dir_path_->read(), "/", name());
	}

	const char* path(const char* entry_name) {
		return path_.write(dir_path_->read(), "/", entry_name);
	}

	bool is_visible() {
		if ((fil_info_->fattrib & AM_HID) || (fil_info_->fattrib & AM_SYS)) {
			return false;
		}

		switch (filter_)
		{
		case NONE:
			return true;
		case FOLDER:
			return is_dir();
		case PROJECT:
			return is_dir() || has_extension(".PRJ");
		case WAV:
			return is_dir() || has_extension(".WAV");
		default:
			break;
		}

		return false;
	}

	bool rename(const char* old_name, const char* new_name) {
		const char* old_path = buffer_.write(dir_path_->read(), "/", old_name);
		const char* new_path = path_.write(dir_path_->read(), "/", new_name);
		return f_rename(old_path, new_path) == FR_OK;
	}

	bool excists(const char* name) {
		return f_stat(path(name), fil_info_) == FR_OK;
	}

	const char* generate_duplicate_name(const char* name) {
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

	bool next_visible() {
		while (next()) {
			if (is_visible()) {
				return true;
			}
		}
		return false;
	}

	void set_list_filter(Filter filter) {
		filter_ = filter;
	}

	int num_visible() {
		int num_entries = 0;

		rewind();

		while (next_visible() && (num_entries < 32767)) {
			++num_entries;
		}
		return num_entries;
	}

	int num_visible_files() {
		int num_entries = 0;

		rewind();

		while (next_visible() && (num_entries < 32767)) {
			if (is_dir() == false) {
				++num_entries;
			}
		}
		return num_entries;
	}

	void make_list(int offset, int size) {
		list_size_ = 0;

		if (size > kMaxListSize) {
			size = kMaxListSize;
		}

		rewind();

		while (offset && next_visible()) {
			--offset;
		}

		int i = 0;
		while (next_visible() && size--) {
			list[i].name.write(fil_info_->fname);
			list[i].size = fil_info_->fsize;
			list[i].is_dir = fil_info_->fattrib & AM_DIR;
			++i;
		}
		list_size_ = i;
	}

	int list_size() {
		return list_size_;
	}

	List* read_list(int idx) {
		return idx >= list_size_ ? nullptr : &list[idx];
	}

	void reset() {
		list_size_ = 0;
		filter_ = NONE;
	}

private:
	DIR* dir_;
	FILINFO* fil_info_;
	StringBuilder* dir_path_;
	StringBuilderBase<64>path_;
	StringBuilderBase<64>buffer_;
	int list_size_ = 0;
	Filter filter_ = NONE;
};

#endif
