const int PIEZO_OUTPUT_PIN = 3;
const int LED_PINS[] = {6,9,10,11};
const int MAX_FREQ = 1000;
void setup()
{
  Serial.begin(9600); // for printing values to console
}

void loop()
{
  int potVal = analogRead(A0); // returns 0 - 1023 (due to 10 bit ADC)
//  Serial.println(potVal);      // print value to Serial
  int freq = map(potVal, 0, 1023, 0, MAX_FREQ);
  if(potVal > 0)
 {
  tone(PIEZO_OUTPUT_PIN, freq);
 } else {
  noTone(PIEZO_OUTPUT_PIN);
 }

 int segment = MAX_FREQ/4;

 int ledVal = freq;
 for(int i =0; i <= 3; i++) {
  if(segment > ledVal) {
    analogWrite(LED_PINS[i], map(ledVal, 0, segment, 0, 255));
    ledVal = 0;
    Serial.print(LED_PINS[i]);
    Serial.print(":");
    Serial.println(ledVal);
  } else if(ledVal > segment) {
    analogWrite(LED_PINS[i], 255);
    ledVal -= segment;
  }
 }

   delay(50);                   // Reading new values at ~20Hz

}
