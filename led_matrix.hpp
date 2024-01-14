#include <cstdint>
#include <sys/_stdint.h>
#ifndef __led_matrix_header_included__
#define __led_matrix_header_included__

#include <FastLED.h>          // official FastLed 3.6.0 release doesnt work (missing uno r4 minima support)
                              // #1523 from facchinm (bit buggy but works) (04.12.2023)--> https://github.com/FastLED/FastLED/tree/0398b9a99901d00044de821ed86e8537995f561b 
#define LED_PIN       5       //5   LED Matrix
#define COLOR_ORDER   GRB     //GRB     breaks if changed
#define CHIPSET       WS2812B //WS2812B breaks if changed
#define BRIGHTNESS    50     //60 normal, 5 testing, 255 maximum (too bright)
#define MATRIX_WIDTH  16      //16      breaks if changed
#define MATRIX_HEIGHT 16      //16      breaks if changed
#define NUM_LEDS      (MATRIX_WIDTH * MATRIX_HEIGHT)
#define TRACER_LENGTH 100

#define COLOR_TAIL    64
#define COLOR_HEAD    160
#define COLOR_FOOD    96
#define COLOR_OBSTAC  0

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
  private:
    //static constexpr int tracerLength_ = 100;//30 100
    //const int refreshTime_   = 50; //50 ms per frame --> 20FPS (Stable)  [mode 0(potential) --> ~47ms/frame] [mode 1(vector) --> ~11ms/frame]

    byte brightness_;
    bool initailized_{false};

    //bool  newRefresh_;
    //unsigned long modeTime_;
    //unsigned long time_;

    //int   frame_; //current frame
    //float radFrame_;
    //float betFrame_;

    //int tracerNow_;
    int ledLine_;

    CRGB leds_[NUM_LEDS];
    //int tracerMemory_[tracerLength_]; //memoryarray of vector tracer

    //##### conversion functions #####
    //(coordinates)
    uint Xm(byte m); //returns X value of Output:leds_[m]
    uint Ym(byte m); //returns Y value of Output:leds_[m]

    byte Mxy(uint8_t x, uint8_t y); //returns leds_[m] value of coordinates (reverse function of Xm/Ym)

    //(pixel values) Hue:0red 96green 160blue | Sat:  0white 255color | Val(brighness: 0off 255full)
    //byte Value2Hue(float Value, double MaxValue); //converts potential value to pixel hue
    //byte Value2HSValue(float Value, double MaxValue); //converts potential value to pixel HSValue

  public:
    led_matrix();
    led_matrix(led_matrix &t);
    ~led_matrix();
    

    enum colorPixel{
      tail     = 64,
      head     = 160,
      food     = 96,
      obstacle = 0
    };
    
    void init();

    //##### visualisation functions #####

    void setPixel(uint8_t x, uint8_t y, colorPixel color);
    void unsetPixel(uint8_t x, uint8_t y);
    void plotLine(int x0, int y0, int x1, int y1, byte hue); //plots line between 2 coordinates + hue

    //void tracer(int Led, int hue, int maxval); //plots fading tracer of leds_[m]  maxval is the maxbrightness of tracerled tracerMemory_ 
    //void resetTracer(); //sets tracerMemory_ to 0,0

    void outputMatrix();
    void clearMatrix(); //sets all matrix values to 0

    void test();
};
}

#endif // __led_matrix_header_included__