#ifndef __joy_stick_header_included__
#define __joy_stick_header_included__
#include <sys/_stdint.h>
#include <Arduino.h>

namespace hardware {

class joy_stick {
  private:
    /**
     * @brief The pin number to which the vrx pin from the physical joy stick
     *        is conntected to.
     * 
     */
    const uint8_t vrxPin_;
    /**
     * @brief The pin number to which the vry pin from the physical joy stick
     *        is conntected to.
     * 
     */
    const uint8_t vryPin_;

    // resolution
    /**
     * @brief The center of the ADC resolution. The raspberry pi pico has a
     *        a 10 bit by default and a 12 bit optionally. The 12 bit is used so
     *        the resolution ist 2^12 = 4096 of which 2047 is the half. This is
     *        used for calculating the dection radius and detection band.
     * 
     */
    const uint16_t centerPoint_{2047};
    /**
     * @brief The radius in which no input is detected. The radius is relative
     *        to the maximal resoultion 4096 of each axis. So a directional
     *        input will be detected only if in any x or y direction the value
     *        from the ADC is greater than the centerPoint_ +
     *        centerDetectRadius_ or less than centerPoint_ -
     *        centerDetectRadius_.
     * 
     */
    const uint16_t centerDetectRadius_{1000};

    // setup
    /**
     * @brief Flag for checking if init was called or not. No mehtod should
     *        be used before init() is called and initialized_ is set true.
     * 
     */
    bool initialized_{false};

    /**
      * @brief Around every axis, there is an inner band of detection. If the
      *       given ADC value is out of this band around the centerPoint_,
      *       the method returns true. For the band centerDetectRadius_ is
      *       used.
      *
      * @param val ADC value to check.
      * @return true Value is out of the detection band around the center.
      * @return false Value is inside of the detection band around the center.
      */
    bool isOutOfDetectBand(const int32_t &val);
    /**
     * @brief Checks if the cartesian coordiante lies outside of the circle
     *        with the radius centerDetectRadius_.
     * 
     * @param x X value of the coordinate.
     * @param y Y value of the coordinate.
     * @return true The coordinate is outside of the circle.
     * @return false The coordinate lies indside the circle.
     */
    bool isOutOfDetectRadius(uint32_t x, uint32_t y); 

  public:
    /**
     * @brief Construct a new joy stick object.
     * 
     * @param _vrxPin Pin number of the vrx pin.
     * @param _vryPin Pin number of the vry pin.
     */
    joy_stick(uint8_t _vrxPin, uint8_t _vryPin);
    /**
     * @brief Copy constructor for a new joy stick object.
     * 
     * @param t Object of joy_stick to copy from.
     */
    joy_stick(joy_stick &t);
    /**
     * @brief Destroy the joy stick object
     * 
     */
    ~joy_stick();

    /**
     * @brief Enum for the possible inputs of the joy stick.
     * 
     */
    enum direction {
      noDirection,
      up,
      down,
      left,
      right
    };

    /**
     * @brief Init function for this object. Need to be called shortly after
     *        initialisation.
     * 
     */
    void init();
    /**
     * @brief Get the current direction of the joy stick.
     * 
     * @return direction Enum of all possible inputs.
     */
    direction getDirection();
};
}

#endif // __joy_stick_header_included__
