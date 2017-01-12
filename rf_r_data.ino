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
#define LCD_DISPLAY 1  //是否要啟用LCD顯示?
char payload;
int vals[8];
//unsigned long vals;

#if defined(LCD_DISPLAY)
  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2);  //set the LCD address to 0x27 for a 16 chars and 2 line display
#endif

struct dataStruct{
  int typeIR;
  unsigned long dataIR;
  unsigned int bitsIR;  
}myData;

void displayLCD(int yPosition, String txtMSG) {
   int xPos = (16-txtMSG.length())/2;
   if(xPos<0) xPos=0;
  lcd.setCursor(xPos,yPosition);
  lcd.print(txtMSG);
}

// 顯示紅外線協定種類
String getIRProtocol(int irNum, boolean sendIR) 
{  
  String irType;
  switch(irNum) {
   case 0:
    irType = "Unknown";
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;    
   case 1:
    irType = "NEC";
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 2:
    irType = "Sony";
    if(sendIR==1) irsend.sendSony((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 3:
    irType = "RC5";
    if(sendIR==1) irsend.sendRC5((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 4:
    irType = "RC6";
    if(sendIR==1) irsend.sendRC6((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 5:
    irType = "SANYO";
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 6:
    irType = "MITSUBISHI";
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 7:
    irType = "DISH";
    if(sendIR==1) irsend.sendDISH((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 8:
    irType = "SHARP";
    if(sendIR==1) irsend.sendSharpRaw((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 9:
    irType = "PANASONIC";
    if(sendIR==1) irsend.sendPanasonic((myData.dataIR, HEX), myData.bitsIR);
    break;
   case 10:
    irType = "JVC";
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
    break;
   default:
    irType = "Nodefine"; 
    if(sendIR==1) irsend.sendNEC((myData.dataIR, HEX), myData.bitsIR);
   break;
  }  
  
  return irType;

}

void setup() {
  Serial.begin(115200);
  radio.begin(); // Start up the radio
  
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openReadingPipe(1, address); // Write to device address name
  radio.startListening();

  if(LCD_DISPLAY==1) {
    lcd.begin();
    displayLCD(0, "IR to RF to IR");
    // 閃爍三次
    for(int i = 0; i < 3; i++) {
      lcd.backlight(); // 開啟背光
      delay(150);
      lcd.noBacklight(); // 關閉背光
      delay(150);
    }
    lcd.backlight();
  }  
}

void loop(void){

  
  
  if( radio.available()){
    radio.stopListening();
    //Serial.print("OK");
    while (radio.available()) {                          // While there is data ready
      radio.read( &myData, sizeof(myData) );             // Get the payload
    }
    
    String typeConvert = getIRProtocol(myData.typeIR, 0);
    Serial.print("Protocol: "); Serial.print(typeConvert); 
    Serial.print(", irCode: "); Serial.print(myData.dataIR, HEX); 
    Serial.print(", bits: "); Serial.println(myData.bitsIR);

    if(LCD_DISPLAY==1) {
      String HEX_IR;
  
      HEX_IR = String(myData.dataIR, HEX);
      lcd.clear();
      displayLCD(0, "[" + typeConvert + "] " + String(myData.bitsIR) + " bits");
      displayLCD(1, HEX_IR);
    }
      
    
    //delay(50);
    radio.startListening();

    
  }
  //irsend.sendNEC(0xff02fd, 32);
  delay(250);
}
