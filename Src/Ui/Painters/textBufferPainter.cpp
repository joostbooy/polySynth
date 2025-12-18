#include "TextBufferPainter.h"

int TextBufferPainter::entries_ = 0;
int TextBufferPainter::top_row_ = 0;
const int TextBufferPainter::kNumRows;
Canvas* TextBufferPainter::canvas_;
StringBuilderBase<32>TextBufferPainter::text_[kNumRows];
