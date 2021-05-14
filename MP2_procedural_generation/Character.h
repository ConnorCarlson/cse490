#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

class Character{

  private:
    const int width = 11;
    const int height = 17;
    int y;
    int x;
    int velocityX;
    int velocityY;
    
  public:
    // Constructor
    Character(int charX, int charY, int charVX, int charVY) {
      x = charX;
      y = charY;
      velocityX = charVX;
      velocityY = charVY;
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
};
