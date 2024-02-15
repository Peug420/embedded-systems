#ifndef __game_header_included__
#define __game_header_included__

#include <string>

#include "snake.hpp"
#include "led_matrix.hpp"
#include "joy_stick.hpp"

namespace software {

class game {
  private:
    /**
     * @brief !Not yet implemented! Defining 3 game modes for possibly later
     *        implementation of game modes.
     * 
     */
    enum gameMode {
      easy,
      normal,
      hard
    };
    /**
     * @brief All possible pixel types that can occur on the game board.
     * 
     */
    enum pixelType {
      unset,
      snakeHead,
      snakeTail,
      food,
      obstacle
    };

    //std::array<std::array<pixelType, MATRIX_WIDTH>, MATRIX_HEIGHT> gameBoard_;
    /**
     * @brief Pixel matrix of the game board, holds the information of which 
     *        pixel is on which position.
     * 
     */
    pixelType gameBoard_[MATRIX_WIDTH][MATRIX_HEIGHT];
    /**
     * @brief Object for storing the led pixel information.
     * 
     */
    hardware::led_matrix ledMatrix_;
    /**
     * @brief Object for reading input from a connected joystick.
     * 
     */
    hardware::joy_stick joyStick_;
    /**
     * @brief Object for controlling the snake.
     * 
     */
    software::snake snake_;
    /**
     * @brief game mode
     * 
     */
    gameMode currentGameMode_{gameMode::normal};
    /**
     * @brief Number of current food on the board.
     * 
     */
    uint8_t numberOfFood_{0};

    /**
     * @brief Set the pixel type object in the gameBoard_ array.
     * 
     * @param coord 
     * @param type 
     */
    void setPixel(hardware::pixelCoordinate coord, pixelType type);
    /**
     * @brief Checks if the pixel has pixel type unset.
     * 
     * @param coord Coordinate to check.
     * @return true Pixel type is unset.
     * @return false Pixel type is not unset.
     */
    bool isPixelFree(hardware::pixelCoordinate coord);
    /**
     * @brief Checks if the pixel has pixel type food.
     * 
     * @param coord Coordinate to check.
     * @return true Pixel type is food.
     * @return false Pixel type is not food.
     */
    bool isPixelFood(hardware::pixelCoordinate coord);
    /**
     * @brief Checks if the pixel has pixel type obstacle.
     * 
     * @param coord Coordinate to check.
     * @return true Pixel type is obstacle.
     * @return false Pixel type is not obstacle.
     */
    bool isPixelObstacle(hardware::pixelCoordinate coord);
    /**
     * @brief Checks if the pixel has pixel type snake.
     * 
     * @param coord Coordinate to check.
     * @return true Pixel type is snake.
     * @return false Pixel type is not snake.
     */
    bool isPixelSnake(hardware::pixelCoordinate coord);
    /**
     * @brief Get the Pixel Type object at the coordinate.
     * 
     * @param coord Coordinate to get pixel type.
     * @return pixelType 
     */
    pixelType getPixelType(hardware::pixelCoordinate coord);
    /**
     * @brief Is called when the game is lost. That's the case when makeMove 
     *        returns false
     * 
     */
    void loseGame();
    /**
     * @brief Spawns a given number of food on random coordiantes on the 
     *        game board. Before overwriting a set pixel, checks if the
     *        pixel is unset. Continuisly checks the pixel set state until
     *        it finds the given number of unset pixel.
     * 
     * @param number Number of pixels to spawn. 
     */
    void spawnFood(const uint8_t &number);
    /**
     * @brief Moves the snake one pixel in the direction of the joy stick,
     *        if the joy stick has no direction, uses the last input direction.
     *        Gets the direction and sets all the pixel data. After setting
     *        everything, checks if the new pixel coordinate is invalid,
     *        for exampel the body of the snake or the game border and returns
     *        the result.
     * 
     * @return true Move was valid, game can continue.
     * @return false Move was not valid, loseGame() should be called, after
     *               returning from this method.
     */
    bool makeMove();
  
  public:
    /**
     * @brief Construct a new game object.
     * 
     * @param _ledMatrix Object for storing the LED information, setting 
     *                   colours and controlling higher graphical functionality.
     * @param _joyStick Object for getting directional information of the 
     *                  connected joy stick.
     */
    game(hardware::led_matrix _ledMatrix, hardware::joy_stick _joyStick);
    /**
     * @brief Destroy the game object
     * 
     */
    ~game();
    /**
     * @brief Should be called shortly after the object is constructed.
     *        Initializes the ledMatrix_ object and the joyStick_ object by
     *        calling their init functions. Unsets all pixels in gameBoard_.
     *        Sets a random inital direction and a random start point for 
     *        the snake. The random startpoint is in an inner square which 
     *        is 70% of the game board size. Spawns 2 randomly placed foods
     *        on the board.
     * 
     */
    void init();
    /**
     * @brief Executes one frame of the game, whereby a frame and a move of
     *        the snake is the same cycle. Thus the game speed is controlled by 
     *        the rate of calling exec. Should be called about every 300 ms for
     *        playable results. Returns if the game is lost.
     * 
     * @return true Game can continue.
     * @return false Game is lost.
     */
    bool exec();
    /**
     * @brief Not yet implemented. Should not be used. At current state just
     *        returns if the joy stick is pressed.
     * 
     * @return true Joy stick is pressed.
     * @return false Joy stick is not pressed.
     */
    bool menu();
};
}

#endif // __game_header_included__