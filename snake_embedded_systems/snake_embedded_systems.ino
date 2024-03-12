/*      sanke game on 16x16 rgb led matrix 
        Projekt Embedded Systems WS2023/24 HTWK Leipzig Prof. Dr.-Ing.
        Pretschner und Marco Braun
        Thomas Rubin, Julius Zeng 20EIB
        (c) Thomas Rubin 12.03.2024
*/

#include "game.hpp"
#include "joy_stick.hpp"
#include "led_matrix.hpp"

#define PIN_JOYSTICK_BUTTON 22
#define PIN_JOYSTICK_VRX 26
#define PIN_JOYSTICK_VRY 27

hardware::led_matrix matrix{};
hardware::joy_stick joyStick{ PIN_JOYSTICK_BUTTON,
                              PIN_JOYSTICK_VRX,
                              PIN_JOYSTICK_VRY };

software::game game{ matrix, joyStick };

bool gameActive = false;


void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup for random numbers
  randomSeed(analogRead(A0));

  game.init();

  gameActive = true;

  Serial.begin(9600);
  Serial.println("Finished Setup");
}

void loop() {
  if (gameActive) {
    if (!game.exec()) {
      Serial.println("You lose!");
      gameActive = false;
      delay(2000);
    }
    delay(300);
  }
}