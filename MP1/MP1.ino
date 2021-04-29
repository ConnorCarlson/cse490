#include <CapacitiveSensor.h>
#include <PushButton.h>

CapacitiveSensor csRed = CapacitiveSensor(4, 2);

CapacitiveSensor csGreen = CapacitiveSensor(8, 7);

CapacitiveSensor csBlue = CapacitiveSensor(13, 12);

CapacitiveSensor sensors[] = {csRed, csGreen, csBlue};

const int numReadings = 30;
const int RED_PIN = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN = 9;

int LEDS[] = {RED_PIN, GREEN_PIN, BLUE_PIN};

enum RGB{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};

int readings[3][numReadings];      // the readings from the analog input

int readIndex[] = {0, 0, 0};            // the index of the current reading
int total[] = {0, 0, 0};                // the running total
int average[] = {0, 0, 0};

int crossFadeValues[] = {255, 0, 0}; // Red, Green, Blue
const int defaultCrossFadeValues[] = {255, 0, 0};
enum RGB _curFadingUpColor = GREEN;
enum RGB _curFadingDownColor = RED;
const int FADE_STEP = 1;  
const int DELAY_MS = 10; // delay in ms between changing colors
const int MAX_COLOR_VALUE = 255;
const int MIC_INPUT_PIN = A0;
const int MAX_ANALOG_IN = 1024; //1024 with 5V, 676 with 3.3V on Arduino Uno
const int MAX_ANALOG_OUT = 255;
const int MODES = 3;
const int MODE_SWITCH_BUTTON = 3;
const int CHOOSE_COLOR_BUTTON = 6;
const int INPUT_LR_PIN = A1;
int mode = 0;

PushButton modeButton = PushButton(MODE_SWITCH_BUTTON);
PushButton colorButton = PushButton(CHOOSE_COLOR_BUTTON );

void setup() {

  csRed.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example Serial.begin(9600);
  csGreen.set_CS_AutocaL_Millis(0xFFFFFFFF);
  csBlue.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(INPUT_LR_PIN, INPUT);
  pinMode(MODE_SWITCH_BUTTON, INPUT_PULLUP);
  pinMode(CHOOSE_COLOR_BUTTON, INPUT_PULLUP);
  modeButton.setActiveLogic(LOW);
  colorButton.setActiveLogic(LOW);
  
  Serial.begin(9600);
  setColor(crossFadeValues[RED], crossFadeValues[GREEN], crossFadeValues[BLUE]);
  pinMode(MIC_INPUT_PIN, INPUT);

}

void loop() {
  modeButton.update();
  if(modeButton.isClicked()){
    mode = (mode+1)%3;
    Serial.println(mode);
  }

  if(mode == 0) {
    mode0();
  } else if(mode == 1) {
    mode1();
  } else {
    mode2();
  }
  
  delay(DELAY_MS);

}

void updateCrossFade(int* currValues, int step) {

  currValues[_curFadingUpColor] += step;
  currValues[_curFadingDownColor] -= step;

  

  // Check to see if we've reached our maximum color value for fading up
  // If so, go to the next fade up color (we go from RED to GREEN to BLUE
  // as specified by the RGB enum)
  // This fade code partially based on: https://gist.github.com/jamesotron/766994
  if(currValues[_curFadingUpColor] > MAX_COLOR_VALUE){
    currValues[_curFadingUpColor] = MAX_COLOR_VALUE;
    _curFadingUpColor = (RGB)((int)_curFadingUpColor + 1);

    if(_curFadingUpColor > (int)BLUE){
      _curFadingUpColor = RED;
    }
  }

  // Check to see if the current LED we are fading down has gotten to zero
  // If so, select the next LED to start fading down (again, we go from RED to 
  // GREEN to BLUE as specified by the RGB enum)
  if(currValues[_curFadingDownColor] < 0){
    currValues[_curFadingDownColor] = 0;
    _curFadingDownColor = (RGB)((int)_curFadingDownColor + 1);

    if(_curFadingDownColor > (int)BLUE){
      _curFadingDownColor = RED;
    }
  }
  
}

