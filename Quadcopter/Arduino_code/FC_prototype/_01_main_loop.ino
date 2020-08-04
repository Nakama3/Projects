void loop() {
  getChannel();
  //armFunction();      //MUST use when mmotors are active with props
  //remoteArming();
  
  timePrev = time;
  time = millis();
  deltaTime = (time - timePrev) / 1000;

  
  RollAngleCalc();
  PID_Calc();
  if(armSwitch > 1000){
    PID_motorOutput();
  }
  //SerialData_motors();
  SerialPlot_gyro();
  Serial.print(armSwitch);
}
