#include "settings.h"

#include "diskUtils.h"

bool Settings::save(const char* new_path) {
	path.write(new_path, "/", project_name());
	if (!save()) {
		path.clear();
		return false;
	}
	return true;
}

bool Settings::save() {
	if (!path.length()) {
		return false;
	}

	fileWriter.start(&disk_->file(), path.read(), current_version());

	//midi().save(fileWriter);


	fileWriter.stop();

	if (!fileWriter.write_ok()) {
		return false;
	}

	return true;
};

bool Settings::load(const char* new_path) {
	init();	// also clears the path

	fileReader.start(&disk_->file(), new_path);

	//midi().load(fileReader);

	fileReader.stop();

	if (!fileReader.read_ok()) {
		init();
		return false;
	}

	path.write(new_path);	// the new valid path
	return true;
};
