void PID_motorOutput(){
  
  pwmRight = throttle - map(PID_roll, -400, 400, -200, 200);
  pwmLeft = throttle + map(PID_roll, -400, 400, -200, 200);

  if(pwmRight < 1500){
    pwmRight = 1500;
  }
  if(pwmLeft < 1500){
    pwmLeft = 1500;
  }
  if(pwmRight > 1900){
    pwmRight = 1900;
  }
  if(pwmLeft > 1900){
    pwmLeft = 1900;
  }

  esc_Left.writeMicroseconds(pwmLeft);
  esc_Right.writeMicroseconds(pwmRight);
}




void PID_LEDoutput(){
  PID_roll = map(PID_roll, -400, 400, -255, 255);

  if(PID_roll > 1){
    analogWrite(led_RG, abs(PID_roll));
  }
  else{
    digitalWrite(led_RG,LOW);
  }
  if(PID_roll < -1){
    analogWrite(led_RR, abs(PID_roll));
  }
  else{
    digitalWrite(led_RR,LOW);
  }
  if(-PID_roll > 1){
    analogWrite(led_LG, abs(PID_roll));
  }
  else{
    digitalWrite(led_LG,LOW);
  }
  if(-PID_roll < -1){
    analogWrite(led_LR, abs(PID_roll) );
  }
  else{
    digitalWrite(led_LR,LOW);
  }
}
