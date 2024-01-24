
//#include <cstdint>
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
    uint8_t brightness_{40};

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
    void setPixel(uint16_t x, uint16_t y, uint32_t color);
    void unsetPixel(uint16_t x, uint16_t y);
    void setBrightness(uint8_t brightn);

    void outputMatrix();
    void clearMatrix(); //sets all matrix values to 0
};
}

#endif // __led_matrix_header_included__