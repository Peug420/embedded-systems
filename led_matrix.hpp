#include <string>
#include <cstdint>
#include <sys/_stdint.h>
#ifndef __led_matrix_header_included__
#define __led_matrix_header_included__

#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>          // official FastLed 3.6.0 release doesnt work (missing uno r4 minima support)
                              // #1523 from facchinm (bit buggy but works) (04.12.2023)--> https://github.com/FastLED/FastLED/tree/0398b9a99901d00044de821ed86e8537995f561b                           
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define COLOR_ORDER   GRB     //GRB     breaks if changed
#define CHIPSET       WS2812B //WS2812B breaks if changed
#define LED_PIN       5
#define MATRIX_WIDTH  16
#define MATRIX_HEIGHT 16
#define NUM_LEDS      (MATRIX_HEIGHT * MATRIX_WIDTH)

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)   
#define LED_GREEN_LOW 		(15 << 5)  
#define LED_GREEN_MEDIUM 	(31 << 5)  
#define LED_GREEN_HIGH 		(63 << 5)  

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)


namespace hardware {

struct pixelCoordinate {
  uint8_t x_;
  uint8_t y_;
  bool increaseX();
  bool increaseY();
  bool decreaseX();
  bool decreaseY();
};

inline bool operator==(const pixelCoordinate& first, const pixelCoordinate& second) {
  return (first.x_ == second.x_) && (first.y_ == second.y_); 
}

class led_matrix {
  protected:
    uint8_t brightness_{80};
    uint8_t test_;

    bool initailized_{false};

    CRGB leds_[NUM_LEDS];
    FastLED_NeoMatrix *matrix_;

    //##### conversion functions #####
    //(coordinates)
    uint Xm(byte m); //returns X value of Output:leds_[m]
    uint Ym(byte m); //returns Y value of Output:leds_[m]

    byte Mxy(uint8_t x, uint8_t y); //returns leds_[m] value of coordinates (reverse function of Xm/Ym)

  public:
    led_matrix();
    led_matrix(led_matrix &t);
    ~led_matrix();
    

    enum colorPixel{
      obstacle = CRGB::Red,
      tail     = CRGB::Yellow,
      food     = CRGB::Green,
      head     = CRGB::Blue
    };
    
    void init();

    //##### visualisation functions #####

    void setPixel(uint16_t x, uint16_t y, colorPixel color);
    void setPixel(uint16_t x, uint16_t y, uint16_t color);
    void unsetPixel(uint16_t x, uint16_t y);
    void setBrightness(uint8_t brightn);

    void outputMatrix();
    void clearMatrix(); //sets all matrix values to 0
};
}

#endif // __led_matrix_header_included__