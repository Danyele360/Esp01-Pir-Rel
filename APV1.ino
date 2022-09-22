#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
 
//------------- Set WiFi credentials--------------------
const char* ap_id   = "EspComunication";
const char* ap_pswd = "Esp123456";
//------------------- Variabili ------------------------
int statoallarme =0;
#define UDP_PORT 4210
#define RELAY 0
#define PIR_PIN 2
WiFiUDP UDP;
char packet[255];

void setup() {
pinMode(PIR_PIN,INPUT);
pinMode(RELAY,OUTPUT);
digitalWrite(RELAY, HIGH);
Serial.begin(115200);
Serial.println();
WiFi.mode(WIFI_AP);
WiFi.softAP(ap_id, ap_pswd);
IPAddress ap_ip = WiFi.softAPIP();
Serial.println();
Serial.print(" APV1 Online! Your IP address: ");
Serial.println(ap_ip);
delay(500);
// Begin listening to UDP port
UDP.begin(UDP_PORT);
Serial.print("Listening on UDP port ");
Serial.println(UDP_PORT);
}

void loop() {
    //--------------------- If packet received----------------------
    int packetSize = UDP.parsePacket();
    if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    if (len > 0)
    { packet[len] = '\0';}
    Serial.print("Packet received: ");
    Serial.println(packet);
    String msg = (packet);
    msg.trim();
    //--------------------Attivo Allarme----------------------------
    if(msg=="stato" and statoallarme==0){
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort()); //Usa remoteip del mittente
    UDP.write("Allarme Attivato!");
    UDP.endPacket();
    statoallarme = 1;
    digitalWrite(RELAY,HIGH); //debug
    }
    //---------------------Disattivo Allarme-----------------------
    else if(msg=="stato" and statoallarme==1){
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort()); //Usa remoteip del mittente
    UDP.write("Allarme Disattivato!");
    UDP.endPacket();
    statoallarme = 0;
    digitalWrite(RELAY,HIGH); //debug
      }
  }
//-----------------------Se Allarme ON--------------------------
if(digitalRead(PIR_PIN) == HIGH and statoallarme==1){
digitalWrite(RELAY,LOW); 
}
//--------------------------------------------------------------
    
delay(500);
}
