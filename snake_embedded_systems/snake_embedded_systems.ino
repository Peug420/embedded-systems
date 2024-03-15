/*      sanke game on 16x16 rgb led matrix 
        Projekt Embedded Systems WS2023/24 HTWK Leipzig Prof. Dr.-Ing.
        Pretschner und Marco Braun
        Thomas Rubin, Julius Zeng 20EIB
        (c) Thomas Rubin 12.03.2024
*/

#include "game.hpp"
#include "joy_stick.hpp"
#include "led_matrix.hpp"
#include "mbed.h"

#define PIN_JOYSTICK_BUTTON 22
#define PIN_JOYSTICK_VRX 26
#define PIN_JOYSTICK_VRY 27

hardware::led_matrix matrix{};
hardware::joy_stick joyStick{ PIN_JOYSTICK_VRX,
                              PIN_JOYSTICK_VRY };

software::game game{ matrix, joyStick };

bool gameActive = false;

// Interrupts
// mbed::Ticker
mbed::Timer debounce;

void callback_button_interrupt();

void setup() {
  // pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup for random numbers
  randomSeed(analogRead(A0));

  game.init();

  attachInterrupt(digitalPinToInterrupt(PIN_JOYSTICK_BUTTON),
                  callback_button_interrupt, FALLING);

  debounce.start();

  Serial.begin(9600);
  Serial.println("Finished Setup");
}

void loop() {
  if (gameActive) {
    if (!game.exec()) {
      Serial.println("You lose!");
      gameActive = false;
      delay(1000);
    }
    delay(300);
  } else {
    if (game.menu()) {
      gameActive = true;
      game.start();
    }
    delay(500);
  }
}

void callback_button_interrupt() {
  if (debounce.read_ms() > 200) {
    game.setGameState(software::game::gameState::running);
    gameActive = true;
    debounce.reset();
  }
}
