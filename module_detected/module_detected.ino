#include <IRLibRecvFrec.h> 
#include <IRLibRecvPCI.h>
#include <IRLibCombo.h>
#include <IRLibRecv.h>
#include <IRLibRecvPCI.h>

IRrecvPCI myReceiver(3);

void setup() {
  Serial.begin(9600);
  delay(1000); 
  while (!Serial); 
  myReceiver.enableIRIn(); 
  myReceiver.setFrameTimeout(100000);
}

void loop() {
  if (myReceiver.getResults()) { 
    Serial.print(F("\n RAW_DATA_LEN "));
    Serial.println(recvGlobal.recvLength,DEC);
    Serial.print(F("RAW_DATA_LEN={\n\t"));
    for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
      Serial.print(recvGlobal.recvBuffer[i],DEC);
      Serial.print(F(", "));
      if( (i % 8)==0) Serial.print(F("\n\t"));
    }
    Serial.println(F("1000};"));
    myReceiver.enableIRIn();
  }
}
