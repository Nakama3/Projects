#include <Adafruit_FXAS21002C.h>  //gyro library
#include <Adafruit_FXOS8700.h>  //accel & magne library
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);


float rollAngle, rollGyro, rollAccel;
float time, timePrev, deltaTime;

float p_gain, i_gain, d_gain;
float rollError, tempError, deltaError;
float rollSetpoint, P_calc, I_calc, D_calc, PID_roll;
int arm_LED = 12, disarm_LED = 11;
int arm = 10, disarm = 9;
int ch;
int throttle;
int pwmLeft, pwmRight;
bool escReady;

int outputPot;

Servo esc_Right, esc_Left;


//////////////////////////////////////////////// setup ////////////////////////////////////////////////////////////
void setup() {
  Wire.begin();
  // put your setup code here, to run once:
  time = millis();
  Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(arm_LED, OUTPUT);
  pinMode(disarm_LED, OUTPUT);
  pinMode(arm, INPUT_PULLUP);
  pinMode(disarm, INPUT_PULLUP);
  
  esc_Right.attach(8);  //pin 8 is right ESC
  esc_Left.attach(7);     //pin 7 is left ESC
  
  rollSetpoint = 0;
  rollError = 0;
  rollGyro = 0;
  rollAccel = 0;
  throttle = 1600;
  /////////////////////////// gain values //////////////////////
  
  p_gain = 2;   //previously 3 // test 1: 8  // test :  6
  i_gain = 0.075;    //previously 0.05 // test 1: 0.075 // test 2: .5
  d_gain = 0.05;     //previously 0.25 test 1: 0.4 // test 2: 0.125

  //////////////////////////////////////////////////////////////

  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1);
    }
  if (!gyro.begin()) {
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1);
    }
  escReady = false;
}

//////////////////////////////////////////////// main loop /////////////////////////////////////////////////////////////////

void loop() {  
  armFunction();      //MUST use when mmotors are active with props

 // rollSetpoint = analogRead(A2);
  //rollSetpoint = -map(analogRead(A2), 0, 1000, -10, 10);
//  throttle = analogRead(A3);
  //throttle = map(analogRead(A3), 0, 1000, 1500, 1900);
  
  // put your main code here, to run repeatedly:
  timePrev = time;
  

  
  RollAngleCalc();
  PID_Calc();
  PID_motorOutput();
  //SerialData_motors();
  SerialPlot_gyro();
  time = millis();
  deltaTime = (time - timePrev) / 1000;
}


//////////////////////////////////////////////// end main loop /////////////////////////////////////////////////////////////////



void armFunction(){
  //Arming delay
   if(digitalRead(arm) == LOW){
      digitalWrite(arm_LED, HIGH);
      digitalWrite(disarm_LED, LOW);
      escReady = true;
    }
   if(digitalRead(disarm) == LOW){
      digitalWrite(disarm_LED, HIGH);
      digitalWrite(arm_LED, LOW);
      escReady = false;
   }
   while(digitalRead(arm) == HIGH && escReady == false){
    
    //Arming sequence//
    esc_Left.writeMicroseconds(1000);
    esc_Right.writeMicroseconds(1000);
    
    delay(1000);
//    Serial.print("Not ready, arming sequence incomplete: escReady = ");
//    Serial.print(escReady);
//    Serial.print("\n");
   
  }
  
//  Serial.print("Ready to go!");
}


void RollAngleCalc(){
  sensors_event_t aevent, mevent;   //creating event for accel/mag sensor and getting the data
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;    //creating event for gyro sensor and getting the data
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.x) / (sqrt( sq((aevent.acceleration.y)) + sq((aevent.acceleration.z))) ));  //Complementary filter
  rollGyro = event.gyro.y * 57.2958 * deltaTime;
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;  //resulting real output angle
}

void new_PID(){
  
}





void PID_Calc(){
  tempError = rollError;
  rollError = rollSetpoint - rollAngle;
  deltaError = rollError - tempError;

  P_calc = p_gain * rollError;
 // I_calc = I_calc + (i_gain * rollError);
 // D_calc = d_gain * (deltaError/deltaTime);

  PID_roll = P_calc + I_calc + D_calc;
/*
  if(PID_roll > 400){
    PID_roll = 400;
  }
  if(PID_roll < -400){
    PID_roll = -400;
  }
  */
}

void PID_motorOutput(){
  
  pwmRight = throttle - map(PID_roll, -400, 400, -200, 200);
  pwmLeft = throttle + map(PID_roll, -400, 400, -200, 200);

  if(pwmRight < 1000){
    pwmRight = 1000;
  }
  if(pwmLeft < 1000){
    pwmLeft = 1000;
  }
  if(pwmRight > 1900){
    pwmRight = 1900;
  }
  if(pwmLeft > 1900){
    pwmLeft = 1900;
  }

  esc_Left.writeMicroseconds(pwmLeft);
  esc_Right.writeMicroseconds(pwmRight);
  
}



void SerialData_motors(){
  Serial.print("current angle: ");
  Serial.print(rollAngle);
  Serial.print(",  Left rotor value: ");
  Serial.print(pwmLeft);
  Serial.print(",  Right rotor value: ");
  Serial.print(pwmRight);

  Serial.print(",  escReady = ");
  Serial.print(escReady);
  Serial.print(" disarm: ");
  Serial.print(digitalRead(disarm_LED));
  Serial.print("\n");
}

void SerialPlot_gyro(){
  
  Serial.print("SetAngle: ");
  Serial.print(rollSetpoint * 10);
  Serial.print(",");
  Serial.print("Angle: ");
  Serial.print(rollAngle);
  Serial.print(",  ");
  Serial.print("MagAngle: ");
  Serial.print(rollAngle * 10);
  Serial.print(",  ");
  //
  Serial.print("pwmRight: ");
  Serial.print(pwmRight/10);
  Serial.print(",");
  Serial.print("pwmLeft: ");
  Serial.print(pwmLeft/10);
  Serial.print(",");
  //
  Serial.print("PIDoutput: ");
  Serial.print(PID_roll);
  Serial.print("\n");
}
