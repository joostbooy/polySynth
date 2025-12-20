#ifndef Directory_h
#define Directory_h

#include "ff.h"
#include "stringBuilder.h"

class Directory {

public:

	void init(DIR *dir, uint8_t root_id) {
		dir_ = dir;
		root_id_ = root_id;
		path.write(root_id_, ":");
	}

	StringBuilder* path_ptr() {
		return &path;
	}

	uint8_t maxPathLength() {
		return kMaxPathLength;
	}

	const char* readPath() {
		const char* curr_path = path.read();
		return buffer.write(curr_path);
	}

	bool openPath(const char* path_) {
		const char* new_path_ = build_new_path(path_);
		if (f_opendir(dir_, new_path_) == FR_OK) {
			path.write(new_path_);
			return true;
		}
		return false;
	}

	bool close() {
		return f_closedir(dir_) == FR_OK;
	}

	const char* root() {
		return buffer.write(root_id_, ":");
	}

	bool hasRoot(const char* path_) {
		return (path_[0] == (root_id_ + 48)) && (path_[1] == ':');
	}

	bool isRoot() {
		const char* text = path.read();
		return hasRoot(text) && (text[2] == '\0');
	}

	bool reset() {
		close();
		path.write(root());
		return f_opendir(dir_, path.read()) == FR_OK;
	}

	bool reopen() {
		return f_opendir(dir_, path.read()) == FR_OK;
	}

	bool enter(const char* dir_name) {
		const char* old_path = path.read();

		buffer.write(old_path);
		buffer.append("/");
		buffer.append(dir_name);

		return openPath(buffer.read());
	}

	bool exit() {
		const char* old_path = path.read();
		uint8_t length = path.length();

		if (isRoot()) {
			return 0;
		}

		old_path += (length - 1);

		while (length--) {
			if (*old_path-- == '/') {
				old_path = path.read();

				buffer.write(old_path);
				buffer.truncate(length);
				return openPath(buffer.read());
			}
		}
		return 0;
	}

	bool make(const char* dir_name) {
		buffer.write(path.read(), "/", dir_name);
		return f_mkdir(buffer.read()) == FR_OK;
	}

	bool remove(const char* dir_name) {
		buffer.write(path.read(), "/", dir_name);
		return f_unlink(buffer.read()) == FR_OK;
	}

private:
	DIR *dir_;
	uint8_t root_id_;

	static const uint8_t kMaxPathLength = 64;
	StringBuilderBase<64>path;			// the current valid path
	StringBuilderBase<64>new_path;		// buffer to build new path, used only in build_new_path()
	StringBuilderBase<64>buffer;		// general text buffer

	const char* build_new_path(const char* path_) {
		new_path.write(path_);

		if (hasRoot(new_path.read()) == false) {
			new_path.prepend(root());
		}

		// remove extension
		int pos = 0;
		path_ = new_path.read();

		while (*path_ != '.') {
			if (*path_++ == '\0') {
				return new_path.read();
			}
			++pos;
		}
		new_path.truncate(pos);
		return new_path.read();
	}

};

#endif
