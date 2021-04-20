const int LED_OUTPUT_PIN = 3;
const int DELAY_MS = 5;
const int MAX_ANALOG_OUT = 255;

void setup() {
  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop(){
  for(int i = 0; i <= MAX_ANALOG_OUT; i += 1){
    analogWrite(LED_OUTPUT_PIN, i);
    delay(DELAY_MS);
  }

  for(int i = MAX_ANALOG_OUT; i >= 0; i -= 1){
    analogWrite(LED_OUTPUT_PIN, i);
    delay(DELAY_MS);
  }
}
