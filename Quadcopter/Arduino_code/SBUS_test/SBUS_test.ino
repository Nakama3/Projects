#include <SoftwareSerial.h>
// comment change
// another change
int weird;
//#define rxPin 7
//#define txPin 8
int channels[18];
SoftwareSerial RXSerial(8,7); //(RX, TX)


void setup() {
  // put your setup code here, to run once:
  //The pins on the Arduino need to be defined. In this demo pin 9-13 are used. Pin 13 is the LED on the Arduino which is 
  //a simple mehod for debugging. The demo turns this on when the Channel 1 (often used for throttle) exceeds a predefined SBUS level. 
  //   pinMode(13, OUTPUT); //Debug with LED
 //    pinMode(12, OUTPUT); //Debug with LED
//     pinMode(11, OUTPUT); //Debug with LED
   //  pinMode(10, OUTPUT); //Debug with LED
//     pinMode(9, OUTPUT); //Debug with LED
     pinMode(0, INPUT);
     pinMode(1, OUTPUT);
//     myservo8.attach(8);  // attaches the servo on pin 8 to the servo object
     //myservo11.attach(11);  // attaches the servo on pin 11 to the servo object
     RXSerial.begin(100000);
     Serial.begin(9600);
     int test;  
}

void loop() {
  // put your main code here, to run repeatedly:

  /*
  if(getChannel(1) > 1500){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }
  
  Serial.print(getChannel(1));
  Serial.print("\n");
  */

 getChannel();

/*
  
  Serial.print(ch1);
  Serial.print(ch7);
  Serial.print("\n");
  */
  //getChannel(1);  Test printing channel 1 method

  
}





void getChannel(){
    //Declare the variabes
  static byte          buffer[25];
//  static int           channels[18];
  static int           errors = 0;
  static bool          failsafe = 0;
  static int           idx;
  static unsigned long last_refresh = 0;
  static int           lost = 0;
  byte b;
  int  i;
  int bluePin = 9;
  int yellowPin = 11;
  //int Pin = 6;
  word results;
  


 //Check the serial port for incoming data
 //This could also be done via the serialEvent()
  if (RXSerial.available ()) {
      b = RXSerial.read ();
       
     //this is a new package and it' not zero byte then it's probably the start byte B11110000 (sent MSB)
     //so start reading the 25 byte package
      
      if (idx == 0 && b != 0x0F) {  // start byte 15?
       // error - wait for the start byte
        
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
          //  Serial.println("Found Packet");
          // 25 byte packet received is little endian. Details of how the package is explained on our website:
          //http://www.robotmaker.eu/ROBOTmaker/quadcopter-3d-proximity-sensing/sbus-graphical-representation
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
          if ((buffer[23] >> 2) & 0x0001){ lost++;}
          //serialPrint (lost); debg the signals lost

          channels[1] = map(channels[1], 1000, 2000, 0, 255);
           Serial.print(channels[1]);
           Serial.print("\n");
        if(channels[1] >= 0){
          analogWrite(bluePin, channels[1]);
            }
   //       Testing channel 1 print to serial monitor:
          
 //         Serial.print(channels[ch]);
  //        Serial.print("\n");

       


       
 //        return channels[ch];
          
          
      }
    }
  }
}
