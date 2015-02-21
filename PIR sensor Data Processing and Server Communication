#include <SPI.h>
#include <Ethernet.h>

// the media access control (ethernet hardware) address for the Galileo:
byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x47, 0x91 };  
//static IP address for the Galileo:
byte ip[] = { 192, 168, 1, 49 };

// Web address for Cloud Server
char server[] = "mainloadbalancer-400162624.us-west-2.elb.amazonaws.com";  

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
String s;

/////////////////////////////
//needed for PIR sensor//////
/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int mypause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 2;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;   //the digital pin connected to the Galileo's built-in LED

void setup() {
    Serial.begin(115200);  //115200 baud rate for Serial output
    //////////////////
    // PIR Setup Code
    pinMode(pirPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(pirPin, LOW);
    //give the sensor some time to calibrate
    Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  
    /// End of PIR Setup Code
    /////////////////
    
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
  // Motion is detected
  if(digitalRead(pirPin) == HIGH){
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    if(lockLow){  
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;            
      Serial.println("---");
      Serial.println("motion detected ");
      httpRequestMotion();
    }
    delay(50);
    takeLowTime = true;
    }
    
  // Motion has ended
  if(digitalRead(pirPin) == LOW){
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

    if(takeLowTime){
      lowIn = millis();     //save the time of the transition from high to low
      takeLowTime = false;  //make sure this is only done at the start of a low phase
    }

    if(!lockLow && millis() - lowIn > mypause){  
      lockLow = true;                        
      Serial.println("motion ended ");      //output
      httpRequestNoMotion();
      delay(50);
    }
  }
}

// Method for connecting to server and sending data corresponding to motion detected
void httpRequestMotion() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP GET request:
    client.println("GET /insertPIR?isinroom=true&roomid=2 HTTP/1.0");
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

// Method for connecting to server and sending data corresponding to motion ending
void httpRequestNoMotion() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP GET request:
    client.println("GET /insertPIR?isinroom=false&roomid=2 HTTP/1.0");
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
