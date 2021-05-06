#include <Adafruit_LIS3DH.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// Create the display object
Adafruit_SSD1306 _display(128, 64, &Wire, 4);
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

const int OUTPUT_PIEZO_PIN = 9; // hook up one piezo leg to GND, other leg to Pin 9

// Ball global variables
const int _ballRadius = 5;
int _xBall = 5;  // x location of the ball
int _yBall = 5;  // y location of the ball
int _xBallPrev = 5;  // x location of the ball
int _yBallPrev = 5;  // y location of the ball
int _xSpeed = 0; // x speed of ball (in pixels per frame)
int _ySpeed = 0; // y speed of ball (in pixels per frame)
int _xBounce = 0;
int _yBounce = 0;

void setup() {
  Serial.begin(9600);
  // Initialize the display
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  lis.begin(0x19);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);

}

void loop() {

  
  lis.read();      // get X Y and Z data at once
  sensors_event_t event;
  lis.getEvent(&event);


  Serial.println();

  _xSpeed = event.acceleration.x + _xSpeed;
  _ySpeed = -event.acceleration.y + _ySpeed;
  
  // Clear the display
  _display.clearDisplay();

  _xBallPrev = _xBall;
  _xBall += _xSpeed;
  if(_xBall + _ballRadius >= _display.width()) {
    _xBall = _display.width() - _ballRadius;
  }
  if(_xBall - _ballRadius <= 0) {
    _xBall = _ballRadius;
  }

  _yBallPrev = _yBall;
  _yBall += _ySpeed;
  if(_yBall + _ballRadius >= _display.height()) {
    _yBall = _display.height() - _ballRadius;
  }
  if(_yBall - _ballRadius <= 0) {
    _yBall = _ballRadius;
  }
  
  
  if((_xBall - _ballRadius <= 0 && _xSpeed < 0) || (_xBall + _ballRadius >= _display.width() && _xSpeed > 0)){
    _xSpeed = _xSpeed*-1; // reverse x direction
    _xSpeed = _xSpeed > 1 ? _xSpeed - 1 : _xSpeed + 1;

    

    if(_xBallPrev != _xBall) {
      for(int i = 300; i <= 380; i += 4) {
        tone(OUTPUT_PIEZO_PIN, i);
        delay(1);
      }
      noTone(OUTPUT_PIEZO_PIN);
    }
    
    
  }
  
  if((_yBall - _ballRadius <= 0 && _ySpeed < 0) || (_yBall + _ballRadius >= _display.height() && _ySpeed > 0)){
    _ySpeed = _ySpeed*-1; // reverse y direction
    _ySpeed = _ySpeed > 1 ? _ySpeed - 1 : _ySpeed + 1;

    if(_yBallPrev != _yBall) {
      for(int i = 300; i <= 380; i += 4) {
        tone(OUTPUT_PIEZO_PIN, i);
        delay(1);
      }
      noTone(OUTPUT_PIEZO_PIN);
    }
    
  }


  // Draw circle
  _display.drawCircle(_xBall, _yBall, _ballRadius, SSD1306_WHITE);
  
  // Render buffer to screen
  _display.display();

  delay(20);
  
}
