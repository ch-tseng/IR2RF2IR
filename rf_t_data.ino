#define CE_PIN 8
#define CSN_PIN 10 //Since we are using 3 pin configuration we will use same pin for both CE and CSN

#include "RF24.h"
#include <IRremote.h>

byte address[11] = "SunplusIT-IR-Demo";
//byte char_array[] = "4FB48B7";

RF24 radio(CE_PIN, CSN_PIN);

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;         // 解碼結果將放在 decode_results 結構的 result 變數裏

struct dataStruct{
  int typeIR;
  unsigned long dataIR;
  unsigned int bitsIR;  
}myData;

// 顯示紅外線協定種類
void showIRProtocol(decode_results *results) 
{
  Serial.print("Protocol: ");
  
  // 判斷紅外線協定種類
  switch(results->decode_type) {
   case NEC:
    myData.typeIR = 1;
    Serial.print("NEC");
    break;
   case SONY:
    myData.typeIR = 2;
    Serial.print("SONY");
    break;
   case RC5:
    myData.typeIR = 3;
    Serial.print("RC5");
    break;
   case RC6:
    myData.typeIR = 4;
    Serial.print("RC6");
    break;
   case SANYO:
    myData.typeIR = 5;
    Serial.print("SANYO");
    break;
   case MITSUBISHI:
    myData.typeIR = 6;
    Serial.print("MITSUBISHI");
    break;
   case DISH:
    myData.typeIR = 7;
    Serial.print("DISH");
    break;
   case SHARP:
    myData.typeIR = 8;
    Serial.print("SHARP");
    break;
   case PANASONIC:
    myData.typeIR = 9;
    Serial.print("PANASONIC");
    break;
   case JVC:
    myData.typeIR = 10;
    Serial.print("JVC");
    break;
   case LG:
    myData.typeIR = 11;
    Serial.print("LG");
    break;
   case AIWA_RC_T501:
    myData.typeIR = 12;
    Serial.print("AIWA_RC_T501");
    break; 
   default:
    myData.typeIR = 0;
    Serial.print("Unknown encoding");  
  }  

  // 把紅外線編碼印到 Serial port
  Serial.print(", irCode: ");            
  //myData.dataIR = (results->value, HEX);
  Serial.print(results->value, HEX);    // 紅外線編碼
  Serial.print(",  bits: ");         
  myData.bitsIR = results->bits;  
  Serial.println(myData.bitsIR);        // 紅外線編碼位元數    
}

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openWritingPipe(address); // Write to device address name
}

void loop(void){

  
  if (irrecv.decode(&results)) {
    myData.dataIR = results.value;
    
    //Serial.println("");
    //Serial.print("IR received in HEX ---> ");
    //Serial.println(myData.dataIR, HEX);    
    //Serial.print("IR received in INT ---> ");
    //Serial.println(myData.dataIR);
    showIRProtocol(&results);
    //Serial.println("");
    
    irrecv.resume(); // Receive the next value

    boolean ok = radio.write( &myData, sizeof(myData) );
      
  }
  /*
  myData.dataIR = 3214333;
  myData.bitsIR =32;
  myData.typeIR = 12;
  boolean ok = radio.write( &myData, sizeof(myData) );
  Serial.println("Send...");
  */
  /*
  if (irrecv.decode(&results)) {
    String stringIR = String(results.value, HEX);
    char char_array[stringIR.length()+1];
    stringIR.toCharArray(char_array, stringIR.length()+1);
    
    Serial.println("");
    Serial.print("IR received HEX ---> ");
    Serial.println(results.value, HEX);
    Serial.println("");
    Serial.print("IR received HEX in STRING ---> ");
    Serial.println(char_array);
    irrecv.resume(); // Receive the next value

    bool okSend = radio.write( &char_array, strlen(char_array) ); //Send data to 'Receiver' ever second
  }
  */
  delay(2000);
}
