#include <SPI.h>
#include <Ethernet.h>
#include <Client.h>
#include "digitalWriteFast.h"
#include "nRF24L01.h"
#include "iBoardRF24.h"
#include <AESLib.h>

// REAL MAC - 0x88, 0x9F, 0xFA, 0x58, 0xC7, 0x6D
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };   

// Information about your controller
IPAddress ip(192,168,0,98);
IPAddress gateway(192,168,1,1);
IPAddress mask(255,255,255,0);
// IP of your server
IPAddress ipserv(192,168,0,80);

// 7887 - some clear port
EthernetServer server(7887);

// States of sensors/modules
boolean led1 = false; 
boolean change = false;

// Ports
const int pinLed1 = A8;
const int buttonLed1 = A2;

// Transaction data
String str = String(30);
String data = String(30);
String value = String(30);

char msg[16];
iBoardRF24 radio(12, 11, 8, 7, 9, PE7);
const uint64_t pipe = 0xE8E8F0F0E1LL;
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
String theMessage = "";

void setup() {
  pinMode(buttonLed1, INPUT);
  pinMode(pinLed1, OUTPUT);
  digitalWrite(pinLed1, LOW);
  Ethernet.begin(mac, ip, gateway, mask);
  delay(1);
  server.begin();
  delay(1);
  Serial.begin(9600);
  delay(1);
  
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  radio.setAutoAck(1);
  radio.setRetries(15,15);
  radio.enableAckPayload();
  radio.setPayloadSize(32);
  
  radio.openReadingPipe(1,pipe);
  radio.setChannel(0x65);
  
  radio.setPALevel (RF24_PA_LOW);
  radio.setDataRate (RF24_250KBPS);
}

// Check of pressing of the button which controll 'led1' 
void checkButtonLed1() {
  if(digitalRead(buttonLed1) != 1) {
    while(digitalRead(buttonLed1) != 1);
    data = "led1";
    if(led1) {
      value = "1";
      led1 = false;
      change = true;
    }
    if(!change)
      if(!led1) {
        value = "2";
        led1 = true;
        change = true;
      }
  } 
  digitalWrite(pinLed1, led1);
}

void checkRadioSignal() {
  if (radio.available()){
    for(int i = 0; i < 16; i++) {
      radio.read(&msg[i], 1);
      //Serial.print(msg[i]);
    }
    //Serial.println();
    aes128_dec_single(key, msg);
    /*Serial.println("DECRYPT:");
    for (int i = 0; i < sizeof(msg); i++) {
      Serial.print(msg[i]);
    }
    Serial.println();*/
    int i = 0;
    data = "";
    value = "";
    for(i; i<16; i++) {
      if((char)msg[i]!='=')
        data+=msg[i];
      else
        break;
      //Serial.print(msg[i]);
    }
    //Serial.println();
    for(i+=1; i<16; i++) {
      if((char)msg[i]!=';')
        value+=msg[i];
      else
        break;
    }
    if((value[0]<58) && (value[0]>47)){change = true;}
  }
}

// Send data about ALL sensors in 'Oona'
void sendData() {
  EthernetClient client = server.available();
  client.connect(ipserv,7887);
  if(client.connected()) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *\r\n");
    client.print("Content-Length: ");
    client.print(data.length()+value.length());
    client.print("\r\n");
    client.print("Connection: close\r\n");
    client.println("POST");
    client.println(data);
    client.println(value);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(ipserv);
    client.print("\r\n");
    client.println();
    client.stop();
    Serial.print("Nice connection!\r\n");
  } else {
    Serial.print("connection failed\r\n");
  }
  delay(100);
}

void loop() {
  EthernetClient client = server.available();
  if(client) {
    char c = client.read();
    if(str.length() < 30) { str += c; }
    if(c == '\n') {   
      Serial.println(str);        
      if(str.indexOf("led1=2") > 0) {
        Serial.println("ON");
        digitalWrite(pinLed1, HIGH);
        data = "led1";
        value = "2";
        led1 = true;
        change = true;
      }
      // Processing of the accepted data
      if(str.indexOf("led1=1") > 0) {
        Serial.println("OFF");
        digitalWrite(pinLed1, LOW);
        data = "led1";
        value = "1";
        led1 = false;
        change = true;
      }
      client.print("HTTP/1.1 200 OK\r\n");
      client.print("Access-Control-Allow-Origin: *\r\n");
      client.print("Host: ");
      client.print(ipserv);
      client.print("\r\n");
      client.print("Content-Type: text/html");
      client.print("Content-Length: ");
      client.print(data.length());
      client.print("\r\n");
      client.print("Connection: close\r\n");
      client.println();
      str="";
      delay(20);
      client.stop();
    }
  }
  checkButtonLed1();
  checkRadioSignal();
  if(change) { sendData(); change = false; }
}
