#include "led_matrix.hpp"

bool hardware::pixelCoordinate::increaseX() {
  if(x_ + 1 >= MATRIX_WIDTH)
    return false;
  ++x_;
  return true;
}

bool hardware::pixelCoordinate::increaseY() {
  if(y_ + 1 >= MATRIX_HEIGHT)
    return false;
  ++y_;
  return true;
}

bool hardware::pixelCoordinate::decreaseX() {
  if(x_ == 0)
    return false;
  --x_;
  return true;
}

bool hardware::pixelCoordinate::decreaseY() {
  if(y_ == 0)
    return false;
  --y_;
  return true;
}



hardware::led_matrix::led_matrix()
{
  
}

hardware::led_matrix::led_matrix(led_matrix &t) : matrix_(t.matrix_)
{
  
}

hardware::led_matrix::~led_matrix() {
  delete matrix_;
}

//##### conversion functions #####
//(coordinates)
uint hardware::led_matrix::Xm(byte m){ //returns X value of Output:leds_[m]
  byte row = (m / MATRIX_WIDTH);
  if(row % 2){  //row is odd
    return ((((((1+row)*MATRIX_WIDTH)- m)-1)*2)-15);
  }
  else{         //row is even
    return (((m - (row*MATRIX_WIDTH))*2)-15);
  }
}

uint hardware::led_matrix::Ym(byte m){ //returns Y value of Output:leds_[m]
  if(0 <= m <= NUM_LEDS-1){
    return (((m / MATRIX_WIDTH)*2)-15);
  }
}

byte hardware::led_matrix::Mxy(uint8_t x, uint8_t y){ //returns leds_[m] value of coordinates (reverse function of Xm/Ym)
  bool reverserow = false;
  if(y % 2){  //row is odd and needs to be reversed
    x = MATRIX_WIDTH - x -1;
  }
  return (y * MATRIX_WIDTH + x);
}

void hardware::led_matrix::init() {
  if(initailized_)
    return;
  matrix_ = new FastLED_NeoMatrix(leds_, MATRIX_WIDTH, MATRIX_HEIGHT, 
                                 NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
                                 NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds_, NUM_LEDS).setCorrection(TypicalSMD5050);  
  matrix_->begin();
  matrix_->setTextWrap(false);
  matrix_->setBrightness(brightness_);

  matrix_->clear();
  matrix_->show();
  
  initailized_ = true;
}

//##### visualisation functions #####

void hardware::led_matrix::setPixel(uint16_t x, uint16_t y, colorPixel color) {
  if(!initailized_)
    return;
  if(x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, y, matrix_->Color24to16(color));
}

void hardware::led_matrix::setPixel(uint16_t x, uint16_t y, uint32_t color) {
  if(!initailized_)
    return;
  if(x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, y, matrix_->Color24to16(color));
}

void hardware::led_matrix::unsetPixel(uint16_t x, uint16_t y) {
  if(!initailized_)
    return;
  if(x > MATRIX_WIDTH || y > MATRIX_HEIGHT)
    return;
  matrix_->writePixel(x, y, 0);
}

void hardware::led_matrix::printText(const char *text, int16_t x, int16_t y, 
                                      CRGB::HTMLColorCode color, bool wrap) {
  matrix_->clear();
  matrix_->setTextWrap(wrap);
  matrix_->setTextSize(1);
  matrix_->setRotation(0);
  matrix_->setCursor(x, MATRIX_HEIGHT - y - 1);
  matrix_->setTextColor(color);
  matrix_->print(text);
  matrix_->show();
}

void hardware::led_matrix::setBrightness(uint8_t brightn) {
  brightness_ = brightn;
  matrix_->setBrightness(brightness_);
}

void hardware::led_matrix::outputMatrix() {
  if(!initailized_)
    return;
  matrix_->show();
}

void hardware::led_matrix::clearMatrix() {
  if(!initailized_)
    return;
  matrix_->clear();
}

