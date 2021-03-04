#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>

const char* ssid = "Familia_Uriza";
const char* password = "";

unsigned int udp_Port = 2390;      // local port to listen on
IPAddress ip_remote(192, 168, 0, 50);

char packetBuffer[255]; //buffer to hold incoming packet
char  replyBuffer[] = "acknowledged";       // a string to send back
String msg_wifi ;

WiFiUDP Udp;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    Serial.println();
    
    Udp.remoteIP() = ip_remote;


    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());

    Udp.begin(udp_Port);

    Udp.beginPacket(ip_remote, udp_Port);//send ip to server
    
    char ipBuffer[255];
    
    WiFi.localIP().toString().toCharArray(ipBuffer, 255);
    Udp.write(ipBuffer);
    Udp.endPacket();
    Serial.println("Sent ip adress to server");
    }

void loop() 
  {
      
      if (Serial.available() > 0) 
        {
          
          msg_wifi += "WIFI=S0;";
          msg_wifi += Serial.readStringUntil('\n'); // read the incoming byte:
          Serial.println(msg_wifi);
  
          int str_len = msg_wifi.length() + 1;
          char buffer_msg [str_len];
          
          msg_wifi.toCharArray(buffer_msg, str_len);
          
          Udp.begin(udp_Port);
            
          Udp.beginPacket(ip_remote, udp_Port);//send ip to server
          Udp.write(buffer_msg);   
          Udp.endPacket();

        }
          msg_wifi="";
    
  }
