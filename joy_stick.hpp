#ifndef __joy_stick_header_included__
#define __joy_stick_header_included__
#include <sys/_stdint.h>
#include <Arduino.h>

namespace hardware {

class joy_stick {
  private:
    // pins
    const uint8_t buttonPin_;
    const uint8_t vrxPin_;
    const uint8_t vryPin_;

    // resolution
    const uint16_t centerPoint_{2047};
    const uint16_t centerDetectRadius_{1000};

    // setup
    bool initialized_{false};

    bool isOutOfDetectBand(const int32_t &val);
    bool isOutOfDetectRadius(uint32_t x, uint32_t y); 

  public:
    joy_stick(uint8_t _buttonPin, uint8_t _vrxPin, uint8_t _vryPin);
    joy_stick(joy_stick &t);
    ~joy_stick();

    enum direction {
      noDirection,
      up,
      down,
      left,
      right
    };

    void init();

    direction getDirection();
    bool isPressed();
};
}

#endif // __joy_stick_header_included__
