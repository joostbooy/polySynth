#include "fileReader.h"

uint8_t FileReader::buffer_[kBufferSize] __attribute__((section(".dtcm")));
