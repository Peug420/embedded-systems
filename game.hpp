#ifndef __game_header_included__
#define __game_header_included__

#include <algorithm>
#include <Arduino.h>
#include "snake.hpp"
#include "led_matrix.hpp"
#include "joy_stick.hpp"

namespace software {

class game {
  private:
    enum gameMode {
      easy,
      normal,
      hard
    };

    enum pixelType {
      unset,
      snakeHead,
      snakeTail,
      food,
      obstacle
    };

    //std::array<std::array<pixelType, MATRIX_WIDTH>, MATRIX_HEIGHT> gameBoard_;
    pixelType gameBoard_[MATRIX_WIDTH][MATRIX_HEIGHT];
    hardware::led_matrix ledMatrix_;
    hardware::joy_stick joyStick_;
    software::snake snake_;
    gameMode currentGameMode_{gameMode::normal};
    uint8_t numberOfFood_{0};


    void setPixel(hardware::pixelCoordinate coord, pixelType type);
    bool isPixelFree(hardware::pixelCoordinate coord);
    bool isPixelFood(hardware::pixelCoordinate coord);
    bool isPixelObstacle(hardware::pixelCoordinate coord);
    bool isPixelSnake(hardware::pixelCoordinate coord);
    pixelType getPixelType(hardware::pixelCoordinate coord);
    void loseGame();
    void spawnFood(const uint8_t &number);
    bool makeMove();
  
  public:
    game(hardware::led_matrix _ledMatrix, hardware::joy_stick _joyStick);
    ~game();

    void init();

    bool exec();
};
}

#endif // __game_header_included__