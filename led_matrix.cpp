#include "FastLED.h"

#include "led_matrix.hpp"

bool hardware::pixelCoordinate::increaseX() {
  if(x_ + 1 >= MATRIX_WIDTH)
    return false;
  ++x_;
  return true;
}

bool hardware::pixelCoordinate::increaseY() {
  if(y_ + 1 >= MATRIX_HEIGHT)
    return false;
  ++y_;
  return true;
}

bool hardware::pixelCoordinate::decreaseX() {
  if(x_ == 0)
    return false;
  --x_;
  return true;
}

bool hardware::pixelCoordinate::decreaseY() {
  if(y_ == 0)
    return false;
  --y_;
  return true;
}



hardware::led_matrix::led_matrix() : brightness_(BRIGHTNESS),
                                     ledLine_(0)
                                  //newRefresh_(true),
                                  //modeTime_(0),
                                  //time_(0),
                                  //frame_(0),
                                  //radFrame_(0),
                                  //betFrame_(0),
                                  //tracerNow_(0)
{
  
}

hardware::led_matrix::led_matrix(led_matrix &t) : brightness_(t.brightness_),
                                                  ledLine_(t.ledLine_)
{
  
}

hardware::led_matrix::~led_matrix() {

}

//##### conversion functions #####
//(coordinates)
uint hardware::led_matrix::Xm(byte m){ //returns X value of Output:leds_[m]
  byte row = (m / MATRIX_WIDTH);
  if(row % 2){  //row is odd
    return ((((((1+row)*MATRIX_WIDTH)- m)-1)*2)-15);
  }
  else{         //row is even
    return (((m - (row*MATRIX_WIDTH))*2)-15);
  }
}

uint hardware::led_matrix::Ym(byte m){ //returns Y value of Output:leds_[m]
  if(0 <= m <= NUM_LEDS-1){
    return (((m / MATRIX_WIDTH)*2)-15);
  }
}

byte hardware::led_matrix::Mxy(uint8_t x, uint8_t y){ //returns leds_[m] value of coordinates (reverse function of Xm/Ym)
  bool reverserow = false;
  if(y % 2){  //row is odd and needs to be reversed
    x = MATRIX_WIDTH - x -1;
  }
  return (y * MATRIX_WIDTH + x);
}

/*
//(pixel values) Hue:0red 96green 160blue | Sat:  0white 255color | Val(brighness: 0off 255full)
byte hardware::led_matrix::Value2Hue(float Value, double MaxValue){ //converts potential value to pixel hue
  float NormValue = abs(Value)/MaxValue;
  if (NormValue > 1){
    NormValue = 1;
  }
  if(Value >= 0){
    return (80 -(NormValue*80));
  }
  else{
    return (80 +(NormValue*80));
  }
}

byte hardware::led_matrix::Value2HSValue(float Value, double MaxValue){ //converts potential value to pixel HSValue
  float NormValue = abs(Value)/MaxValue;
    if (NormValue > 1){
      NormValue = 1;
    }
    return (NormValue*255);
}
*/

void hardware::led_matrix::init() {
  if(initailized_)
    return;
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds_, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  for(CRGB &led : leds_) {
    led = CHSV(0, 0, 0);
  }
  FastLED.show();
  initailized_ = true;
}

//##### visualisation functions #####

