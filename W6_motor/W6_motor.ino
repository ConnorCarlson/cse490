#include <PushButton.h>

class Buzzer{

  private:
    const int MOTOR_OUTPUT = 3;
    const unsigned long length;  // blink interval in ms
    int powerIndex;
    const int *power;                     // current state (either HIGH OR LOW)
    const int arrSize;
    unsigned long lastTime; // last state toggle in ms
    
  public:
    // Constructor
    Buzzer(int motorPowers[], int powerArrSize, unsigned long blinkInterval) :
      power(motorPowers), arrSize(powerArrSize), length(blinkInterval) // initialize const like this in C++
    {
      powerIndex = 0;
      lastTime = 0;
      Serial.begin(9600);
    }


    void update(){
      unsigned long currentTimestampMs = millis();
      
      if (currentTimestampMs - lastTime >= length) {
        Serial.println(power[powerIndex]);
        lastTime = currentTimestampMs;
        analogWrite(MOTOR_OUTPUT, power[powerIndex]);
        powerIndex = (powerIndex + 1) % (arrSize);
      }
    }
};


const int MOTOR_OUTPUT = 3;
const int BUTTON_PIN = 4;
int mode = 0;
const int maxModes = 5;
long delayT = 0;
PushButton b = PushButton(BUTTON_PIN);

int mode0BuzzerPowers[] = {255, 0};
Buzzer mode0Buzzer = Buzzer(mode0BuzzerPowers, 2, 1000);

int mode2BuzzerPowers[] = {255, 0, 255, 0, 255, 255, 255, 0, 0};
Buzzer mode2Buzzer = Buzzer(mode2BuzzerPowers, 9, 200);

int mode3BuzzerPowers[] = {50, 0};
Buzzer mode3Buzzer = Buzzer(mode3BuzzerPowers, 2, 2000);

int mode4BuzzerPowers[80];
Buzzer mode4Buzzer = Buzzer(mode4BuzzerPowers, 80, 100);
void setup() {
  pinMode(MOTOR_OUTPUT, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  b.setActiveLogic(LOW);

  for(int i = 0; i < 30; i++) {
    mode4BuzzerPowers[i] = i*3;
  }
  for(int i = 30; i < 40; i++) {
    mode4BuzzerPowers[i] = 0;
  }

  for(int i = 40; i < 70; i++) {
    mode4BuzzerPowers[i] = (70-i)*3;
  }

  for(int i = 70; i < 80; i++) {
    mode4BuzzerPowers[i] = 0;
  }

  

}

void loop() {
  b.update();
  if(b.isClicked()){
    mode = (mode + 1) % maxModes;
    Serial.println(mode);
  }
  
  if(mode == 0) {
    mode_0();
  } else if(mode == 1) {
    mode_1();
  } else if(mode == 2) {
    mode_2();
  } else if(mode == 3) {
    mode_3();
  } else {
    mode_4();
  }
}

void mode_0() {
  mode0Buzzer.update();
}

void mode_1() {
  int potVal = analogRead(A0);
  int anOut = map(potVal, 0, 1023, 0, 255);
  analogWrite(MOTOR_OUTPUT, anOut);
}

void mode_2() {
  mode2Buzzer.update();
}

void mode_3() {
  mode3Buzzer.update();
}

void mode_4() {
  mode4Buzzer.update();
}
