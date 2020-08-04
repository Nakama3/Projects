void SerialData_motors(){

  Serial.print("target angle: ");
  Serial.print(rollSetpoint);
  Serial.print(",");
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
