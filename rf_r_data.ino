#define CE_PIN 8
#define CSN_PIN 10

#include <SPI.h>
#include "RF24.h"
#include <IRremote.h>

IRsend irsend;
RF24 radio(CE_PIN, CSN_PIN);
//bool radioNumber = 1;
//byte addresses[][6] = {"1Node","2Node"};


byte address[11] = "SunplusIT-IR-Demo";
char payload;
int vals[8];
//unsigned long vals;

struct dataStruct{
  int typeIR;
  unsigned long dataIR;
  unsigned int bitsIR;  
}myData;

// 顯示紅外線協定種類
String getIRProtocol(int irNum) 
{  
  String irType;
  switch(irNum) {
   case 0:
    irType = "Unknown";
    break;    
   case 1:
    irType = "NEC";
    //irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 2:
    irType = "Sony";
    //irsend.sendSony((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 3:
    irType = "RC5";
    //irsend.sendRC5((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 4:
    irType = "RC6";
    //irsend.sendRC6((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 5:
    irType = "SANYO";
    //irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 6:
    irType = "MITSUBISHI";
    //irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 7:
    irType = "DISH";
    //irsend.sendDISH((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 8:
    irType = "SHARP";
    //irsend.sendSharpRaw((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 9:
    irType = "PANASONIC";
    //irsend.sendPanasonic((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 10:
    irType = "JVC";
    //irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   default:
    irType = "Nodefine"; 
    //irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
   break;
  }  

  return irType;

}

void setup() {
  Serial.begin(115200);
  radio.begin(); // Start up the radio
  /*
  radio.setPALevel(RF24_PA_LOW);
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();
  */
  
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openReadingPipe(1, address); // Write to device address name
  radio.startListening();
  
}

void loop(void){

  
  
  if( radio.available()){
    radio.stopListening();
    //Serial.print("OK");
    while (radio.available()) {                          // While there is data ready
      radio.read( &myData, sizeof(myData) );             // Get the payload
    }
    //sendIRProtocol();
    Serial.print("Protocol: "); Serial.print(getIRProtocol(myData.typeIR)); 
    Serial.print(", irCode: "); Serial.print(myData.dataIR, HEX); 
    Serial.print(", bits: "); Serial.println(myData.bitsIR);
    
    //delay(50);
    radio.startListening();

    
  }

  delay(500);
}
