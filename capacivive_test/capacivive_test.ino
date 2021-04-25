#include <CapacitiveSensor.h>

CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2); // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
const int numReadings = 10;
const int OUTPUT_PIN = 6;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;

void setup(){

  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example Serial.begin(9600);
  Serial.begin(9600);

}

void loop(){

  long start = millis();
  
//  long total1 = cs_4_2.capacitiveSensor(30);

  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = cs_4_2.capacitiveSensor(30);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  
  Serial.print(millis() - start); // check on performance in milliseconds
  
  Serial.print("\t"); // tab character for debug window spacing
  
  Serial.println(average); // print sensor output 1

  average = average < 10 ? 0 : average - 10;

  int ledPower = average > 255 ? 255 : average;

  analogWrite(OUTPUT_PIN, ledPower);
  
  delay(10); // arbitrary delay to limit data to serial port

}
