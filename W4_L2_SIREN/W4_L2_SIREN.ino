const int OUTPUT_PIEZO_PIN = 6; // hook up one piezo leg to GND, other leg to Pin 9
const int OUTPUT_LED_PIN = LED_BUILTIN; // we'll flash an LED on/off with the sound
const int SOUND_DURATION_MS = 10; // duration to play each siren part

int curFreq;
boolean up;

void setup() {
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  curFreq = 100; 
  up = true;

}

void loop() {

  if(curFreq < 100 || curFreq > 500) {
    up = !up;
  }
  if(up) {
    curFreq = curFreq +1;
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else {
    curFreq = curFreq -1;
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
  tone(OUTPUT_PIEZO_PIN, curFreq);
  
  delay(SOUND_DURATION_MS);

}
