static int ch1;
static int *test;  
static int channels[18];
static byte          buffer[25];
static int           errors = 0;
static bool          failsafe = 0;
static int           idx;
static unsigned long last_refresh = 0;
static int           lost = 0;
byte b;


bool armed;
int rollSetpoint, throttle;

//HardwareSerial Serial2(2);

void setup() {
     pinMode(0, INPUT);
     pinMode(1, OUTPUT);
     Serial2.begin(100000,SERIAL_8E2); //The SBUS is a non standard baud rate which can be confirmed using an oscilloscope  
}

void loop() {
  if (Serial2.available ()) {
      b = Serial2.read ();

      if (idx == 0 && b != 0x0F) {  // start byte 15?
      } 
      else {
        buffer[idx++] = b;  // fill the buffer with the bytes until the end byte B0000000 is recived
      }
 
      if (idx == 25) {  // If we've got 25 bytes then this is a good package so start to decode
      idx = 0;
      if (buffer[24] != 0x00) {
        errors++;
      } 
      else {
        channels[1]  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
        channels[2]  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
        channels[3]  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
        channels[4]  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
        channels[5]  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
        channels[6]  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
        channels[7]  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
        channels[8]  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
        channels[9]  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
        channels[10]  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
        channels[11] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
        channels[12] = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
        channels[13] = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
        channels[14] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
        channels[15] = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
        channels[16] = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);
        channels[17] = ((buffer[23])      & 0x0001) ? 2047 : 0;
        channels[18] = ((buffer[23] >> 1) & 0x0001) ? 2047 : 0;
 
        failsafe = ((buffer[23] >> 3) & 0x0001) ? 1 : 0;
        if ((buffer[23] >> 2) & 0x0001) lost++;


        //////////////// main loop code after remote inputs are acquired ////////////////////////
        
        if(channels[9]>1000){
          while(armed == false){
            Serial2.print(channels[9]);
            Serial2.print("   armed and ready\n");
            delay(2000);
            armed = true;
          }

          /////////////// Flight code ////////////////////////

          rollSetpoint = map(channels[2], 172, 1811, -10, 10);
          throttle = map(channels[1], 172, 1811, 1000, 2000);
          Serial2.print("current angle: ");
          Serial2.print(rollSetpoint);
          Serial2.print("   throttle level: ");
          Serial2.print(throttle);
          Serial2.print("\n");
                  
        }
        else{
          Serial2.print(channels[9]);
          Serial2.print("   not armed\n");
          armed = false;
        }
        
      }
    }
  }
}
