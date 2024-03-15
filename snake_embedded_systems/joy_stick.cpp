#include "joy_stick.hpp"

hardware::joy_stick::joy_stick(uint8_t _vrxPin,
                               uint8_t _vryPin)
  : vrxPin_(_vrxPin),
    vryPin_(_vryPin) {
}

hardware::joy_stick::joy_stick(joy_stick &t)
  : vrxPin_(t.vrxPin_),
    vryPin_(t.vryPin_) {
}

hardware::joy_stick::~joy_stick() {
}

bool hardware::joy_stick::isOutOfDetectBand(const int32_t &val) {
  return (abs(val - centerPoint_) > centerDetectRadius_);
}


bool hardware::joy_stick::isOutOfDetectRadius(uint32_t x, uint32_t y) {
  double distance = sqrt(sq(x - (centerPoint_)) + sq(y - (centerPoint_)));
  return (distance > centerDetectRadius_);
}

void hardware::joy_stick::init() {
  if (initialized_)
    return;
  analogReadResolution(12);
  initialized_ = true;
}

hardware::joy_stick::direction hardware::joy_stick::getDirection() {  //Returns Joystick User input 0 = No Input; 1 = Up; 2 = Down; 3 = Left; 4 = Right;
  if (!initialized_)                                                  // not initialized
    return hardware::joy_stick::direction::noDirection;

  uint16_t x = analogRead(vrxPin_);
  uint16_t y = analogRead(vryPin_);

  if (isOutOfDetectRadius(x, y)) {
    if ((x >= y) && (!isOutOfDetectBand(y)))
      return hardware::joy_stick::direction::up;
    else if ((x < y) && (!isOutOfDetectBand(y)))
      return hardware::joy_stick::direction::down;
    else if ((y >= x) && (!isOutOfDetectBand(x)))
      return hardware::joy_stick::direction::right;
    else if ((y < x) && (!isOutOfDetectBand(x)))
      return hardware::joy_stick::direction::left;
  }

  return hardware::joy_stick::direction::noDirection;
}