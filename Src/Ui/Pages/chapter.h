#ifndef Chapter_h
#define Chapter_h

#define ENTRY_SIZE(entry) (sizeof(entry) / sizeof(int))

#include "pages.h"

class Chapter {

public:

	class Entry {

	public:

		void init(const int *page_table, const char* const *page_names, int size) {
			page_table_ = page_table;
			page_names_ = page_names;
			size_ = size;
		}

		int size() {
			return size_;
		}

		int page_index() {
			return pages_index_;
		}

		void set_page_index(int index) {
			pages_index_ = SettingsUtils::clip(0, size_ - 1, index);
		}

		int selected_page() {
			return page_table_[pages_index_];
		}

		const char *name(int index) {
			return page_names_[index];
		}

	private:
		int pages_index_ = 0;
		int size_;
		const int *page_table_;
		const char* const* page_names_;
	};

	enum Id {
		INSTRUMENT,
		SAMPLE,
		SYSTEM,
		MIDI,
		MODULATION,
		STORAGE,

		NUM_CHAPTERS
	};

	const char *name() {
		switch (selected_)
		{
		case MIDI:			return "MIDI";
		case MODULATION:	return "MODULATION";
		case SYSTEM:		return "SYSTEM";
		case STORAGE:		return "STORAGE";
		case INSTRUMENT:	return "INSTRUMENT";
		case SAMPLE:		return "SAMPLE";
		default:
			break;
		}
		return nullptr;
	}

	void init();

	int selected() {
		return selected_;
	}

	void set(int index) {
		selected_ = SettingsUtils::clip(0, NUM_CHAPTERS - 1, index);

		switch (selected_)
		{
		case MIDI:
			entry_ = &midi_;
			break;
		case MODULATION:
			entry_ = &modulation_;
			break;
		case SAMPLE:
			entry_ = &sample_;
			break;
		case INSTRUMENT:
			entry_ = &instrument_;
			break;
		case STORAGE:
			entry_ = &storage_;
			break;
		case SYSTEM:
			entry_ = &system_;
			break;
		default:
			break;
		}
	}

	Entry *entry() {
		return entry_;
	}

private:
	Entry *entry_;
	int selected_ = INSTRUMENT;

	// Midi
	Entry midi_;
	static const int midi_pages[];
	static const char* const midi_page_names[];

	// Modulation
	Entry modulation_;
	static const int modulation_pages[];
	static const char* const modulation_page_names[];

	// System
	Entry system_;
	static const int system_pages[];
	static const char* const system_page_names[];

	// Instrument
	Entry instrument_;
	static const int instrument_pages[];
	static const char* const instrument_page_names[];

	// Sample
	Entry sample_;
	static const int sample_pages[];
	static const char* const sample_page_names[];

	// Storage
	Entry storage_;
	static const int storage_pages[];
	static const char* const storage_page_names[];
};

#endif
