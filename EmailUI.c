#include <SPI.h>

#include <Ethernet.h>
#include <EthernetClient.h>

byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x2D, 0xDE };
byte ip[] = { 192,168,0,167 }; // IP address you wish to assign to Arduino
char server[] = "pop.mail.yahoo.com"; // IP address of your POP3 server
char inString[165]; // Number of characters to be collected
int i = 0;
int mailNum1 = 0; // First digit of the email number
int mailNum2 = 0; // Second digit
int mailTotal = 0; // Total # of messsage
char d;
int ledPin = 13;
EthernetClient client; //The default POP port is 110
long updateTimer;
boolean clientConnected = false;

void setup()
{
  system("ifdown eth0");
  system("ifup eth0");
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("Attempting to start Ethernet");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Serial.println("Attempting to configure Ethernet using Static IP");
    Ethernet.begin(mac, ip);
  }
  Serial.print("Your IP address: ");
  Serial.println(Ethernet.localIP());
  system("curl -u username:password --silent https://mail.google.com/mail/feed/atom" | perl -ne 
'print "\t" if /<name>/; print "$2\n" if /<(title|name)>(.*)<\/\1>/;'");
}

void loop()
{
  updateClient();
  d = checkAvail();
  if (d >= 10){
    getMailNum();
  }
}

/*========================================

        Functions

=========================================*/

void updateClient() //This function contacts the POP3 server
{
  if ((millis() - updateTimer) > 5000)
  {
   Ethernet.begin(mac, ip);
   Serial.println("connecting...");
   delay(1000);

   if (client.connect(server, 995))
   {
     Serial.println("connected");
     client.println("user cusdgalileo@yahoo.com"); //Insert your usual email login name
     client.println("pass sustainabledesign"); //And your password here
     client.println("quit");
     client.println();
     clientConnected = true;
   }
   else
   {
     Serial.println("connection failed");
   }
   updateTimer = millis();
  }
}

char checkAvail() //This checks if there is data available and returns a char
{
  if (clientConnected) {
   if (client.available()) {
     char c = client.read();
     return(c);
   }
   if (!client.connected()) {
     Serial.println();
     Serial.println("disconnecting.");
     client.stop();
     clientConnected = false;
   }
  }
}

int getMailNum() //This actually loads the char returned by checkAvail() and puts in into an array
{
  inString[i] = d;
  i++;
  if (i == 165) {
     i = 0;
     client.flush();
     mailNum1 = inString[106] - 48; //Array position 106 contains the first digit
     mailNum2 = inString[107] - 48; //Array position 107 contains the 2nd digit if it is available
     if ((mailNum2 >= 0) && (mailNum2 <= 9)) { //If mailNum2 is present, then it is a two digit mail count
       mailTotal = (mailNum1 * 10) + mailNum2; //when 2 digits are present, multiply the 1st by 10 then add to mailTotal
       Serial.print("Total emails:  ");
       Serial.println(mailTotal);
       blinkLED(); //Run the blink function as many times as there are emails
     }
     else {
       if ((mailNum1 >= 0) && (mailNum1 <= 9)) { //if there is only one digit, then that is mailTotal
         mailTotal = mailNum1;
         Serial.print("Total emails:  ");
         Serial.println(mailTotal);
         blinkLED(); //Blink the LED
       }
     }
   }
}

void blinkLED()
{ //Blinks the LED for as many times as indicated by mailTotal
   for(int x = mailTotal; x >= 1; x--){
     digitalWrite(ledPin, HIGH);
     delay(200);             
     digitalWrite(ledPin, LOW);
     delay(200);
   }
}
