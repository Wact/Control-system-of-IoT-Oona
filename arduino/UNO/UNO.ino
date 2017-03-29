#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <AESLib.h>
//#include "Ultrasonic.h"
//Ultrasonic ultrasonic(12,13);

char msg[16];
int last = 200;
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(A0,INPUT);
  
  radio.setAutoAck(1);
  radio.setRetries(15, 15);
  radio.enableAckPayload();
  radio.setPayloadSize(32);

  radio.openWritingPipe(pipe);
  radio.setChannel(0x65);

  radio.setPALevel (RF24_PA_LOW);
  radio.setDataRate (RF24_250KBPS);
}

void loop(void){
  int value = analogRead(A0)/10;
  if(last != value) {
    String strValue = String(value);
    String strKey = String("sensorA1");
    String theMessage = strKey+"="+strValue;
    int messageSize = theMessage.length();
    
    for (int i = 0; i < 16; i++) {
      if(theMessage[i])
        msg[i] = theMessage[i];
       else
         msg[i] = ';';
      Serial.print(msg[i]);
    }
    aes128_enc_single(key, msg);
    Serial.println("ENCRYPT:");
    for (int i = 0; i < sizeof(msg); i++) {
      radio.write(&msg[i], 1);
      Serial.print(msg[i]);
    }
    Serial.println();
    last = value;
  }
  //Serial.print("To an object of: ");
  //Serial.println(ultrasonic.Ranging(CM));
  delay(1000);
}
