/*      display biopotential distribution on 16x16 rgb led matrix 
        Projekt Medizinische Elektronik WS2023 HTWK Leipzig Prof. Laukner
        Joris Bakker, Julius Zeng, Luis Klaus 20EIB
        (c) Julius Zeng 04.12.2023
*/      

#include <Arduino.h>
#include <sstream>
#include "game.hpp"
#include "joy_stick.hpp"
#include "led_matrix.hpp"

hardware::led_matrix matrix{};

hardware::joy_stick joyStick{22, 26, 27};

software::game game{matrix, joyStick};

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup for random numbers
  randomSeed(analogRead(A0));

  digitalWrite(LED_BUILTIN, HIGH);
  
  game.init();

  joyStick.init();

  Serial.begin(9600);
  Serial.println("Finished Setup");
}

void loop(){
  if(!game.exec())
    Serial.println("You lose!");
  delay(300);
  
  /*
  for(uint i = 0; i < MATRIX_WIDTH; i++) {
    matrix.setPixel(0, i, COLOR_FOOD);
    matrix.outputMatrix();
    delay(1000);
  }
  matrix.clearMatrix();
  matrix.outputMatrix();
  

  for(uint i = 0; i < MATRIX_WIDTH; i++) {
    for(uint j = 0; j < MATRIX_HEIGHT; j++) {
      matrix.setPixel(i, j, COLOR_FOOD);
      
      Serial.write("x: ");
      Serial.print(i);
      Serial.write(" y: ");
      Serial.print(j);
      Serial.println();
      
      matrix.outputMatrix();
      delay(20);
    }
  }

  matrix.clearMatrix();
  matrix.outputMatrix();

  Serial.print("GetDirection: ");
  Serial.println(GetDirection());
  Serial.print("button: ");
  Serial.println(digitalRead(BUTTON_PIN));
  */
}


/*
  uint16_t x = joyStick.getX();
  uint16_t y = joyStick.getY();

  Serial.print("x = ");
  Serial.print(x);
  Serial.print("   y = ");
  Serial.print(y);
  Serial.print("  Direction: ");
  switch(joyStick.getDirection()) {
    case hardware::joy_stick::direction::left:
      Serial.print("left");
      break;
    case hardware::joy_stick::direction::up:
      Serial.print("up");
      break;
    case hardware::joy_stick::direction::down:
      Serial.print("down");
      break;
    case hardware::joy_stick::direction::right:
      Serial.print("right");
      break;
    case hardware::joy_stick::direction::noDirection:
      Serial.print("no direction");
      break;
  }
  Serial.print("  Distance: ");
  Serial.print(joyStick.isOutOfDetectRadius(x, y));
  Serial.println("");
  */