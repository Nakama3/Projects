const int dirPin = 21;
const int stepPin = 22;

void setup() {
  // put your setup code here, to run once:
pinMode(dirPin, OUTPUT);
pinMode(stepPin, OUTPUT);
Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(stepPin, HIGH);
delayMicroseconds(500);
digitalWrite(stepPin, LOW);
delayMicroseconds(500);
Serial.println("ready");
}
