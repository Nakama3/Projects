#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

float accel_angleX;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1)
      ;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t aevent, mevent;
  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);

  accel_angleX = atan((aevent.acceleration.x) / (sqrt( sq((aevent.acceleration.y)) + sq((aevent.acceleration.z))) ));
  accel_angleX *= 57.2958;
  
  //Serial.print("\ncurrent angle: ");
  Serial.print(accel_angleX);
 // Serial.print("  Z grav: ");
 // Serial.print(aevent.acceleration.z, 4);
 // Serial.print("  X grav: ");
//  Serial.print(aevent.acceleration.x, 4);
//  Serial.print("  Y grav: ");
//  Serial.print(aevent.acceleration.y, 4);
   Serial.print("\n");
    
  
}
