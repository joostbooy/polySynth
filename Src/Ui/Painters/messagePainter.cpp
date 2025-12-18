#include "messagePainter.h"

int MessagePainter::x = 0;
int MessagePainter::y = 16;
int MessagePainter::w = 0;
int MessagePainter::h = 32;

Canvas* MessagePainter::canvas_;

uint16_t MessagePainter::millis_ = 0;
StringBuilderBase<32> MessagePainter::str;
