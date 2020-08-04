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

Servo esc_Right, esc_Left;
