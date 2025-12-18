#include "frameBuffer.h"

uint8_t FrameBuffer::data_[FrameBuffer::size_] __attribute__((section(".dtcm")));
