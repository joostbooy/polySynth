#include "controller.h"

uint32_t Controller::pressed_[(8 * 8) / 32];
uint8_t Controller::curr_touched_encoder_;
uint8_t Controller::prev_touched_encoder_;
