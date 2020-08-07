#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>


//////////////////////// sensor class setup ///////////////
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

//////////////////////// Variables ////////////////////
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

/////////////////////// Task Declaration //////////////////////
//void TaskIMU_angle( void *pvParameters );
//void TaskPID_roll( void *pvParameters );
//void TaskMotorOutput( void *pvParameters );
//void TaskArmFunction( void *pvParameters );


/////////////////////////////////////////////////////////// setup /////////////////////////////////////////////////////////
void setup() {

  time = millis();
  Serial.begin(9600);

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

  p_gain = 6;
  i_gain = 0.05;
  d_gain = 0.125;

  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1);
    }
  if (!gyro.begin()) {
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1);
    }
    
  escReady = false;
  Serial.print("test");
/////////////////// Task Creation //////////////////
  xTaskCreate(TaskIMU_angle, "IMU_angle", 200, NULL, 2, NULL);
  xTaskCreate(TaskPID_roll, "PID_roll", 200, NULL, 1, NULL);
  xTaskCreate(TaskMotorOutput, "MotorOutput", 200, NULL, 0, NULL);
  xTaskCreate(TaskArmFunction, "ArmFunction", 200, NULL, 1, NULL);

  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:
  // we leave the main loop empty as the tasks are in control of the code
}
///////////////////////////////////////////////////////////////// Starting Tasks //////////////////////////////////////////////////////////

static void TaskArmFunction( void *pvParameters ){
  (void) pvParameters;
  Serial.print("Arming sequence started");
  for(;;){
     if(digitalRead(arm) == HIGH){
        escReady = true;
        Serial.print("1");
        vTaskDelay(500 / portTICK_PERIOD_MS);
     }
     if(digitalRead(disarm) == HIGH){
        escReady = false;
        vTaskDelay(500 / portTICK_PERIOD_MS);
     }/*
     while(digitalRead(arm) == LOW && escReady == false){
      
      //Arming sequence//
      esc_Left.writeMicroseconds(1000);
      esc_Right.writeMicroseconds(1000);
      
      delay(1000);
    }*/
  }
}

static void TaskIMU_angle( void *pvParameters ){
  (void) pvParameters;
  Serial.print("test 2");
  for(;;){
    timePrev = time;
    time = millis();
    deltaTime = (time - timePrev) / 1000;
  
    sensors_event_t aevent, mevent;   //creating event for accel/mag sensor and getting the data
    accelmag.getEvent(&aevent, &mevent);
    
    sensors_event_t event;    //creating event for gyro sensor and getting the data
    gyro.getEvent(&event);
  
    rollAccel = 57.2958* atan((aevent.acceleration.y) / (sqrt( sq((aevent.acceleration.x)) + sq((aevent.acceleration.z))) ));  //Complementary filter
    rollGyro = event.gyro.x * 57.2958 * deltaTime;
    
    rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;  //resulting real output angle
    Serial.print("2");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  
}

static void TaskPID_roll( void *pvParameters ){
  (void) pvParameters;
  Serial.print("test 3");
  for(;;){
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
    Serial.print("3");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

static void TaskMotorOutput( void *pvParameters ){
  (void) pvParameters;
  Serial.print("test 4");
  for(;;){
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
    Serial.print("4");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
