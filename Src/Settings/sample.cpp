#include "sample.h"

char Sample::name_[kMaxPathLength];
const uint8_t Sample::char_to_note_[7];
const bool Sample::has_seminote_[12];

bool Sample::name_to_midi_note(const char *name, uint8_t *note) {
	int oct_ = -1;
	int note_ = -1;

	while (*name != '\0') {
		char c = *name++;

		switch (c)
		{
		case 'A' ... 'G':
			if (note_ == -1 && oct_ == -1) {
				note_ = char_to_note_[c - 'A'];
			} else {
				return false;
			}
			break;
		case '#':
			if (note_ >= 0 && oct_ == -1 && has_seminote_[note_] == true) {
				note_ += 1;
			} else {
				return false;
			}
			break;
		case '0' ... '9':
			if (note_ == -1) {
				return false;
			}

			if (oct_ == -1) {
				oct_ = 12 * (c - '0');
			} else if (oct_ == 12 && c == '0') {
				oct_ = 120;
			} else {
				return false;
			}
			break;
		case '.':
		case '_':
		case '\0':
			if (note_ >= 0 && oct_ >= 0) {
				note_ += oct_;
				*note = note_ > 127 ? 127 : note_;
				return true;
			} else {
				return false;
			}
		default:
			return false;
		}
	}

	return false;
}
