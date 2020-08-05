//change
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
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
int led_LR = 6, led_LG = 10, led_RR = 5, led_RG = 3, led_up = 9, led_down = 11;
int ch;
int throttle;
int pwmLeft, pwmRight;
int arm = 2;
int disarm = 4;
bool escReady;

int outputPot;

Servo esc_Right, esc_Left;


//////////////////////////////////////////////// setup ////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  time = millis();
  Serial.begin(9600);
  pinMode(led_LR, OUTPUT);
  pinMode(led_LG, OUTPUT);
  pinMode(led_RR, OUTPUT);
  pinMode(led_RG, OUTPUT);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(arm, INPUT);
  pinMode(disarm, INPUT);

  esc_Right.attach(11);  //pin 11 is right ESC
  esc_Left.attach(9);     //pin 9 is left ESC
  
  rollSetpoint = 0;
  rollError = 0;
  rollGyro = 0;
  rollAccel = 0;
  throttle = 1600;
  /////////////////////////// gain values //////////////////////
  
  p_gain = 6;   //previously 3 // test 1: 8
  i_gain = 0.05;    //previously 0.05 // test 1: 0.075
  d_gain = 0.125;     //previously 0.25 test 1: 0.4

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

 // rollSetpoint = -map(analogRead(A2), 0, 1000, -10, 10);
 throttle = map(analogRead(A2), 0, 1000, 1500, 1900);
  
  // put your main code here, to run repeatedly:
  timePrev = time;
  time = millis();
  deltaTime = (time - timePrev) / 1000;

  
  RollAngleCalc();
  PID_Calc();
  PID_motorOutput();
  //SerialData_motors();
  SerialPlot_gyro();
}


//////////////////////////////////////////////// end main loop /////////////////////////////////////////////////////////////////



void armFunction(){
  //Arming delay
   if(digitalRead(arm) == HIGH){
      escReady = true;
    }
   if(digitalRead(disarm) == HIGH){
      escReady = false;
   }
   while(digitalRead(arm) == LOW && escReady == false){
    
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
  Serial.print(digitalRead(disarm));
  Serial.print("\n");
}

void SerialPlot_gyro(){
  Serial.print("SetAngle: ");
  Serial.print(rollSetpoint * 10);
  Serial.print(",");
  Serial.print("Angle: ");
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




void RollAngleCalc(){
  sensors_event_t aevent, mevent;   //creating event for accel/mag sensor and getting the data
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;    //creating event for gyro sensor and getting the data
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.y) / (sqrt( sq((aevent.acceleration.x)) + sq((aevent.acceleration.z))) ));  //Complementary filter
  rollGyro = event.gyro.x * 57.2958 * deltaTime;
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;  //resulting real output angle
}

void PID_Calc(){
  tempError = rollError;
  rollError = rollSetpoint - rollAngle;
  deltaError = rollError - tempError;

  P_calc = p_gain * rollError;
  I_calc = I_calc + (i_gain * rollError);
  D_calc = d_gain * (deltaError/deltaTime);

  PID_roll = P_calc + I_calc + D_calc;

  if(PID_roll > 400){
    PID_roll = 400;
  }
  if(PID_roll < -400){
    PID_roll = -400;
  }
}

void PID_motorOutput(){
  
  pwmRight = throttle - map(PID_roll, -400, 400, -300, 300);
  pwmLeft = throttle + map(PID_roll, -400, 400, -300, 300);

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




void PID_LEDoutput(){
  PID_roll = map(PID_roll, -400, 400, -255, 255);

  if(PID_roll > 1){
    analogWrite(led_RG, abs(PID_roll));
  }
  else{
    digitalWrite(led_RG,LOW);
  }
  if(PID_roll < -1){
    analogWrite(led_RR, abs(PID_roll));
  }
  else{
    digitalWrite(led_RR,LOW);
  }
  if(-PID_roll > 1){
    analogWrite(led_LG, abs(PID_roll));
  }
  else{
    digitalWrite(led_LG,LOW);
  }
  if(-PID_roll < -1){
    analogWrite(led_LR, abs(PID_roll) );
  }
  else{
    digitalWrite(led_LR,LOW);
  }
}
