
#include <math.h> 
#include <Adafruit_LIS3DH.h>
const int DELAY_MS = 5;
Adafruit_LIS3DH acc = Adafruit_LIS3DH();

void setup() {
  Serial.begin(115200);
  acc.begin(0x18);
}

void loop() {
  acc.read();
  

  sensors_event_t event;
  acc.getEvent(&event);

  float accX = event.acceleration.x;
  float accY = event.acceleration.y;
  float accZ = event.acceleration.z;

  float pitch = 180 * atan (accX/sqrt(accY*accY + accZ*accZ))/M_PI;
  float roll = 180 * atan (accY/sqrt(accX*accX + accZ*accZ))/M_PI;
  float yaw = 180 * atan (accZ/sqrt(accX*accX + accZ*accZ))/M_PI;
  Serial.print(pitch);
  Serial.print(","); Serial.print(roll);
  Serial.print(","); Serial.print(yaw);
  Serial.println();

  delay(DELAY_MS);
}
