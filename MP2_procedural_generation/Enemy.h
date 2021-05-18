#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

class Enemy{

  private:
    const byte width = 9;
    const byte height = 9;
    int y;
    int x;
    
  public:
    // Constructor
    Enemy() {
      x = -1000;
      y = -1000;
    };


    void updatePos(int newX, int newY) {
      x = newX;
      y = newY;
    }

    int getX() {
      return x;
    }

    int getY() {
      return y;
    }

    void setX(int newX) {
      x = newX;
    }

    void setY(int newY) {
      y = newY;
    }

    int getWidth() {
      return width;
    }

    int getHeight() {
      return height;
    }
};
