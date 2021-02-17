#include <Adafruit_FXAS21002C.h>  //gyro library
#include <Adafruit_FXOS8700.h>  //accel & magne library
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

float rollAngle, rollGyro, rollAccel, accelVect;
float timePrev, deltaTime, time;


void setup() {
  Wire.begin();
  // put your setup code here, to run once:
  time = millis();
  Serial.begin(9600);

  rollGyro = 0;
  rollAccel = 0;
  
  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1);
    }
  if (!gyro.begin()) {
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  timePrev = time;
  time = millis();
  deltaTime = (time - timePrev) / 1000;

  Complementary();
  Data();
}


void Complementary(){
  sensors_event_t aevent, mevent;   //creating event for accel/mag sensor and getting the data
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;    //creating event for gyro sensor and getting the data
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.x) / (sqrt( sq((aevent.acceleration.y)) + sq((aevent.acceleration.z))) ));  //Accelerometer angle x
  rollGyro = event.gyro.y * 57.2958 * deltaTime; //Gyro angle y
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;  //Complementary filter
  
}


void Data(){
  Serial.print("Setpoint: ");
  Serial.print(0);
  Serial.print(", ");
  Serial.print("IMUangle: ");
  Serial.print(rollAngle);
  Serial.print(", ");
  Serial.print("\n");
}
