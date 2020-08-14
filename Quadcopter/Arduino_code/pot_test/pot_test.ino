long outputPot;

void setup() {
  Serial.begin(9600);
}

void loop() {
  outputPot = map(analogRead(A2), 0, 1000, -10, 10);
  
  Serial.print("throttle: ");
  Serial.print(analogRead(A2));
  Serial.print("    angle: ");
  Serial.print(analogRead(A3));
  Serial.print("\n");
}


/*if(digitalRead(pinP) == HIGH){
    pValue = (outputPot * pValueMax) / 1000.000;
  }
  if(digitalRead(pinI) == HIGH){
    iValue = (outputPot * pValueMax) / 1000.000;;
  }
  if(digitalRead(pinD) == HIGH){
    dValue = (outputPot * pValueMax) / 1000.000;
  }

  
  Serial.print("P: ");
  Serial.print((outputPot * pValueMax)/1000.0000);
  Serial.print(",  I: ");
  Serial.print((outputPot * iValueMax)/1000.0000);
  Serial.print(",  D: ");
  Serial.print((outputPot * dValueMax)/1000.0000);
  Serial.print("\n");
  
*/
