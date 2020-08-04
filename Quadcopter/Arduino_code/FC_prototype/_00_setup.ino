void setup() {
  // put your setup code here, to run once:
  time = millis();
  Serial.begin(9600);
//  Serial.print("b");
  pinMode(led_LR, OUTPUT);
  pinMode(led_LG, OUTPUT);
  pinMode(led_RR, OUTPUT);
  pinMode(led_RG, OUTPUT);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(arm, INPUT);
  pinMode(disarm, INPUT);

  esc_Right.attach(11);  //pin 11 is right ESC
  esc_Left.attach(9);     //pin 9 is left ESC
  
  rollSetpoint = 0;
  rollError = 0;
  rollGyro = 0;
  rollAccel = 0;
  
  p_gain = 2;   //previously 15
  i_gain = 0.05;    //previously 0.03
  d_gain = 2;     //previously 1

    if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1)
      ;
    }
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
     */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1)
      ;
    }
  //Serial.print("p");

  escReady = false;

}
