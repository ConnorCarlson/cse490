#include <Adafruit_LIS3DH.h>
#include "Platform.h"
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
Adafruit_SSD1306 display(128, 64, &Wire, 4);
int lines[64][2] = {0};

void setup() {
   Serial.begin(9600);
  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D); 

}

void loop() {
  Serial.println("here");
  display.clearDisplay();


  genLines();
  drawLines();

  display.display();

  delay(10);

}

void genLines() {
  for(int i = 63; i > 0; i--) {
    lines[i][0] = lines[i-1][0];
    lines[i][1] = lines[i-1][1];
  }

  if(random(3) == 0) {
    lines[0][0] = random(128);
    lines[0][1] = random(10,20);
  } else {
    lines[0][0] = 0;
    lines[0][1] = 0;
  }
  
}

void drawLines() {
  for(int i = 0; i < 64; i++) {
    if(lines[i][1] > 0) {
      display.drawLine(lines[i][0], i, lines[i][0]+lines[i][1], i, SSD1306_WHITE);
    }
    
  }
}
