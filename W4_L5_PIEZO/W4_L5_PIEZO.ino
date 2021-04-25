const int PIEZO_OUTPUT_PIN = 3;

void setup()
{
  Serial.begin(9600); // for printing values to console
}

void loop()
{
  int potVal = analogRead(A0); // returns 0 - 1023 (due to 10 bit ADC)
  Serial.println(potVal);      // print value to Serial
  if(potVal > 0)
 {
  tone(PIEZO_OUTPUT_PIN, map(potVal, 0, 1023, 0, 1000));
 } else {
  noTone(PIEZO_OUTPUT_PIN);
 }
  delay(50);                   // Reading new values at ~20Hz
}
