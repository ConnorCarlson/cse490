#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

class Bullet {

  private:
    const byte width = 5;
    const byte height = 5;
    int y;
    int x;
    int velocityX;
    int velocityY;

  public:
    // Constructor
    Bullet() {
      x = -1000;
      y = 1000;
      velocityX = 0;
      velocityY = 0;
    };


    void updatePos(int newX, int newY) {
      x = newX;
      y = newY;
    }
    void updateVel(int newVX, int newVY) {
      velocityX = newVX;
      velocityY = newVY;
    }

    int getVX() {
      return velocityX;
    }

    int getVY() {
      return velocityY;
    }

    void setVX(int vx) {
      velocityX = vx;
    }

    void setVY(int vy) {
      velocityY = vy;
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
    void move() {
      x = x + velocityX;
      y = y + velocityY;

      if (y > 128 || y + width < 0 || x > 64 || x + width < 0) {
        x = -1000;
      }


    }
};
