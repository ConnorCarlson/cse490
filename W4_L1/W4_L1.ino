const int INPUT_BUTTON_PIN = 2;
const int OUTPUT_LED_PIN = 3;

void setup() {
  pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_LED_PIN, OUTPUT);

}

void loop() {
  int buttonVal = digitalRead(INPUT_BUTTON_PIN);
  digitalWrite(OUTPUT_LED_PIN, !buttonVal);

  

}
