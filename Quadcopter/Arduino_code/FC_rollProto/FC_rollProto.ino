#include <SBUS.h>
#include <Arduino.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
#include <limits.h>


int _channels[18];
int _failsafe;
long _goodFrames;
long _lostFrames;
long _decoderErrorFrames;
long long _lastGoodFrame;

    
SBUS sbus(Serial);

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);


float time, timePrev, deltaTime;
float rollAngle, rollGyro, rollAccel;
float p_gain, i_gain, d_gain;
float rollError, tempError, deltaError;
float rollSetpoint, P_calc, I_calc, D_calc, PID_roll;
int throttle = 1700;
int pwmLeft, pwmRight;
const int rLED = 10;
const int gLED = 9;
bool armed;
bool escReady;

Servo esc_Right, esc_Left;

void setup() {
  // put your setup code here, to run once:
  
  noInterrupts();
  TCCR2A  = 0;
  TCCR2B  = 0;
  TCNT2   = 0;
  OCR2A   = 249;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22);
  TIMSK2 |= (1 << OCIE2A);
  interrupts();
  
  time = millis();
  sbus.begin();
   
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);

  esc_Right.attach(6);  //pin 11 is right ESC
  esc_Left.attach(5);     //pin 9 is left ESC
  
  rollSetpoint = 0;
  rollError = 0;
  rollGyro = 0;
  rollAccel = 0;
  /////////////////////////// gain values //////////////////////
  
  p_gain = 2;   //previously 3 // test 1: 8
  i_gain = 0.05;    //previously 0.05 // test 1: 0.075
  d_gain = 0.05;     //previously 0.25 test 1: 0.4

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

ISR(TIMER2_COMPA_vect){
  sbus.process();
}

int getChannel(int channel, int min, int max){
  int value = sbus.getChannel(channel);
  int result = map(value, 172, 1811, min, max);
  return result;
}

//////////////////////////////////////////////// main loop /////////////////////////////////////////////////////////////////


void loop() {

  //// setting channel values /////
  
  rollSetpoint = getChannel(4, -15, 15);
  throttle = getChannel(3, 1000, 1900);
  
  ///// Arming check ///////
  if(getChannel(9, 172, 1811)>1000){
    while(armed == false){
      digitalWrite(gLED, HIGH);
      digitalWrite(rLED, LOW);
      delay(1000);
      armed = true;
    }
  
     /////////////// Flight code ////////////////////////
  
    
      timePrev = time;  //time dalay
      time = millis();
      deltaTime = (time - timePrev) / 1000;
             
      RollAngleCalc();  //PID calcs
      PID_Calc();
      
      PID_motorOutput();  //ESC signal output       
            
  }
  else{
    esc_Left.writeMicroseconds(1000);
    esc_Right.writeMicroseconds(1000);
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, LOW);
    armed = false;
  }
}


//////////////////////////////////////////////// end main loop /////////////////////////////////////////////////////////////////




void RollAngleCalc(){
  sensors_event_t aevent, mevent;
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.y) / (sqrt( sq((aevent.acceleration.x)) + sq((aevent.acceleration.z))) ));
  rollGyro = event.gyro.x * 57.2958 * deltaTime;
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;
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
  
  pwmRight = throttle - map(PID_roll, -400, 400, -100, 100);
  pwmLeft = throttle + map(PID_roll, -400, 400, -100, 100);

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
