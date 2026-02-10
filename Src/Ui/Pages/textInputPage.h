#ifndef TextInputPage_h
#define TextInputPage_h

namespace TextInputPage {

using TopPage::canvas_;
using TopPage::disk_;
using TopPage::leds_;
using TopPage::buttons_;
using TopPage::pages_;

uint8_t box_x_;
uint8_t box_w_;
int char_cursor_ = 0;
int text_cursor = 0;
uint16_t text_cursor_ticks_ = 0;

char* dest_ptr_;
void (*callback_)(bool);

uint8_t dest_max_;
const uint8_t kCharTableSize = 51;
const uint8_t kCharsPerRow = 17;
const uint8_t kStackMax = 64;
Stack<char, kStackMax> char_stack;
StringBuilderBase<kStackMax> message_;
StringBuilderBase<kStackMax> text_copy;

enum FooterOptions {
  FINISH,
  CANCEL,
  BACKSPACE,
  INSERT,

  NUM_FOOTER_OPTIONS
};

const char* const footer_text[NUM_FOOTER_OPTIONS] = {"FINISH", "CANCEL", "BACKSPACE", "INSERT"};

const char char_table[kCharTableSize] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '!', '#', '$', '%', '&', '-', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '(', ')', '@', '^', '_', '`', '{', '}', '~'};

void init() {
  char_stack.clear();
  message_.clear();
  dest_max_ = 0;
  callback_ = nullptr;
}

void enter() {
}

void exit() {
}

void write_to_dest() {
  for (int i = 0; i < char_stack.size(); ++i) {
    dest_ptr_[i] = char_stack.read(i);
  }
}

void write_to_buffer(char* text, int max) {
  char_stack.clear();

  int length = max - 1;
  while (length-- && *text != '\0') {
    char_stack.push(*text++);
  }
  char_stack.push('\0');

  // set cursor far right
  text_cursor = 0;

  for (int i = 0; i < char_stack.size(); ++i) {
    char c = char_stack.read(i);
    if (c == '\0' || c == '.') {
      return;
    }
    ++text_cursor;
  }
}

void set(const char* dest_ptr, uint8_t dest_max, const char* message, void (*callback)(bool)) {
  char_cursor_ = 0;
  text_cursor_ticks_ = 0;
  message_.write(message);

  callback_ = callback;

  dest_ptr_ = const_cast<char*>(dest_ptr);
  dest_max_ = dest_max;
  if (dest_max_ >= kStackMax) {
    dest_max_ = kStackMax;
  }

  box_w_ = (dest_max_ * 6) + 2;
  box_x_ = (canvas_->width() - box_w_) / 2;

  write_to_buffer(&dest_ptr_[0], dest_max_);
}

// text edit without disk_ check
void set(const char* dest_ptr_, uint8_t max_, const char* message_) {
  set(dest_ptr_, max_, message_, nullptr);
}

void insert(char c) {
  if (char_stack.size() < dest_max_) {
    char_stack.insert(text_cursor, c);
    ++text_cursor;
  }
}

void backspace() {
  if (text_cursor > 0) {
    --text_cursor;
    char_stack.remove_by_index(text_cursor);
  }
}

void text_cursor_left() {
  if (text_cursor > 0) {
    --text_cursor;
  }
}

void text_cursor_right() {
  if (char_stack.read(text_cursor) != '\0' && char_stack.read(text_cursor) != '.') {
    ++text_cursor;
  }
}

void char_cursor_left() {
  if (char_cursor_ > 0) {
    --char_cursor_;
  }
}

void char_cursor_right() {
  if (char_cursor_ < kCharTableSize - 1) {
    ++char_cursor_;
  }
}

void clear() {
  char c = '\0';
  write_to_buffer(&c, dest_max_);
  return;
}

void copy() {
  if (char_stack.size() > 1) {
    text_copy.write(char_stack.pointer());
    MessagePainter::show("TEXT COPIED");
  }
}

void paste() {
  if (text_copy.read()) {
    write_to_buffer(const_cast<char*>(text_copy.read()), dest_max_);
    MessagePainter::show("TEXT PASTED");
  }
}

void on_encoder(int id, int inc) {
  switch (id) {
    case 0:
      inc > 0 ? text_cursor_right() : text_cursor_left();
      break;
    case 1:
      inc > 0 ? char_cursor_right() : char_cursor_left();
      break;
    default:
      break;
  }
}

void on_button(int id, int state) {
  if (!state) {
    return;
  }

  switch (id) {
    case Buttons::ENC_X:
    case Buttons::ENC_Y:
      insert(char_table[char_cursor_]);
      return;
    default:
      break;
  }

  switch (buttons_->toFunction(id)) {
    case BACKSPACE:
      backspace();
      break;
    case FINISH:
      write_to_dest();

      pages_->close(Pages::TEXT_INPUT_PAGE);
      if (callback_) {
        callback_(true);
      }
      break;
    case CANCEL:
      pages_->close(Pages::TEXT_INPUT_PAGE);
      if (callback_) {
        callback_(false);
      }
      break;
    case INSERT:
      insert(char_table[char_cursor_]);
      break;
    default:
      break;
  }
}

void refresh_leds() {
 
}

const size_t target_fps() {
  return 1000 / 16;
}

void draw() {
  canvas_->clear();
  canvas_->setFont(Font::SMALL);

  // draw message
  canvas_->fill(0, 0, canvas_->width(), 10, Canvas::BLACK);
  canvas_->drawText(0, 0, canvas_->width(), 10, message_.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

  // draw char buffer
  canvas_->fill(box_x_, 14, box_w_, 11, Canvas::DARK_GRAY);

  int cursor_x = box_x_;
  int char_x = box_x_ + 2;
  int char_y = 16;
  int length = char_stack.size();

  canvas_->setFont(Font::SMALL, Canvas::WHITE);

  for (int i = 0; i < length; ++i) {
    char c = char_stack.read(i);
    canvas_->drawChar(char_x, char_y, c);
    char_x += canvas_->font().width(c);

    if (i < text_cursor) {
      cursor_x = char_x;
    }
  }

  // draw text_cursor
  if (text_cursor_ticks_ < 500) {
    uint8_t h = canvas_->font().height();
    cursor_x = SettingsUtils::clipMin(box_x_, cursor_x - 1);
    canvas_->verticalLine(cursor_x, char_y, h, Canvas::WHITE);
  }

  if ((text_cursor_ticks_ += target_fps()) >= 1000) {
    text_cursor_ticks_ = 0;
  }

  // draw char table
  int x = 2;
  int y = 27;
  Canvas::Color color;

  for (int i = 0; i < kCharTableSize; ++i) {
    color = char_cursor_ == i ? Canvas::BLACK : Canvas::LIGHT_GRAY;
    canvas_->setFont(Font::SMALL, color);
    canvas_->drawChar(x, y, char_table[i]);
    x = canvas_->textCursor() + 10;
    if ((i % kCharsPerRow) == (kCharsPerRow - 1)) {
      x = 2;
      y += 8;
    }
  }

  // footer
  WindowPainter::draw_footer(footer_text, NUM_FOOTER_OPTIONS);
}

Pages::Page page = {&init, &enter, &exit, &draw, &refresh_leds, &on_button, &on_encoder, &target_fps};

};  // namespace TextInputPage

#endif
