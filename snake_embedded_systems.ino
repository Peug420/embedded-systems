/*      display biopotential distribution on 16x16 rgb led matrix 
        Projekt Medizinische Elektronik WS2023 HTWK Leipzig Prof. Laukner
        Joris Bakker, Julius Zeng, Luis Klaus 20EIB
        (c) Julius Zeng 04.12.2023
*/      

#include "game.hpp"
#include "joy_stick.hpp"
#include "led_matrix.hpp"

hardware::led_matrix matrix{};
hardware::joy_stick joyStick{22, 26, 27};

software::game game{matrix, joyStick};

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

void loop(){
  if(gameActive) {
    if(!game.exec()) {
      Serial.println("You lose!");
      gameActive = false;
      delay(2000);
    }
    delay(300);
  }
}