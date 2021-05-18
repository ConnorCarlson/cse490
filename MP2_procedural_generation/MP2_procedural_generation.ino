#include <Adafruit_LIS3DH.h>
#include "Platform.h"
#include "Character.h"
#include "Enemy.h"
#include "Bullet.h"
#include <Adafruit_GFX.h>
//#include <Adafruit_GrayOLED.h>
//#include <Adafruit_SPITFT.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <gfxfont.h>
#include <ParallaxJoystick.hpp>
//#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_SSD1306.h>
//#include <splash.h>

Adafruit_SSD1306 display(128, 64, &Wire, 4);
Adafruit_LIS3DH acc = Adafruit_LIS3DH();
ParallaxJoystick joystick(A0, A1, 1023, UP);
Platform linesO[128];
Character c(32, 64, 0, 0);
const int gravity = 1;
const int enemyWidth = 9;
Enemy enemies[3];
Bullet bullets[5];
int bulletDelay = 10;
const int restingUpDown = 1850;
const int restingLeftRight = 1850;
int mode = 0;

int timeSinceLastLine = -5;
int score = 0;

const int PWM_CHANNEL = 0;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8; 
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);
const int VIB_OUTPUT_PIN = 21;

const int TONE_OUTPUT_PIN = 17;
const int TONE_PWM_CHANNEL = 10; 
const int TONE_FREQ = 8000;

int enemyKillTime = 0;

int bounceTime = 0;

int dieTime = 0;

int shootTime = 0;

void setup() {
  Serial.begin(9600);

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(TONE_PWM_CHANNEL, TONE_FREQ, 12);
  ledcAttachPin(VIB_OUTPUT_PIN, PWM_CHANNEL);
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.setRotation(3);
  acc.begin(0x18);
}

void resetGame() {
  c.updatePos(32, 64);
  c.updateVel(0,0);
  for(int i = 0; i < 3; i++) {
    enemies[i].setX(-1000);
  }
  for(int i = 0; i < 5; i++) {
    bullets[i].setX(-1000);
  }
  
  score = 0;
  timeSinceLastLine = -5;

  for ( int i = 0; i < 128; i++) {
    genLines();
  }
  linesO[125].updateX(0);
  linesO[125].updateSize(64);
}

void loop() {
  enemyKillUpdate();
  bounceUpdate();
  dieUpdate();
  shootUpdate();
  if (mode == 0) {
    startState();
  } else if (mode == 1) {
    gameState();
  } else {
    endState();
  }
  delay(10);

}

void enemyKillUpdate() {
  enemyKillTime = enemyKillTime -1 < 0 ? 0 : enemyKillTime -1;
  if(turnOffHaptics()) {
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
  } else {
    ledcWriteTone(TONE_PWM_CHANNEL, map(enemyKillTime, 0, 7, 700, 500));
  }
}

void enemyKillStart() {
  enemyKillTime = 7;
}

void bounceUpdate() {
  bounceTime = bounceTime -1 < 0 ? 0 : bounceTime -1;
  if(turnOffHaptics()) {
    ledcWrite(PWM_CHANNEL, 0);
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
  } else {
    Serial.println("bounce sound");
    ledcWrite(PWM_CHANNEL, 100);
    ledcWriteTone(TONE_PWM_CHANNEL, 700);
  }
}

void bounceStart() {
  bounceTime = 2;
}

void dieUpdate() {
  dieTime = dieTime -1 < 0 ? 0 : dieTime -1;
  if(turnOffHaptics()) {
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
  } else {
    ledcWriteTone(TONE_PWM_CHANNEL, 900);
  }
}

void dieStart() {
  dieTime = 20;
}

void shootUpdate() {
  shootTime = shootTime -1 < 0 ? 0 : shootTime -1;
  if(shootTime == 0) {
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
  } else {
    ledcWriteTone(TONE_PWM_CHANNEL, map(shootTime, 0, 4, 600, 900));
  }
}

void shootStart() {
  shootTime = 4;
}

boolean turnOffHaptics() {
  return enemyKillTime == 0 && bounceTime == 0 && dieTime == 0 && shootTime == 0;
}

void startState() {
  drawTextScreen("LineBounce", "To start", "go right", "->");
}

void drawTextScreen(String text1, String text2, String text3, String text4) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  uint16_t textWidth, textHeight;
  int16_t x, y;


  display.getTextBounds(text1, 0, 0, &x, &y, &textWidth, &textHeight);

  display.setCursor(display.width() / 2 - textWidth / 2, display.height() / 2 - textHeight / 2);
  display.print(text1);

  display.getTextBounds(text2, 0, 0, &x, &y, &textWidth, &textHeight);
  display.setCursor(display.width() / 2 - textWidth / 2, display.height() / 2 + textHeight);
  display.print(text2);
  display.getTextBounds(text3, 0, 0, &x, &y, &textWidth, &textHeight);
  display.setCursor(display.width() / 2 - textWidth / 2, display.height() / 2 + textHeight * 2 + 5);
  display.print(text3);
  display.getTextBounds(text4, 0, 0, &x, &y, &textWidth, &textHeight);
  display.setCursor(display.width() / 2 - textWidth / 2, display.height() / 2 + textHeight * 3 + 10);
  display.print(text4);
  display.display();
  if (checkConfirm()) {
    resetGame();
    mode = 1;
  }
}



