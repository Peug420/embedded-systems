#include "game.hpp"

// constructor / destrcutor

software::game::game(hardware::led_matrix &_ledMatrix,
                     hardware::joy_stick &_joyStick)
  : ledMatrix_(_ledMatrix),
    joyStick_(_joyStick),
    snake_() {
}

software::game::~game() {
}

// private

void software::game::setPixel(hardware::pixelCoordinate coord, pixelType type) {
  if (coord.x_ >= MATRIX_WIDTH || coord.y_ >= MATRIX_HEIGHT)
    return;
  gameBoard_[coord.x_][coord.y_] = type;

  switch (type) {
    case pixelType::snakeHead:
      ledMatrix_.setPixel(coord.x_, coord.y_, hardware::led_matrix::colorPixel::head);
      break;
    case pixelType::snakeTail:
      ledMatrix_.setPixel(coord.x_, coord.y_, hardware::led_matrix::colorPixel::tail);
      break;
    case pixelType::food:
      ledMatrix_.setPixel(coord.x_, coord.y_, hardware::led_matrix::colorPixel::food);
      break;
    case pixelType::obstacle:
      ledMatrix_.setPixel(coord.x_, coord.y_, hardware::led_matrix::colorPixel::obstacle);
      break;
    case pixelType::unset:
      ledMatrix_.unsetPixel(coord.x_, coord.y_);
      break;
  }
}

bool software::game::isPixelFree(hardware::pixelCoordinate coord) {
  if (gameBoard_[coord.x_][coord.y_] == pixelType::unset)
    return true;
  return false;
}

bool software::game::isPixelFood(hardware::pixelCoordinate coord) {
  if (gameBoard_[coord.x_][coord.y_] == pixelType::food)
    return true;
  return false;
}

bool software::game::isPixelObstacle(hardware::pixelCoordinate coord) {
  if (gameBoard_[coord.x_][coord.y_] == pixelType::obstacle)
    return true;
  return false;
}

bool software::game::isPixelSnake(hardware::pixelCoordinate coord) {
  if ((gameBoard_[coord.x_][coord.y_] == pixelType::snakeHead) || (gameBoard_[coord.x_][coord.y_] == pixelType::snakeTail))
    return true;
  return false;
}

software::game::pixelType software::game::getPixelType(hardware::pixelCoordinate coord) {
  return gameBoard_[coord.x_][coord.y_];
}

void software::game::loseGame() {
  const static int16_t charWidth = 6;

  std::string lost{ "You lost! Score: " + std::to_string(snake_.getSnakeLength()) };

  for (int16_t x = (MATRIX_WIDTH - 1);
       x >= (static_cast<int16_t>(lost.size()) * charWidth * -1); x--) {
    ledMatrix_.printText(lost.c_str(), x, 10, CRGB::Blue, false);
    delay(100);
  }
  currentState_ = gameState::menuState;
}

void software::game::spawnFood(const uint8_t &number) {
  if (number > 10)
    return;
  for (uint8_t i = 0; i < number; ++i) {
    bool spawnSucc = false;
    do {
      uint8_t x = random(MATRIX_WIDTH);
      uint8_t y = random(MATRIX_HEIGHT);
      hardware::pixelCoordinate newCoord{ x, y };
      if (isPixelFree(newCoord)) {
        setPixel(newCoord, pixelType::food);
        spawnSucc = true;
      }
    } while (!spawnSucc);
    ++numberOfFood_;
  }
}

