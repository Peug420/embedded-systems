/*      display biopotential distribution on 16x16 rgb led matrix 
        Projekt Medizinische Elektronik WS2023 HTWK Leipzig Prof. Laukner
        Joris Bakker, Julius Zeng, Luis Klaus 20EIB
        (c) Julius Zeng 04.12.2023
*/      
#include <FastLED.h>          // official FastLed 3.6.0 release doesnt work (missing uno r4 minima support)
                              // #1523 from facchinm (bit buggy but works) (04.12.2023)--> https://github.com/FastLED/FastLED/tree/0398b9a99901d00044de821ed86e8537995f561b 
#define LED_PIN       5       //5   LED Matrix
#define BUTTON_PIN    22      //22  |
#define VRX           26      //26  | Joystick
#define VRY           27      //27  |
#define COLOR_ORDER   GRB     //GRB     breaks if changed
#define CHIPSET       WS2812B //WS2812B breaks if changed
#define BRIGHTNESS    5     //60 normal, 5 testing, 255 maximum (too bright)
#define MATRIX_WIDTH  16      //16      breaks if changed
#define MATRIX_HEIGHT 16      //16      breaks if changed
#define NUM_LEDS      (MATRIX_WIDTH * MATRIX_HEIGHT)

//##### global constants #####

const int RefreshTime   = 50; //50 ms per frame --> 20FPS (Stable)  [mode 0(potential) --> ~47ms/frame] [mode 1(vector) --> ~11ms/frame]

const int TracerLength  = 100;//30 100

//##### global variables #####


bool  NewRefresh        = true;
unsigned long ModeTime  = 0;
unsigned long Time      = 0;

int   Frame     = 0; //current frame
float RadFrame  = 0;
float BetFrame  = 0;

int TracerNow   = 0;
int LedLine;

//##### global arrays #####
CRGB leds[NUM_LEDS];
int TracerMemory[TracerLength]; //memoryarray of vector tracer

//##### conversion functions #####
//(coordinates)
double Xm(byte m){ //returns X value of Output:leds[m]
  byte row = (m / MATRIX_WIDTH);
  if(row % 2){  //row is odd
    return ((((((1+row)*MATRIX_WIDTH)- m)-1)*2)-15);
  }
  else{         //row is even
    return (((m - (row*MATRIX_WIDTH))*2)-15);
  }
}

double Ym(byte m){ //returns Y value of Output:leds[m]
  if(0 <= m <= NUM_LEDS-1){
    return (((m / MATRIX_WIDTH)*2)-15);
  }
}

byte Mxy(double x, double y){ //returns leds[m] value of coordinates (reverse function of Xm/Ym)
  bool reverserow = false;
  byte row    = (y/2)+8;
  byte collum = (x/2)+8;
  if(row % 2){  //row is odd and needs to be reversed
    collum = MATRIX_WIDTH - collum -1;
  }
  return (row * MATRIX_WIDTH + collum);
}

//(pixel values) Hue:0red 96green 160blue | Sat:  0white 255color | Val(brighness: 0off 255full)
byte Value2Hue(float Value, double MaxValue){ //converts potential value to pixel hue
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

byte Value2HSValue(float Value, double MaxValue){ //converts potential value to pixel HSValue
  float NormValue = abs(Value)/MaxValue;
    if (NormValue > 1){
      NormValue = 1;
    }
    return (NormValue*255);
}

//##### visualisation functions #####

void plotLine(int x0, int y0, int x1, int y1, byte hue){ //plots line between 2 coordinates + hue
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
        LedLine = Mxy(XNow, YNow);
        leds[LedLine] = CHSV(hue, 255, 255);
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

void tracer(int Led, int hue, int maxval){ //plots fading tracer of leds[m]  maxval is the maxbrightness of tracerled TracerMemory 
  TracerMemory[TracerNow] = Led;
  int TracerMemNow = TracerNow;
  if(maxval <=0 || maxval > 255){
    maxval = 255;
  }
  for(int n = TracerLength; n > 0; n--){
    TracerMemNow--;
    if(TracerMemNow < 0){
      TracerMemNow = TracerLength - 1;
    }
    double multiplyer = n; //double/float needed
    int val = ((maxval - 50) * (multiplyer/TracerLength)) + 50; //-+ offset 
    leds[TracerMemory[TracerMemNow]]  = CHSV( hue, 255, val);
  }
  TracerNow++;
  if(TracerNow >= TracerLength){
    TracerNow = 0;
  }
}

void TracerReset(){ //sets TracerMemory to 0,0
  for(int n = 0; n < TracerLength; n++){
    TracerMemory[n] = Mxy(0,0);
  }
  LedLine = Mxy(0,0);
}


void ClearMatrix(){ //sets all matrix values to 0
  for (int x = 0; x < NUM_LEDS; x++)
  {
    leds[x]= 0x000000;;
  }
}

int GetDirection(){ //Returns Joystick User input 0 = No Input; 1 = Up; 2 = Down; 3 = Left; 4 = Right;
  int XVal = analogRead(VRX);
  int YVal = analogRead(VRY);

  int XMin = 0;
  int XMax = 1024;
  int YMin = 0;
  int YMax = 1024;
  /*
  Serial.print("VRX: ");
  Serial.println(analogRead(VRX));
  Serial.print("VRY: ");
  Serial.println(analogRead(VRY));
  */
  if(XVal >= 716){
    if((YVal >= 716) && (YVal > XVal)){
      return 1;//YVal up
    }
    if((YVal <= 307) && (YVal < (1024 - XVal))){
      return 2;//YVal down
    }
    return 3;//XVal left
  }
  if(XVal <= 307){
    if((YVal <= 307) && (YVal < XVal)){
      return 2;//YVal down
    }
    if((YVal >= 716) && (YVal > (1024 - XVal))){
      return 1;//YVal up
    }
    return 4;//XVal right

  }
  if(YVal >= 716){
    return 1;//YVal up
  }
  if(YVal <= 307){
    return 2;//YVal down
  }
  return 0;//no input
}

void setup() {
  pinMode(2, INPUT_PULLUP);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);
  Serial.println("end setup");
}

void loop(){
  /*
  Serial.print("GetDirection: ");
  Serial.println(GetDirection());
  Serial.print("button: ");
  Serial.println(digitalRead(BUTTON_PIN));
  */
  int color;
  ClearMatrix();
  if(digitalRead(BUTTON_PIN)){
    color = 96;
  }
  else{
    color = 0;
  }
  leds[GetDirection()] = CHSV(color, 255, 255);
  FastLED.show();
  delay(50);
}