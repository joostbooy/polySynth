#ifndef Settings_h
#define Settings_h

#include "stringBuilder.h"
#include "settingsUtils.h"
#include "disk.h"
#include "fileWriter.h"
#include "fileReader.h"

class Settings {

public:

	static const size_t kMaxVoices = 8;
	static const size_t kNumInstruments = 8;
	static const size_t kNumLfos = 4;
	static const size_t kNumEnvelopes = 2;

	void init(Disk *disk) {
		disk_ = disk;

		selected_sample_index_ = 0;
		selected_lfo_index_ = 0;
		selected_instrument_index_ = 0;
		selected_envelope_index_ = 0;
		selected_instrument_sample_index_ = 0;

		path.clear();

		set_project_name("NEW.PRJ");
	}

	void init() {
		if (disk_ != nullptr) {
			init(disk_);
		}
	}


	// save & load
	bool save();
	bool save(const char* new_path);
	bool load(const char* new_path);

	bool has_valid_path() {
		char file_name[max_name_length()];

		if (path.length() > 0) {
			StringUtils::get_file_name_from_path(const_cast<char*>(path.read()), file_name);
			return StringUtils::text_is_equal(file_name, project_name_);
		}
		return false;
	}

	uint32_t current_version() {
		return 0;
	}

	Disk *disk() {
		return disk_;
	}

	// name
	const char *project_name() {
		return project_name_;
	}

	void set_project_name(const char *value) {
		return StringUtils::copy(project_name_, const_cast<char*>(value), max_name_length());
	}

	const size_t max_name_length() {
		return 13;
	}


private:
	StringBuilderBase<63>path;

	FileWriter fileWriter;
	FileReader fileReader;

	Disk *disk_ = nullptr;

	int selected_lfo_index_;
	int selected_sample_index_;
	int selected_instrument_index_;
	int selected_envelope_index_;
	int selected_instrument_sample_index_;

	char project_name_[8];
};

#endif
