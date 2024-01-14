#ifndef __snake_header_included__
#define __snake_header_included__

#include <deque>
#include <iterator>
#include <algorithm>
#include "led_matrix.hpp"
#include "joy_stick.hpp"

namespace software {
class snake {
  private:
    std::deque<hardware::pixelCoordinate> snake_;
    hardware::joy_stick::direction snakeDirection_;
    bool initialized_{false};

    bool contains(const hardware::pixelCoordinate &coord);

  public:
    snake();
    ~snake();

    void init(hardware::pixelCoordinate coord, 
              hardware::joy_stick::direction dir);

    void move(hardware::pixelCoordinate coord);
    void grow(hardware::pixelCoordinate coord);

    hardware::pixelCoordinate getHeadCoord();
    hardware::pixelCoordinate getTailCoord();
    hardware::joy_stick::direction getDirection();
    void setDirection(hardware::joy_stick::direction dir);
    uint8_t getSnakeLength();
};
}

#endif // __snake_header_included__
