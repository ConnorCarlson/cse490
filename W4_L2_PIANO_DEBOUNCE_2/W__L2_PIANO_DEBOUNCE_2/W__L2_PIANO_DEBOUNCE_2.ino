#include <CapacitiveSensor.h>
#include <PushButton.h>

#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz

const int DEBOUNCE_WINDOW = 40;

const int INPUT_BUTTON_C_PIN = 2;
const int INPUT_BUTTON_D_PIN = 3;
const int INPUT_BUTTON_E_PIN = 4;
const int INPUT_BUTTON_F_PIN = 5;
const int INPUT_BUTTON_G_PIN = 6;
const int INPUT_BUTTON_THERAMIN = 7;

const int OUTPUT_PIEZO_PIN = 9;
const int OUTPUT_LED_PIN = LED_BUILTIN; // visual feedback on button press
int _savedButtonVal = HIGH;
const int numReadings = 60;
CapacitiveSensor cs = CapacitiveSensor(11,10);
PushButton cButton = PushButton(INPUT_BUTTON_C_PIN);
PushButton dButton = PushButton(INPUT_BUTTON_D_PIN);
PushButton eButton = PushButton(INPUT_BUTTON_E_PIN);
PushButton fButton = PushButton(INPUT_BUTTON_F_PIN);
PushButton gButton = PushButton(INPUT_BUTTON_G_PIN);
PushButton tButton = PushButton(INPUT_BUTTON_THERAMIN);


int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
void setup() {
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_THERAMIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  cButton.setActiveLogic(LOW);
  dButton.setActiveLogic(LOW);
  eButton.setActiveLogic(LOW);
  fButton.setActiveLogic(LOW);
  gButton.setActiveLogic(LOW);
  tButton.setActiveLogic(LOW);
  Serial.begin(9600);

}

void loop() {
  cButton.update();
  dButton.update();
  eButton.update();
  fButton.update();
  gButton.update();
  tButton.update();

  
  if(cButton.isClicked()){
    Serial.println("click!");
    while(cButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, KEY_C);
      cButton.update();
    }
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  }else if(dButton.isClicked()){
    while(dButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, KEY_D);
      dButton.update();
    }
  }else if(eButton.isClicked()){
    while(eButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, KEY_E);
      eButton.update();
    }
  }else if(fButton.isClicked()){
    while(fButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, KEY_F);
      fButton.update();
    }
  }else if(gButton.isClicked()){
    while(gButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, KEY_G);
      gButton.update();
    }
  }else if(tButton.isClicked()) {
    int average = getAverage();
    Serial.println(average*5);
    while(tButton.isActive()){
      tone(OUTPUT_PIEZO_PIN, getAverage()*5);
      tButton.update();
    }
  }
  else{
    noTone(OUTPUT_PIEZO_PIN); // turn off the waveform
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }

}

int getAverage() {
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = cs.capacitiveSensor(30);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  return average;
}

boolean isButtonPressed(int btnPin){
  int btnVal = digitalRead(btnPin);

  delay(DEBOUNCE_WINDOW);

  // read the button value again
  int btnVal2 = digitalRead(btnPin);

  if(btnVal == btnVal2 && _savedButtonVal != btnVal){
    _savedButtonVal = btnVal;
  }
  
  if(_savedButtonVal == LOW){
    // button is hooked up with pull-up resistor
    // and is in a pressed state
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }

  // button is not pressed
  return false;
}
