#include "snake.hpp"

// constructors / destructors

software::snake::snake() : 
            snakeDirection_(hardware::joy_stick::direction::noDirection)
{
  
}

software::snake::~snake() {

}

// private

bool software::snake::contains(const hardware::pixelCoordinate &coord) {
  return ((std::find(snake_.begin(), snake_.end(), coord) == std::end(snake_)) 
          ? false : true);
}

// public

void software::snake::init(hardware::pixelCoordinate coord, hardware::joy_stick::direction dir) {
  if(initialized_)
    return;

  snake_.push_front(coord);
  snakeDirection_ = dir;

  initialized_ = true;
}

void software::snake::move(hardware::pixelCoordinate coord) {
  if(!initialized_)
    return;
  snake_.push_front(coord);
  snake_.pop_back();
}

void software::snake::grow(hardware::pixelCoordinate coord) {
  if(!initialized_)
    return;
  snake_.push_front(coord);
}

hardware::pixelCoordinate software::snake::getHeadCoord() {
  return snake_.front();
}

hardware::pixelCoordinate software::snake::getTailCoord() {
  return snake_.back();
}

hardware::joy_stick::direction software::snake::getDirection() {
  return snakeDirection_;
}

void software::snake::setDirection(hardware::joy_stick::direction dir) {
  snakeDirection_ = dir;
}

uint8_t software::snake::getSnakeLength() {
  return snake_.size();
}





