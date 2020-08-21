#include <SBUS.h>

int LED = 3;
int LED2 = 2;
int stv1 = 0;
bool armed = false;

SBUS sbus(Serial);

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
  
  sbus.begin();

  
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
smArm();
Serial.print(sbus.getChannel(9));
Serial.print(armed);
Serial.print("\n");

}

ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

int getChannel(int channel, int min, int max){
  int value = sbus.getChannel(channel);
  int result = map(value, 172, 1811, min, max);
  return result;
}

void smArm(){
  switch(stv1){
    case 0:
      stv1 = 1;
      break;
    case 1:
      if(sbus.getChannel(9) > 1000){
        digitalWrite(LED, HIGH);
        if(armed == false){
          delay(1000);
        }
        digitalWrite(LED2, LOW);
        armed = true;
      }
      else if(sbus.getChannel(9) < 1000){
        digitalWrite(LED, LOW);
        digitalWrite(LED2, HIGH);
        armed = false;
      }
      stv1 = 0;
      break;
  }
}
