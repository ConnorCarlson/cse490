const int PWM_CHANNEL_RED = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_CHANNEL_GREEN = 1; 
const int PWM_CHANNEL_BLUE = 2; 
const int PWM_FREQ = 500;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 
const int RED = 11;
const int BLUE = 9;
const int GREEN = 10;
// The max duty cycle value based on PWM resolution (will be 255 if resolution is 8 bits)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
//  ledcSetup(PWM_CHANNEL_RED, 1000, PWM_RESOLUTION);
//  ledcSetup(PWM_CHANNEL_GREEN, 2100, PWM_RESOLUTION);
//  ledcSetup(PWM_CHANNEL_BLUE, 4500, PWM_RESOLUTION);
//
//  ledcAttachPin(RED, PWM_CHANNEL_RED);
//  ledcAttachPin(GREEN, PWM_CHANNEL_GREEN);
//  ledcAttachPin(BLUE, PWM_CHANNEL_BLUE);
  

}

void loop() {
  if(Serial.available() > 0){
    // If we're here, then serial data has been received
    // Read data off the serial port until we get to the endline delimeter ('\n')
    // Store all of this data into a string
    byte red = Serial.read(); 
     
    byte green = Serial.read();
    byte blue = Serial.read();
    analogWrite(RED, red);
    analogWrite(GREEN, green);
    analogWrite(BLUE, blue);
//    ledcWrite(PWM_CHANNEL_RED, map(red, 0 ,255, 0, MAX_DUTY_CYCLE));   
//    ledcWrite(PWM_CHANNEL_GREEN, map(green, 0 ,255, 0, MAX_DUTY_CYCLE));
//    ledcWrite(PWM_CHANNEL_BLUE, map(blue, 0 ,255, 0, MAX_DUTY_CYCLE));

  }

}