void writeColor(int step) {
  int currValues[] = {255,0,0};
  int _tempFadingUpColor=1;
  int _tempFadingDownColor=0;
  while(step >= 255){
    currValues[_tempFadingUpColor]=255;
    _tempFadingUpColor = (RGB)((int)_curFadingUpColor + 1);
    if(_tempFadingUpColor > (int)BLUE){
      _tempFadingUpColor = RED;
    }
    currValues[_tempFadingDownColor]=0;
    _tempFadingDownColor = (RGB)((int)_curFadingDownColor + 1);
    if(_tempFadingDownColor > (int)BLUE){
      _tempFadingDownColor = RED;
    }
    step -= 255;
  }

  currValues[_curFadingUpColor] += step;
  currValues[_curFadingDownColor] -= step;

  setColor(currValues[0], currValues[1], currValues[2]);
  
}

void mode0() {
  // Increment and decrement the RGB LED values for the current
  // fade up color and the current fade down color
  int lrInput = analogRead(INPUT_LR_PIN);
  int ledVal = 255 - map(lrInput, 0, 1023, 0, 255);
  
  updateCrossFade(crossFadeValues, FADE_STEP);
  int newValues[3];
  int max = 0;
  for (int i = 0; i <= 2; i++) {
    if(crossFadeValues[i] > max) {
      max = crossFadeValues[i];
    }
      
  }
  for (int i = 0; i <= 2; i++) {
    if(crossFadeValues[i] == max){ 
      newValues[i] = ledVal;
    } else {
      newValues[i] = (int)(ledVal*(double)((double)crossFadeValues[i]/(double)max));
    }
      
  }
//  Serial.println(ledVal);

//  Serial.print(newValues[0]);
//  Serial.print(", ");
//  Serial.print(newValues[1]);
//  Serial.print(", ");
//  Serial.println(newValues[2]);
  
  setColor(newValues[RED], newValues[GREEN], newValues[BLUE]);

  // Set the color and then delay
  
  
}

void mode1() {
  int max = 0;

  if(colorButton.isActive()){
    for (int i = 0; i <= 2; i++) {
    
      total[i] = total[i] - readings[i][readIndex[i]];
   
      // read from the sensor:
      readings[i][readIndex[i]] = sensors[i].capacitiveSensor(30);
      // add the reading to the total:
      total[i] = total[i] + readings[i][readIndex[i]];
      // advance to the next position in the array:
      readIndex[i] = readIndex[i] + 1;
  
      // if we're at the end of the array...
      if (readIndex[i] >= numReadings) {
        // ...wrap around to the beginning:
        readIndex[i] = 0;
      }
  
      // calculate the average:
      average[i] = total[i] / numReadings;
  
      if(average[i] > max) {
        max = average[i];
      }
    }
  
    for (int i = 0; i <= 2; i++) {
      if(average[i] == max){ 
        analogWrite(LEDS[i], 255);
      } else {
        analogWrite(LEDS[i], average[i] > MAX_ANALOG_OUT ? MAX_ANALOG_OUT : (int)(MAX_ANALOG_OUT*(double)((double)average[i]/(double)max)));
      }
      
    }
  }

  
}

void mode2() {
  // Read in current sound level from microphone
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
 
  while (millis() - startMillis < 100)
  {
     int sample = analogRead(MIC_INPUT_PIN); 
     if (sample < 1024)  // toss out spurious readings
     {
        if (sample > signalMax)
        {
            signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
           signalMin = sample;  // save just the min levels
        }
     }
  } 
  int step = signalMax - signalMin;
  Serial.println(step);
  writeColor(step);
}

void setColor(int red, int green, int blue)
{
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);  
}
