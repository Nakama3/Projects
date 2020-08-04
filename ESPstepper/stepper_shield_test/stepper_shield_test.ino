const int MS1pin = 3;
const int MS2pin = 4;
const int MS3pin = 5;
const int stepPinX = 6;
const int dirPinX = 7;
const int enablePin = 8;
const int upPin = 10;
const int downPin = 11;
const int trBase = 12;
const int trigger = 13;


void setup() {
  // put your setup code here, to run once:
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(10, INPUT);
pinMode(11, INPUT);
pinMode(12, OUTPUT);
pinMode(13, INPUT);
Serial.begin(9600);

digitalWrite(MS1pin,HIGH);
digitalWrite(MS2pin,HIGH);
digitalWrite(MS3pin,HIGH);
digitalWrite(enablePin, LOW);
digitalWrite(10, HIGH);
digitalWrite(11, HIGH);
digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigger,HIGH);
  if(digitalRead(upPin)==LOW){
    digitalWrite(dirPinX, HIGH);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(500);
  }
  if(digitalRead(downPin)==LOW){
    digitalWrite(dirPinX, LOW);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(500);
  }
  if(digitalRead(trigger)==LOW){
    digitalWrite(trBase, HIGH);
    Serial.write("ON");
  }
    else{
      digitalWrite(trBase,LOW);
      Serial.write("OFF");
    }
}
