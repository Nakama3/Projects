#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Servo.h>

int flag = 0;
int i;
int buffer[25];
int channels[16];
byte b;
bool failsafe = 0;
int LED = 9;
int ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11, ch12, ch13, ch14, ch15, ch16, ch17, ch18;


SoftwareSerial RXSerial(8,7); //(RX, TX)


void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  
  RXSerial.begin(100000);
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  Timer1.initialize(975);
  Timer1.attachInterrupt(readFrSky);
}



void loop() {
  // put your main code here, to run repeatedly:
 // Serial.print(i); 
 /*
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
 */ 
  if(flag == 1){
    Timer1.stop();
    
    bytesToChannels();
    Serial.print(ch3);
    Serial.print("\n");
    
    analogWrite(LED, map(ch3, 172, 1811, 0, 255));
    
    i = 0;
    flag = 0;
 //  Serial.print(i);
   Timer1.restart();
  }
}



void readFrSky(){
  if(RXSerial.available()){     //wait for data to be availble on the serial stream
    b = RXSerial.read();
    if(i == 0 && b != 0x0F){      //wait for start byte 00001111 (0x0F)
      //error?
    }
    else{
      buffer[i++] = b;
 //     Serial.print(i);
 //     Serial.print("\n");
    }
  }
  if(i == 25){
   
    flag = 1;
  }
}

void bytesToChannels(){
//  Serial.print("parsing channels\n");
  ch1  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
  ch2  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
  ch3  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
  ch4  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
  ch5  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
  ch5  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
  ch6  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
  ch8  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
  ch9  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
  ch10  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
  ch11 = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
  ch12 = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
  ch13 = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
  ch14 = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
  ch15 = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
  ch16 = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);
  ch17 = ((buffer[23])      & 0x0001) ? 2047 : 0;
  ch18 = ((buffer[23] >> 1) & 0x0001) ? 2047 : 0;
  
  failsafe = ((buffer[23] >> 3) & 0x0001) ? 1 : 0;
  if ((buffer[23] >> 2) & 0x0001){ //lost
  }
//  Serial.print(ch1);
 // Serial.print("\n");
}
