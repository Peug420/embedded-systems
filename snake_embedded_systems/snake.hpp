#ifndef __snake_header_included__
#define __snake_header_included__

#include <deque>
#include <algorithm>
#include "led_matrix.hpp"
#include "joy_stick.hpp"

namespace software {
class snake {
  private:
    /**
     * @brief Safes the coordinates of the snake connected by a deque. By this
     *        method the and tail can dynamically added or removed.
     * 
     */
    std::deque<hardware::pixelCoordinate> snake_;
    /**
     * @brief The direction the snake faces. Can only be up, left, down or
     *        right. Can not be noDirection.
     * 
     */
    hardware::joy_stick::direction snakeDirection_;
    /**
     * @brief Is set true, when init() is called.
     * 
     */
    bool initialized_{false};
    /**
     * @brief Checks if the snake contains the coordinate.
     * 
     * @param coord Coordinate to check.
     * @return true Contains the coordinate.
     * @return false Does not contain the coordinate.
     */
    bool contains(const hardware::pixelCoordinate &coord);

  public:
    /**
     * @brief Construct a new snake object.
     * 
     */
    snake();
    /**
     * @brief Destroy the snake object.
     * 
     */
    ~snake();
    /**
     * @brief Init function for this object. Need to be called shortly after
     *        initialisation. The start coordinate and start direction is set by
     *        this method.
     * 
     * @param coord Starting coordinate.
     * @param dir Starting direction.
     */
    void init(hardware::pixelCoordinate coord, 
              hardware::joy_stick::direction dir);
    /**
     * @brief Move the snake on coordinate. Add the new coordinate and deletes
     *        the tail coordiante of the snake.
     * 
     * @param coord New coordinate to add.
     */
    void move(hardware::pixelCoordinate coord);
    /**
     * @brief Grows the snake to the coordinate. Add the new coordinate, but
     *        does not delete the tail coordinate.
     * 
     * @param coord New coordinate to add.
     */
    void grow(hardware::pixelCoordinate coord);
    /**
     * @brief Get the coordinate of the snake head.
     * 
     * @return hardware::pixelCoordinate Coordinate of the head.
     */
    hardware::pixelCoordinate getHeadCoord();
    /**
     * @brief Get the coordinate of the snake tail.
     * 
     * @return hardware::pixelCoordinate Coordinate of the tail.
     */
    hardware::pixelCoordinate getTailCoord();
    /**
     * @brief Get the direction of the snake.
     * 
     * @return hardware::joy_stick::direction Direction of the snake.
     */
    hardware::joy_stick::direction getDirection();
    /**
     * @brief Set the direction object of the snake. Can not be no direction,
     *        must be up, right, lef or down.
     * 
     * @param dir New direction to set.
     */
    void setDirection(hardware::joy_stick::direction dir);
    /**
     * @brief Get the length of the snake, the number of coordinates in the
     *        deque.
     * 
     * @return uint8_t Length of the snake.
     */
    uint8_t getSnakeLength();
};
}

#endif // __snake_header_included__
