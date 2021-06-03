
#include <math.h> 
#include <Adafruit_LIS3DH.h>
#include <Adafruit_SSD1306.h>
#include <PushButton.h>

const int DELAY_MS = 5;
Adafruit_LIS3DH acc = Adafruit_LIS3DH();
Adafruit_SSD1306 display(128, 64, &Wire, 4);
int health;
const int barGap = 20;
const int THRUST_PIN = 18;
PushButton thrustButton = PushButton(THRUST_PIN);

void setup() {
  Serial.begin(115200);
  pinMode(THRUST_PIN, INPUT_PULLUP);
  thrustButton.setActiveLogic(LOW);
  acc.begin(0x18);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  health = 100;
}

void loop() {
  thrustButton.update();

  display.clearDisplay();
  if(Serial.available() > 0){
    String newDataType = Serial.readStringUntil(':');
    String newData = Serial.readStringUntil('\n');
    if(newDataType.equals("HEALTH")) {
      health = newData.toInt();
    }
  }

  display.drawRect(barGap, barGap, 128 - barGap*2, barGap, 1);
  int healthBarLength = map(health, 0, 100, 0, 128-barGap*2);
  display.fillRect(barGap, barGap, healthBarLength, barGap, 1);
  
  
  acc.read();
  

  sensors_event_t event;
  acc.getEvent(&event);

  float accX = event.acceleration.x;
  if(accX > 3 || accX < -3) {
    Serial.print("ACC:");
    Serial.print(accX);
    Serial.println();
    
  }


  if(thrustButton.isActive()){
    Serial.print("THRUST:");
    Serial.print("-");
    Serial.println();
  }
  
  display.display();
  

  delay(DELAY_MS);
}
