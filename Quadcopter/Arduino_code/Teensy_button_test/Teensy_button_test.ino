void setup() {
  // put your setup code here, to run once:
pinMode(12, OUTPUT);
pinMode(10, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(10) == LOW){
    digitalWrite(12, HIGH);
  }
  else if(digitalRead(10) == HIGH){
    digitalWrite(12, LOW);
  }

}
