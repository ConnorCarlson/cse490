#include <Adafruit_LIS3DH.h>
#include "Platform.h"
#include "Character.h"
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

Adafruit_SSD1306 display(128, 64, &Wire, 4);
Adafruit_LIS3DH acc = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

Platform linesO[128];
Character c(32, 64, 0, 0);
const int gravity = 3;



void setup() {
  Serial.begin(9600);
  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.setRotation(3);
  acc.begin(0x19);
  for ( int i = 0; i < 128; i++) {
    genLines();
  }
}

void loop() {
  display.clearDisplay();
  acc.read();
  sensors_event_t event;
  acc.getEvent(&event);

  Serial.println(c.getY());

  c.setVX(-event.acceleration.y * 1.5);
  c.setVY(gravity + c.getVY());

  c.setX(c.getX() + c.getVX());
  c.setY(c.getY() + c.getVY());


  drawLines();
  if (c.getY() + c.getHeight() >= 128 || (checkBounce() && c.getVY() > 0)) {
//    c.setY(128 - c.getHeight());
    c.setVY(-15);
  }
  if (c.getY() < 64) {
    for (int i = 0; i < 64 - c.getY(); i++) {
      genLines();
    }
    c.setY(64);
  }

  c.setX(c.getX() % 64);


  drawCharacter(c.getX(), c.getY());
  if (c.getX() > 64 - c.getWidth()) {
    drawCharacter((c.getX() + c.getWidth()) % 64, c.getY());
  } else if (c.getX() < 0) {
    drawCharacter(64 + c.getX(), c.getY());
  }

  //  for(int i = 0; i < 128; i++) {
  //    for(int j = 0; j < 64; j++) {
  //      display.drawPixel(j, i, SSD1306_WHITE);
  //      display.display();
  //      delay(200);
  //    }
  //  }

  display.display();

  delay(10);

}

void genLines() {
  for (int i = 127; i > 0; i--) {
    linesO[i] = linesO[i - 1];
  }

  if (random(5) == 0) {
    int curSize = random(10, 20);
    linesO[0].updateX(random(64 - curSize - 1));
    linesO[0].updateSize(curSize);
  } else {
    linesO[0].updateX(0);
    linesO[0].updateSize(0);
  }

}

void drawLines() {
  for (int i = 0; i < 128; i++) {
    if (linesO[i].getSize() > 0) {
      display.drawLine(linesO[i].getX(), i, linesO[i].getX() + linesO[i].getSize(), i, SSD1306_WHITE);
    }

  }
}

void drawCharacter(int cx, int cy) {
  int circCenterX = cx + 5;
  int circCenterY = cy + 5;
  display.fillCircle(circCenterX, circCenterY, 5, 1);
  display.fillRect(circCenterX - 5, circCenterY, 11, 8, 1 );
  display.fillCircle(circCenterX, circCenterY, 4, 0);
  display.fillRect(circCenterX - 4, circCenterY, 9, 7, 0 );
  display.drawLine(circCenterX - 3, circCenterY + 8, circCenterX - 3, circCenterY + 11, 1 );
  display.drawLine(circCenterX + 3, circCenterY + 8, circCenterX + 3, circCenterY + 11, 1 );
}

boolean checkBounce() {

  int yCheck = c.getY() + c.getHeight() + 1;
  int yCheckStart = yCheck - c.getVY();
  for (int k = yCheckStart; k < yCheck; k++) {
    for (int i = linesO[k].getX(); i < linesO[k].getX() + linesO[k].getSize(); i++) {
      for (int j = c.getX(); j < c.getX() + c.getWidth(); j++) {
        int jCheck = j;
        if(j < 0) {
          jCheck = 64 + j;
        }
        if(j > 64) {
          jCheck = j-64;
        }
        
        if (i == jCheck) {
            c.setY(k-c.getHeight());
            return true;
        }
      }
    }
  }
  return false;

}
