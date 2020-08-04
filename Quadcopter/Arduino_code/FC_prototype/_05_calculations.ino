void PID_Calc(){
  tempError = rollError;
  rollError = rollSetpoint - rollAngle;
  deltaError = rollError - tempError;

  P_calc = p_gain * rollError;
  I_calc = I_calc + (i_gain * rollError);
  D_calc = d_gain * (deltaError/deltaTime);

  PID_roll = P_calc + I_calc + D_calc;

  if(PID_roll > 400){
    PID_roll = 400;
  }
  if(PID_roll < -400){
    PID_roll = -400;
  }
}


void RollAngleCalc(){
    sensors_event_t aevent, mevent;
  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  
  sensors_event_t event;
  gyro.getEvent(&event);

  rollAccel = 57.2958* atan((aevent.acceleration.y) / (sqrt( sq((aevent.acceleration.x)) + sq((aevent.acceleration.z))) ));
  rollGyro = event.gyro.x * 57.2958 * deltaTime;
  
  rollAngle = 0.98 * rollAngle + rollGyro + 0.02 *rollAccel ;
}