void hardware::led_matrix::setPixel(uint8_t x, uint8_t y, colorPixel color) {
  if(!initailized_)
    return;
  if(x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;
  leds_[Mxy(x, y)] = CHSV(color, 255, brightness_);
}

void hardware::led_matrix::unsetPixel(uint8_t x, uint8_t y) {
  if(!initailized_)
    return;
  if(x > MATRIX_WIDTH || y > MATRIX_HEIGHT)
    return;
  leds_[Mxy(x, y)] = CHSV(0, 0, 0);
}

void hardware::led_matrix::plotLine(int x0, int y0, int x1, int y1, byte hue){ //plots line between 2 coordinates + hue
  if(!initailized_)
    return;
  int  dx = x1 - x0; //int
  int  dy = y1 - y0; //int
  byte  NPixel;   //number of all Pixels per Line
  byte  NSteps;   //number of steps per Line
  bool    PlotX;
  bool    UpNPixel = false;
  bool    UpNSteps = false;

  if(abs(dx) >= abs(dy)){  //x>y
    //Serial.println("x>y Horizontal");
    NPixel = abs(dx)+1; //+1
    NSteps = abs(dy)+1; //+1
    PlotX  = true;
    if(dx > 0){UpNPixel = true;}
    if(dy > 0){UpNSteps = true;}
  }
  else{                   //y>x
    //Serial.println("y>x Vertikal");
    NPixel = abs(dy)+1; //+1
    NSteps = abs(dx)+1; //+1
    PlotX  = false;
    if(dx > 0){UpNSteps = true;}
    if(dy > 0){UpNPixel = true;}
  }
  int  XNow = x0; //int
  int  YNow = y0; //int
  byte    PixelNow  = 1;
  byte    StepNow   = 1;
  double  RatioStart  = NPixel/NSteps;
  double  RatioNow;
  byte    StepNrm;  //normal lengh of step
  byte    StepOdd;  //odd lengh of step
  byte    StepLengh;
  byte    NStepsNrm;//number of normal steps per Line
  byte    NStepsOdd;//number of odd steps per Line
  if(NPixel % NSteps){  //--> odd Steps required
    StepNrm = NPixel/NSteps +1;
    StepOdd = StepNrm -1;
  }
  else{                 //--> no odd steps
    StepNrm = NPixel/NSteps;
  }
  for(byte Step = 1; Step <= NSteps; Step++){
    if(PixelNow <= NPixel){  //redundant glaube
      RatioNow = ((NPixel+1) - PixelNow)/(NSteps+1 - Step);
      if((RatioNow/RatioStart)>=1){StepLengh = StepNrm;}
      else{StepLengh = StepOdd;}
      for(byte StepPixel = 1 ; StepPixel <= StepLengh; StepPixel++){
        ledLine_ = Mxy(XNow, YNow);
        leds_[ledLine_] = CHSV(hue, 255, 255);
        if(PlotX){
          if(UpNPixel){XNow++;}
          else{XNow--;}
        }
        else{
          if(UpNPixel){YNow++;}
          else{YNow--;}
        }
        PixelNow++;
      }
    }
    if(PlotX){
      if(UpNSteps){YNow++;}
      else{YNow--;}
    }
    else{
      if(UpNSteps){XNow++;}
      else{XNow--;}
    }
    StepNow++;
    RatioNow = ((NPixel-PixelNow) / (NSteps-StepNow));
  }
}


/*
void hardware::led_matrix::tracer(int Led, int hue, int maxval){ //plots fading tracer of leds_[m]  maxval is the maxbrightness of tracerled tracerMemory_ 
  tracerMemory_[tracerNow_] = Led;
  int TracerMemNow = tracerNow_;
  if(maxval <=0 || maxval > 255){
    maxval = 255;
  }
  for(int n = tracerLength_; n > 0; n--){
    TracerMemNow--;
    if(TracerMemNow < 0){
      TracerMemNow = tracerLength_ - 1;
    }
    double multiplyer = n; //double/float needed
    int val = ((maxval - 50) * (multiplyer/tracerLength_)) + 50; //-+ offset 
    leds_[tracerMemory_[TracerMemNow]]  = CHSV( hue, 255, val);
  }
  tracerNow_++;
  if(tracerNow_ >= tracerLength_){
    tracerNow_ = 0;
  }
}

void hardware::led_matrix::resetTracer(){ //sets tracerMemory_ to 0,0
  for(int n = 0; n < tracerLength_; n++){
    tracerMemory_[n] = Mxy(0,0);
  }
  ledLine_ = Mxy(0,0);
}
*/
void hardware::led_matrix::outputMatrix() {
  if(!initailized_)
    return;
  FastLED.show();
}

void hardware::led_matrix::clearMatrix() {
  if(!initailized_)
    return;
  for(CRGB &pixel : leds_) {
    pixel = 0x000000;
  }
  /*
  for(byte i = 0; i < NUM_LEDS; i++) {
    leds_[i] = 0x000000;
  }
  */
}

void hardware::led_matrix::test() {
  if(!initailized_)
    return;
  leds_[16] = CHSV(COLOR_FOOD, 255, brightness_);
}
