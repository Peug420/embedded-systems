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
  /**
   * @brief Increases the x_ value by 1, but not higher than the MATRIX_HEIGHT.
   * 
   * @return true New value is not out of matrix dimension MATRIX_HEIGHT.
   * @return false New value is out of matrix dimension MATRIX_HEIGHT.
   */
  bool increaseX();
  /**
   * @brief Increases the y_ value by 1, but not higher than the MATRIX_WIDTH.
   * 
   * @return true New value is not out of matrix dimension MATRIX_WIDTH.
   * @return false New value is out of matrix dimension MATRIX_WIDTH.
   */
  bool increaseY();
  /**
   * @brief Decreases the x_ value by 1, but not less than 0.
   * 
   * @return true New value is not out of matrix dimension.
   * @return false New value is out of matrix dimension.
   */
  bool decreaseX();
  /**
   * @brief Decreases the y_ value by 1, but not less than 0.
   * 
   * @return true New value is not out of matrix dimension.
   * @return false New value is out of matrix dimension.
   */
  bool decreaseY();
};

inline bool operator==(const pixelCoordinate& first, const pixelCoordinate& second) {
  return (first.x_ == second.x_) && (first.y_ == second.y_); 
}

class led_matrix {
  protected:
    /**
     * @brief Brightness of the leds on the matrix. Should not be too low, to
     *        avoid weird side effects. Too high values could also lead to
     *        weird side effects or just not working matrix, due to not enough
     *        power delivery.
     * 
     */
    uint8_t brightness_{40};
    /**
     * @brief Is set true, when init() is called.
     * 
     */
    bool initailized_{false};
    /**
     * @brief Matrix of CRGB objects for setting the color values of the LEDs.
     * 
     */
    CRGB leds_[NUM_LEDS];
    /**
     * @brief Object of the FastLED Neomatrix library for controlling LEDs.
     *        This is used for higher graphical functionality like scrolling
     *        text for example.
     * 
     */
    FastLED_NeoMatrix *matrix_;

  public:
    /**
      * @brief Construct a new led matrix object.
      * 
      */
    led_matrix();
    /**
     * @brief Copy construct for a new led matrix object.
     * 
     * @param t Object to copy from.
     */
    led_matrix(led_matrix &t);
    /**
     * @brief Destroy the led matrix object
     * 
     */
    ~led_matrix();
    /**
     * @brief Enumeration of the default colors for every game pixel.
     * 
     */
    enum colorPixel{
      obstacle = CRGB::Red,
      tail     = CRGB::Yellow,
      food     = CRGB::Green,
      head     = CRGB::Blue
    };
    /**
     * @brief Init function for this object. Need to be called shortly after
     *        initialisation.
     * 
     */
    void init();

    //##### visualisation functions #####
    /**
     * @brief Set the color from the default colors of a coordinate.
     * 
     * @param x X value of the coordinate to set.
     * @param y Y value of the coordinate to set.
     * @param color Default color enum.
     */
    void setPixel(uint16_t x, uint16_t y, colorPixel color);
    /**
     * @brief Set the color of a coordinate.
     * 
     * @param x X value of the coordinate to set.
     * @param y Y value of the coordinate to set.
     * @param color Color value.
     */
    void setPixel(uint16_t x, uint16_t y, uint32_t color);
    /**
     * @brief Unsets the pixel by setting the color value to 0 making the LED
     *        black.
     * 
     * @param x X value of the coordinate to set.
     * @param y Y value of the coordinate to set.
     */
    void unsetPixel(uint16_t x, uint16_t y);
    /**
     * @brief Print text on the led matrix. Uses FastLED Neomatrix library.
     *        Text could wrap and scroll.
     * 
     * @param text Character array of the text to print.
     * @param x TODO where is the starting point?
     * @param y TODO
     * @param color TODO what does this option?
     * @param wrap If true, text wraps around edges.
     */
    void printText(const char *text, int16_t x, int16_t y, 
                    CRGB::HTMLColorCode color, bool wrap);
    /**
     * @brief Set the brightness of the led matrix.
     * 
     * @param brightn Brightness to set.
     */
    void setBrightness(uint8_t brightn);
    /**
     * @brief Outputs the content of the leds_ array to the physical led matrix.
     * 
     */
    void outputMatrix();
    /**
     * @brief Clears the content in leds_ matrix. If this should take effect on
     *        the physical matrix, outputMatrix() should be called aferwards.
     * 
     */
    void clearMatrix();
};
}

#endif // __led_matrix_header_included__