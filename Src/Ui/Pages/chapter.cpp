#include "chapter.h"

// Midi
 constexpr const int Chapter::midi_pages[] = {
	Pages::EMPTY_PAGE,
};

 constexpr const char* const Chapter::midi_page_names[] = {
	"EMPTY",
};


// Modulation
constexpr const int Chapter::modulation_pages[] = {
	Pages::EMPTY_PAGE,
};

constexpr const char* const Chapter::modulation_page_names[] = {
	"EMPTY",
};


// System
constexpr const int Chapter::system_pages[] = {
	Pages::EMPTY_PAGE,
};

constexpr const char* const Chapter::system_page_names[] = {
	"EMPRY",
};


// Instrument
constexpr const int Chapter::instrument_pages[] = {
	Pages::EMPTY_PAGE,
};

constexpr const char* const Chapter::instrument_page_names[] = {
	"EMPTY",
};


// Sample
constexpr const int Chapter::sample_pages[] = {
	Pages::EMPTY_PAGE,
};

constexpr const char* const Chapter::sample_page_names[] = {
	"EMPTY",
};


// Storage
constexpr const int Chapter::storage_pages[] = {
	Pages::EMPTY_PAGE,
};

constexpr const char* const Chapter::storage_page_names[] = {
	"EMPTY"
};


void Chapter::init() {
	modulation_.init(modulation_pages, modulation_page_names, ENTRY_SIZE(modulation_pages));
	midi_.init(midi_pages, midi_page_names, ENTRY_SIZE(midi_pages));
	sample_.init(sample_pages, sample_page_names, ENTRY_SIZE(sample_pages));
	system_.init(system_pages, system_page_names, ENTRY_SIZE(system_pages));
	storage_.init(storage_pages, storage_page_names, ENTRY_SIZE(storage_pages));
	instrument_.init(instrument_pages, instrument_page_names, ENTRY_SIZE(instrument_pages));
}
