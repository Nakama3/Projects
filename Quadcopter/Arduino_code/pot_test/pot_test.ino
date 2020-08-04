long outputPot;

int pinP = 4, pinI = 5, pinD = 6;
long pValueMax = 10.0000, iValueMax = 0.05, dValueMax = 3.00;
long pValue = 0.0000, iValue = 0.0000, dValue = 0.0000;

void setup() {
  Serial.begin(9600);
  pinMode(pinP, INPUT);
  pinMode(pinI, INPUT);
  pinMode(pinD, INPUT);
}

void loop() {
  outputPot = map(analogRead(A2), 0, 1000, -10, 10);
  
  Serial.print(outputPot);
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