bool software::game::makeMove() {
  hardware::pixelCoordinate newCoord;
  switch (snake_.getDirection()) {
    case hardware::joy_stick::direction::up:
      newCoord = snake_.getHeadCoord();
      if (!newCoord.increaseY())  // move is out of game board
        return false;
      break;
    case hardware::joy_stick::direction::down:
      newCoord = snake_.getHeadCoord();
      if (!newCoord.decreaseY())
        return false;  // move is out of game board
      break;
    case hardware::joy_stick::direction::left:
      newCoord = snake_.getHeadCoord();
      if (!newCoord.decreaseX())  // move is out of game board
        return false;
      break;
    case hardware::joy_stick::direction::right:
      newCoord = snake_.getHeadCoord();
      if (!newCoord.increaseX())  // move is out of game board
        return false;
      break;
    case hardware::joy_stick::direction::noDirection:
      return false;
      break;
    default:
      return false;
      break;
  }

  switch (getPixelType(newCoord)) {
    case pixelType::unset:
      setPixel(snake_.getTailCoord(), pixelType::unset);
      if (snake_.getSnakeLength() > 1) {
        setPixel(snake_.getHeadCoord(), pixelType::snakeTail);
        snake_.move(newCoord);
        setPixel(newCoord, pixelType::snakeHead);
      } else {
        snake_.move(newCoord);
        setPixel(snake_.getHeadCoord(), pixelType::snakeHead);
      }
      return true;
    case pixelType::snakeTail:
      return false;
    case pixelType::food:
      setPixel(snake_.getHeadCoord(), pixelType::snakeTail);
      snake_.grow(newCoord);
      setPixel(newCoord, pixelType::snakeHead);
      --numberOfFood_;
      return true;
    case pixelType::obstacle:
      return false;
    default:
      return false;
  }
}

// public

void software::game::init() {
  ledMatrix_.init();
  joyStick_.init();
  initialized_ = true;
}

void software::game::start() {
  if (not initialized_)
    return;

  for (size_t x = 0; x < MATRIX_WIDTH; x++) {
    for (size_t y = 0; y < MATRIX_HEIGHT; y++) {
      setPixel(hardware::pixelCoordinate{ x, y }, pixelType::unset);
    }
  }

  hardware::joy_stick::direction dir;
  switch (random(0, 4)) {
    case 0:
      dir = hardware::joy_stick::direction::up;
      break;
    case 1:
      dir = hardware::joy_stick::direction::down;
      break;
    case 2:
      dir = hardware::joy_stick::direction::left;
      break;
    case 3:
      dir = hardware::joy_stick::direction::right;
      break;
    default:
      dir = hardware::joy_stick::direction::up;
      break;
  }

  // initialize snake
  snake_.init(hardware::pixelCoordinate{ random(MATRIX_WIDTH * 0.3,
                                                MATRIX_WIDTH * 0.7),
                                         random(MATRIX_HEIGHT * 0.3,
                                                MATRIX_HEIGHT * 0.7) },
              dir);

  setPixel(snake_.getHeadCoord(), pixelType::snakeHead);

  numberOfFood_ = 0;
  spawnFood(2);

  ledMatrix_.outputMatrix();
  currentState_ = gameState::running;
}

void software::game::setGameState(gameState state) {
  currentState_ = state;
}

bool software::game::exec() {
  if (currentState_ != gameState::running)
    return false;

  hardware::joy_stick::direction dir = joyStick_.getDirection();

  if (dir != hardware::joy_stick::direction::noDirection) {
    snake_.setDirection(dir);
  }

  if (!makeMove()) {
    currentState_ = gameState::loseScreen;
    loseGame();
    return false;
  }

  if (numberOfFood_ < 2) {
    spawnFood(2);
  }

  ledMatrix_.outputMatrix();

  return true;
}

bool software::game::menu() {
  if (currentState_ != gameState::menuState)
    return false;
  const static int16_t charWidth = 6;

  std::string menu{ "Press to start." };

  for (int16_t x = (MATRIX_WIDTH - 1);
       x >= (static_cast<int16_t>(menu.size()) * charWidth * -1); x--) {
    ledMatrix_.printText(menu.c_str(), x, 10, CRGB::Blue, false);
    if (currentState_ == game::running)
      return true;
    delay(100);
  }
  return false;
}