void endState() {
  drawTextScreen("Game Over", "Score:", String(score), "Again! ->");
}

boolean checkConfirm() {
  joystick.read();
  return joystick.getLeftRightVal() - restingLeftRight > 1000;
}

void gameState() {
  bulletDelay = bulletDelay - 1;
  if (bulletDelay < 0) {
    bulletDelay = 0;
  }

  display.clearDisplay();
  acc.read();
  sensors_event_t event;
  acc.getEvent(&event);

  //  Serial.println(event.acceleration.y);
  c.updateVel(-event.acceleration.y * 1.5, gravity + c.getVY());
  c.updatePos(c.getX() + c.getVX(), c.getY() + c.getVY());

  shoot();
  drawBullets();
  drawLines();
  drawEnemies();
  if (checkBounce()) {
    bounceStart();
    c.setVY(-8);
  }
  if (c.getY() + c.getHeight() >= 128 || checkEnemyCollision(c.getX(), c.getY(), true)) {
    dieStart();
    mode = 2;
  }

  if (c.getY() < 64) {
    for (int i = 0; i < 64 - c.getY(); i++) {
      score += 1;
      genLines();
      genEnemies();
    }
    c.setY(64);
  }
  if (c.getX() >= 64) {
    c.setX(c.getX() % 64);
  }
  if (c.getX() + c.getWidth() < 0) {
    c.setX(64 + c.getX());
  }

  drawCharacter(c.getX(), c.getY());
  if (c.getX() > 64 - c.getWidth()) {
    drawCharacter( ((c.getX() + c.getWidth()) - 64) - c.getWidth(), c.getY());
  } else if (c.getX() < 0) {
    drawCharacter(64 + c.getX(), c.getY());
  }
  moveBullets();

  printScore();

  display.display();
}

void genEnemies() {
  for (int i = 0; i < 3; i++) {
    if (enemies[i].getX() > -1000) {
      enemies[i].setY(enemies[i].getY() + 1);
      if (enemies[i].getY() > 128) {
        enemies[i].setX(-1000);
      }
    }
  }
  if (linesO[0].getSize() > -1) {
    int lineCenter = linesO[0].getX() + linesO[0].getSize() / 2;
    int enemySpot = enemySpotAvailable();
    if (lineCenter + enemyWidth / 2 < 64 && lineCenter - enemyWidth / 2 > 0 && enemySpot != -1 && (15 - score/100 < 0 ? 0 : random(15 - score/100)) == 0 ) {
      enemies[enemySpot].updatePos(lineCenter - enemyWidth / 2, enemyWidth * -1 - 1);
    }
  }
}

void drawEnemies() {
  for (int i = 0; i < 3; i++) {
    if (enemies[i].getX() > -1000) {
      drawEnemy(enemies[i].getX(), enemies[i].getY());
    }
  }
}

boolean lineInterfereWithEnemy(int x, int width) {
  for (int i = 0; i < 3; i++) {

    if (enemies[i].getX() > -1000 && collisionDetect(enemies[i].getX(), enemies[i].getY(), enemyWidth, enemyWidth, x, 0, width, 1)) {
      return true;
    }

  }
  return false;
}

int enemySpotAvailable() {
  for (int i = 0; i < 3; i++) {
    if (enemies[i].getX() == -1000) {
      return i;
    }
  }
  return -1;
}

void printScore() {
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(1, 1);
  display.print(String(score));

}

void genLines() {
  for (int i = 127; i > 0; i--) {
    linesO[i] = linesO[i - 1];
  }
  int curSize = random(5, 10);
  int curX = random(64 - curSize - 1);
  int difficulty = score / 100;
  int lineGap = -5;
  if (difficulty > 19) {
    lineGap = lineGap + (19 - difficulty) < -18 ? -18 : lineGap + (19 - difficulty);
    difficulty = 19;
  }

  if (timeSinceLastLine >= 0 && random(difficulty - timeSinceLastLine < 0 ? 0 : difficulty - timeSinceLastLine) == 0 && !lineInterfereWithEnemy(curX, curSize)) {
    timeSinceLastLine = lineGap;
    linesO[0].updateX(curX);
    linesO[0].updateSize(curSize);
  } else {
    timeSinceLastLine = timeSinceLastLine + 1;
    linesO[0].updateX(-1);
    linesO[0].updateSize(-1);
  }

}

void drawLines() {
  for (int i = 0; i < 128; i++) {
    if (linesO[i].getSize() > -1) {
      int x = linesO[i].getX();
      display.drawLine(x, i, x + linesO[i].getSize(), i, SSD1306_WHITE);
    }

  }
}

