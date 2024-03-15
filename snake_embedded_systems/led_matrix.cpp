#include "led_matrix.hpp"

bool hardware::pixelCoordinate::increaseX() {
  if (x_ + 1 >= MATRIX_WIDTH)
    return false;
  ++x_;
  return true;
}

bool hardware::pixelCoordinate::increaseY() {
  if (y_ + 1 >= MATRIX_HEIGHT)
    return false;
  ++y_;
  return true;
}

bool hardware::pixelCoordinate::decreaseX() {
  if (x_ == 0)
    return false;
  --x_;
  return true;
}

bool hardware::pixelCoordinate::decreaseY() {
  if (y_ == 0)
    return false;
  --y_;
  return true;
}



hardware::led_matrix::led_matrix() {
}

hardware::led_matrix::led_matrix(led_matrix &t)
  : matrix_(t.matrix_) {
}

hardware::led_matrix::~led_matrix() {
  delete matrix_;
}

void hardware::led_matrix::init() {
  if (initailized_)
    return;
  matrix_ = new FastLED_NeoMatrix(leds_, MATRIX_WIDTH, MATRIX_HEIGHT,
                                  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds_, NUM_LEDS).setCorrection(TypicalSMD5050);
  matrix_->begin();
  matrix_->setTextWrap(false);
  matrix_->setBrightness(brightness_);

  matrix_->clear();
  matrix_->show();

  initailized_ = true;
}

void hardware::led_matrix::reset() {
  if(not initailized_) 
    return;

  matrix_->setTextWrap(false);
  matrix_->setBrightness(brightness_);

  matrix_->clear();
  matrix_->show();
}

//##### visualisation functions #####

void hardware::led_matrix::setPixel(uint16_t x, uint16_t y, colorPixel color) {
  if (!initailized_)
    return;
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, MATRIX_HEIGHT - y - 1, matrix_->Color24to16(color));
}

void hardware::led_matrix::setPixel(uint16_t x, uint16_t y, uint32_t color) {
  if (!initailized_)
    return;
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, MATRIX_HEIGHT - y - 1, matrix_->Color24to16(color));
}

void hardware::led_matrix::unsetPixel(uint16_t x, uint16_t y) {
  if (!initailized_)
    return;
  if (x > MATRIX_WIDTH || y > MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, MATRIX_HEIGHT - y - 1, 0);
}

void hardware::led_matrix::printText(const char *text, int16_t x, int16_t y,
                                     CRGB::HTMLColorCode color, bool wrap) {
  matrix_->clear();
  matrix_->setTextWrap(wrap);
  matrix_->setTextSize(1);
  matrix_->setRotation(0);
  matrix_->setCursor(x, MATRIX_HEIGHT - y - 1);
  matrix_->setTextColor(matrix_->Color24to16(color));
  matrix_->print(text);
  matrix_->show();
}

void hardware::led_matrix::setBrightness(uint8_t brightn) {
  brightness_ = brightn;
  matrix_->setBrightness(brightness_);
}

void hardware::led_matrix::outputMatrix() {
  if (!initailized_)
    return;
  matrix_->show();
}

void hardware::led_matrix::clearMatrix() {
  if (!initailized_)
    return;
  matrix_->clear();
}
