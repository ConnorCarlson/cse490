const int LED_OUTPUT_PIN = 3;
void setup() {
  // set Pin 3 to output
  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_OUTPUT_PIN, HIGH);
  delay(1000);                         
  digitalWrite(LED_OUTPUT_PIN, LOW);
  delay(1000);                         
}
