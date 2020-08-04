#include <FrskySP.h>
#include <SoftwareSerial.h>

FrskySP FrskySP (10);

void setup() {
  // put your setup code here, to run once:
  FrskySP.ledSet(13);
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned int i[29] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  while (FrskySP.available()){
    if (FrskySP.read() == 0x7E){
      while (!FrskySP.available());
        if(FrskySP.read() == 0xA1){
           // 1st FLVSS
            if (i[2] % 6 == 0)  FrskySP.sendData (FRSKY_SP_CELLS, FrskySP.lipoCell (0, 3.50, 3.51));
            if (i[2] % 6 == 1)  FrskySP.sendData (FRSKY_SP_CELLS, FrskySP.lipoCell (2, 3.52, 3.53));
            if (i[2] % 6 == 2)  FrskySP.sendData (FRSKY_SP_CELLS, FrskySP.lipoCell (4, 3.54, 3.55));
  
            i[2]++;
        }
    }
  }
}
