void armFunction(){
  //Arming delay
   if(digitalRead(arm) == HIGH){
      escReady = true;
    }
   if(digitalRead(disarm) == HIGH){
      escReady = false;
   }
   while(digitalRead(arm) == LOW && escReady == false){
    
    //Arming sequence//
    esc_Left.writeMicroseconds(1000);
    esc_Right.writeMicroseconds(1000);
    
    delay(1000);
//    Serial.print("Not ready, arming sequence incomplete: escReady = ");
//    Serial.print(escReady);
//    Serial.print("\n");
   
  }
  
//  Serial.print("Ready to go!");
}


void remoteArming(){
  while(armSwitch < 1000){
    
    //Arming sequence//
    esc_Left.writeMicroseconds(1000);
    esc_Right.writeMicroseconds(1000);
    Serial.print(armSwitch);
    delay(1000);
}
