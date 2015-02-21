#include <SPI.h>
#include <Ethernet.h>

// the media access control (ethernet hardware) address for the Galileo:
byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x47, 0x91 };
//the IP address for the Galileo:
byte ip[] = { 192, 168, 1, 49 };

// Web address for Cloud Server
char server[] = "mainLoadBalancer-400162624.us-west-2.elb.amazonaws.com";  

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
String s;

void setup() {
  // put your setup code here, to run once:
  system("ifdown eth0");
  system("ifup eth0");
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13, OUTPUT);   
  // Start Ethernet Connection
    Serial.println("Attempting to start Ethernet");
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        Serial.println("Attempting to configure Ethernet using Static IP");
        Ethernet.begin(mac, ip);
    }
    Serial.print("Your IP address: ");
    Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly: 
  Serial1.write("Marco");
  if (Serial1.available() > 0) {
    while(Serial1.available() >0){
      Serial1.read();
    }
    Serial.println("XBee in range!");
    httpRequestXBee(); // Send the thing to the http
    for (int i = 0; i < 125; i++){ 
      Serial1.write("Marco"); // sends broadcast message to XBees in range
      if (Serial1.available() > 0) {
        while(Serial1.available() >0){
        Serial1.read();
        }
        Serial.println("XBee is still there");
        i = 0;
      }
      Serial.println(i);
      delay(2500); //wait 2.5 milisecond
    }
    Serial.println("XBee out of range!");
    httpRequestNoXBee();
  }
  delay(2500);
}

// Method for connecting to server and sending data corresponding to XBee in range
void httpRequestXBee() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP PUT request:
    client.println("GET /insertxBee?isinrange=true&roomid=2 HTTP/1.0");
    //client.println("Host: www.arduino.cc");
    //client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    s = "";
    if (client.available()) {
      while (client.read() != -1) {
        char c = client.read();
        s += c;
      }
      if (s.substring(s.length()-9,s.length()-1) == "ucs nisr") {
        Serial.println("success in insert!");
      }
      client.stop();
    }
    else {
      Serial.println("Client not available :(");
    }
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

// Method for connecting to server and sending data corresponding to XBee not in range
void httpRequestNoXBee() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP PUT request:
    client.println("GET /insertxBee?isinrange=false&roomid=2 HTTP/1.0");
    //client.println("Host: www.arduino.cc");
    //client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    s = "";
    if (client.available()) {
      while (client.read() != -1) {
        char c = client.read();
        s += c;
      }
      if (s.substring(s.length()-9,s.length()-1) == "ucs nisr") {
        Serial.println("success in insert!");
      }
      client.stop();
    }
    else {
      Serial.println("Client not available :(");
    }
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}