void shoot() {
  joystick.read();
  int upDownVal = joystick.getUpDownVal();
  int leftRightVal = joystick.getLeftRightVal();
  int availableBullet = getAvialableBullet();
  if ((abs(upDownVal - restingUpDown) > 500 || abs(leftRightVal - restingLeftRight) > 500) && bulletDelay == 0 && availableBullet != -1) {
    shootStart();
    bulletDelay = 10;
    float rad = atan2(leftRightVal - restingLeftRight, upDownVal - restingUpDown);
    int xVel = sin(rad) * 5;
    int yVel = -cos(rad) * 5;
    bullets[availableBullet].updatePos(c.getX() + 1, c.getY());
    bullets[availableBullet].updateVel(xVel, yVel);
  }


}

void moveBullets() {
  for (int i = 0; i < 5; i++) {
    if (bullets[i].getX() > -1000) {
      bullets[i].move();
      if (checkEnemyCollision(bullets[i].getX(), bullets[i].getY(), false)) {
        bullets[i].setX(-1000);
      }
    }
  }

}

void drawBullets() {
  for (int i = 0; i < 5; i++) {
    drawBullet(bullets[i].getX(), bullets[i].getY());
  }
}

int getAvialableBullet() {
  for (int i = 0; i < 5; i++) {
    if (bullets[i].getX() == -1000) {
      return i;
    }
  }
  return -1;
}

void drawCharacter(int cx, int cy) {
  int ccx = cx + 3;
  int ccy = cy + 3;
  display.fillCircle(ccx, ccy, 3, 1);
  display.fillRect(ccx - 3, ccy, 7, 4, 1 );
  display.fillCircle(ccx, ccy, 2, 0);
  display.fillRect(ccx - 2, ccy, 5, 3, 0 );
  display.drawLine(ccx - 2, ccy + 4, ccx - 2, ccy + 6, 1 );
  display.drawLine(ccx + 2, ccy + 4, ccx + 2, ccy + 6, 1 );
}

void drawBullet(int bx, int by) {
  int ccx = bx + 2;
  int ccy = by + 2;
  display.fillCircle(ccx, ccy, 2, 1);
}

void drawEnemy(int ex, int ey) {
  int ccx = ex + 4;
  int ccy = ey + 4;
  display.fillCircle(ccx, ccy, 2, 1);
  display.drawLine(ccx - 4, ccy, ccx + 4, ccy, 1);
  display.drawLine(ccx, ccy + 4, ccx, ccy - 4, 1);
  display.drawLine(ccx + 4, ccy + 4, ccx - 4, ccy - 4, 1);
  display.drawLine(ccx - 4, ccy + 4, ccx + 4, ccy - 4, 1);
  display.fillCircle(ccx, ccy, 1, 0);

}

boolean collisionDetect(int rect1X, int rect1Y, int rect1Width, int rect1Height, int rect2X, int rect2Y, int rect2Width, int rect2Height) {
  return (rect1X <= rect2X + rect2Width &&
          rect1X + rect1Width >= rect2X &&
          rect1Y < rect2Y + rect2Height &&
          rect1Y + rect1Height > rect2Y);
}

boolean checkEnemyCollision(int x, int y, boolean character) {
  for (int j = 0; j < 3; j++) {
    int enemyX = enemies[j].getX();
    int enemyY = enemies[j].getY();
    if (character) {
      boolean result = characterCollisionDetect(x, y, c.getHeight(), enemyX, enemyY, enemyWidth, enemyWidth);

      if (result) {
        return true;
      }
    } else {
      boolean result = collisionDetect(x, y, 5, 5, enemyX, enemyY, enemyWidth, enemyWidth);
      if (result) {
        enemies[j].setX(-1000);
        enemyKillStart();
        return true;
      }
    }


    
  }
  return false;
}

boolean characterCollisionDetect(int newX, int newY, int height, int otherX, int otherY, int otherWidth, int otherHeight) {
  boolean result = false;
  if (newX < 0) {
    result = collisionDetect(64 + newX, newY, c.getWidth(), height, otherX, otherY, otherWidth, otherHeight) || result;
    result = collisionDetect(0, newY, c.getWidth() + newX, height, otherX, otherY, otherWidth, otherHeight) || result;
  } else if (newX > 64 - c.getWidth()) {
    result = collisionDetect(newX, newY, c.getWidth(), height, otherX, otherY, otherWidth, otherHeight) || result;
    result = collisionDetect(0, newY, (newX + c.getWidth() - 64), height, otherX, otherY, otherWidth, otherHeight) || result;
  } else {
    result = collisionDetect(newX, newY, c.getWidth(), height, otherX, otherY, otherWidth, otherHeight) || result;
  }
  return result;
}

boolean checkBounce() {
  if (c.getVY() < 0) {
    return false;
  }
  int yCheck = c.getY() + c.getHeight() + 1;
  int yCheckStart = yCheck - c.getVY();
  for (int k = yCheckStart; k < yCheck; k++) {
    int newY = k;
    int newX = map(k, yCheckStart, yCheck, c.getX() - c.getVX(), c.getX());
    boolean result = characterCollisionDetect(newX, newY, 1, linesO[k].getX(), k, linesO[k].getSize(), 1);
    if (result) {
      c.setY(k - c.getHeight());
      return true;
    }

  }
  return false;

}
