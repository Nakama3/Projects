#include <Adafruit_FXAS21002C.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Gyroscope Test");
  Serial.println("");

  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
     */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1){};
  }

  sensor_t sensor;
  gyro.getSensor(&sensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event;
  gyro.getEvent(&event);

  Serial.print("X: ");
  Serial.print(event.gyro.x);

}
