//change
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);


float time, timePrev, deltaTime;

float rollAngle, rollGyro, rollAccel;
float p_gain, i_gain, d_gain;
float rollError, tempError, deltaError;
float rollSetpoint, P_calc, I_calc, D_calc, PID_roll;
int led_LR = 6, led_LG = 10, led_RR = 5, led_RG = 3, led_up = 9, led_down = 11;
int ch;
int throttle = 1700;
int pwmLeft, pwmRight;
int arm = 2;
int disarm = 4;
bool escReady;
int armSwitch;


static int channels[18];
static byte          buffer[25];
static int           errors = 0;
static bool          failsafe = 0;
static int           idx;
static unsigned long last_refresh = 0;
static int           lost = 0;
byte b;


bool armed;

int led;
Servo esc_Right, esc_Left;

void setup() {
  // put your setup code here, to run once:
  time = millis();
  Serial.begin(100000);
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


   if (Serial.available ()) {
      b = Serial.read ();

      if (idx == 0 && b != 0x0F) {  // start byte 15?
      } 
      else {
        buffer[idx++] = b;  // fill the buffer with the bytes until the end byte B0000000 is recived
      }
 
      if (idx == 25) {  // If we've got 25 bytes then this is a good package so start to decode
      idx = 0;
      if (buffer[24] != 0x00) {
        errors++;
      } 
      else {
        channels[1]  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
        channels[2]  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
        channels[3]  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
        channels[4]  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
        channels[5]  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
        channels[6]  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
        channels[7]  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
        channels[8]  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
        channels[9]  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
        channels[10]  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
        channels[11] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
        channels[12] = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
        channels[13] = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
        channels[14] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
        channels[15] = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
        channels[16] = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);
        channels[17] = ((buffer[23])      & 0x0001) ? 2047 : 0;
        channels[18] = ((buffer[23] >> 1) & 0x0001) ? 2047 : 0;
 
        failsafe = ((buffer[23] >> 3) & 0x0001) ? 1 : 0;
        if ((buffer[23] >> 2) & 0x0001) lost++;

        


        //////////////// main loop code after remote inputs are acquired ////////////////////////

        //// setting channel values /////
        
        rollSetpoint = map(channels[2], 172, 1811, -10, 10);
      //  throttle = map(channels[1], 172, 1881, 1000, 2000);
        
        ///// Arming check ///////
        if(channels[9]>1000){
          while(armed == false){
            Serial.print(channels[9]);
            Serial.print("   armed and ready\n");
            delay(1000);
            armed = true;
          }

          /////////////// Flight code ////////////////////////

          
            timePrev = time;
            time = millis();
            deltaTime = (time - timePrev) / 1000;
                   
            
            RollAngleCalc();
            PID_Calc();
            
            PID_motorOutput();  
            SerialPlot_gyro();
            Serial.print(rollSetpoint);
            Serial.print("\n");
            
           /////////////////// End Flight code ////////////////////
         
                  
        }
        else{
          Serial.print(channels[9]);
          Serial.print("   not armed\n");
          esc_Left.writeMicroseconds(1000);
          esc_Right.writeMicroseconds(1000);
          armed = false;
        }
        
      }
    }
  } 
}


//////////////////////////////////////////////// end main loop /////////////////////////////////////////////////////////////////



bool Testarm(int armSwitch){
  if(armSwitch < 1000){
    esc_Left.writeMicroseconds(1000);
    esc_Right.writeMicroseconds(1000);
    Serial.print("not ready");
    Serial.print(armSwitch);
    Serial.print("\n");
    return escReady = false;
  }
  else{
    Serial.print("ready!");
    Serial.print(armSwitch);
    Serial.print("\n");
    return escReady = true;
  }
}

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
  Serial.print(rollSetpoint);
  Serial.print("\n");
  Serial.print("Angle: ");
  Serial.print(rollAngle * 10);
  Serial.print(",  ");
  /*
  Serial.print("pwmRight: ");
  Serial.print(pwmRight);
  Serial.print(",");
  Serial.print("pwmLeft: ");
  Serial.print(pwmLeft);
  Serial.print("\n");
  */
  Serial.print("PIDoutput: ");
  Serial.print(PID_roll);
  Serial.print("\n");
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


void RollAngleCalc(){
    sensors_event_t aevent, mevent;
  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.y) / (sqrt( sq((aevent.acceleration.x)) + sq((aevent.acceleration.z))) ));
  rollGyro = event.gyro.x * 57.2958 * deltaTime;
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;
}


void PID_motorOutput(){
  
  pwmRight = throttle - map(PID_roll, -400, 400, -200, 200);
  pwmLeft = throttle + map(PID_roll, -400, 400, -200, 200);

  if(pwmRight < 1500){
    pwmRight = 1500;
  }
  if(pwmLeft < 1500){
    pwmLeft = 1500;
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
