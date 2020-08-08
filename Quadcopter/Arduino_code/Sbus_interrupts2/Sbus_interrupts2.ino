#include <SBUS.h>
#include <limits.h>
#include <Servo.h>

// Remember to unplug the RX pin of your UNO when uploading this sketch.
SBUS sbus(Serial);

const int rLED = 10;
const int gLED = 5;
bool armed;
Servo motorL, motorR;

void setup()
{
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  motorR.attach(11);
  motorL.attach(9);

  sbus.begin();
}

// This is timer2, which triggers ever 1ms and processes the incoming SBUS datastream.
ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

static int minChannel = INT_MAX;
static int maxChannel = INT_MIN;

// Scale the S.BUS channel values into the range [0, 255] for use as LED brightness values.
int getChannel(int channel) {
  int value = sbus.getChannel(channel);
 int result = map(value, 172, 1811, -15, 15);
  return result; 
}

void loop()
{

  analogWrite(gLED, getChannel(2));

  
  /*
  if(getChannel(9)>1000){
    while(armed == false){
      digitalWrite(gLED, HIGH);
      digitalWrite(rLED, LOW);
      delay(1000);
      armed = true;
    }
    
      /////////////// Flight code ////////////////////////
    motorL.writeMicroseconds(getChannel(4));
    motorR.writeMicroseconds(getChannel(3));



      
    /*
    timePrev = time;  //time dalay
    time = millis();
    deltaTime = (time - timePrev) / 1000;
           
    RollAngleCalc();  //PID calcs
    PID_Calc();
    
    PID_motorOutput();  //ESC signal output
    
  
     /////////////////// End Flight code ////////////////////
  
        
  }
  else{
    motorL.writeMicroseconds(1000);
    motorR.writeMicroseconds(1000);
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, LOW);
    armed = false;
  }
  */
}
