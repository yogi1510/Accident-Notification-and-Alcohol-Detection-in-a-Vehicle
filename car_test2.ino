 #include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <SoftwareSerial.h>
SoftwareSerial sim808(22,23);
  const int RMF = 3; // Right Motor Forward IN1
  const int RMB = 4; // Right Motor Backward IN2
  const int LMF = 5; // Left Motor Forward IN3
  const int LMB = 6; // Left Motor Backward IN4
  int limit;//used in gas sensor
  int value;//used in gas sensor
  int state;
  const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
  const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
  const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino
  int c=0;
 
  double latitude,longitude;
SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
  void setup() 
  {
    Serial.begin(9600);
    sim808.begin(9600);
    pinMode(ledPin, OUTPUT);
  pinMode (RMF, OUTPUT); // Setting all the motor pins as OUTPUT
 pinMode (RMB, OUTPUT);
 pinMode (LMF, OUTPUT);
 pinMode (LMB, OUTPUT);
Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
 }
 
 void loop(){
  
 
if(Serial.available() > 0)  
  
      state = Serial.read();
 
      
        value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
        limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
 
        if (limit == LOW){
         digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator 
          
        digitalWrite(RMF, HIGH);
        digitalWrite(RMB, HIGH);
        digitalWrite(LMF, HIGH);
        digitalWrite(LMB, HIGH);
        
    while(serial_connection.available())//While there are characters to come from the GPS
  {
   gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time

  }
  if(gps.location.isUpdated()&&c==0)//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS uni
   latitude=gps.location.lat();
   longitude=gps.location.lng();
  
    Serial.print(latitude,6);
    Serial.print(",");
    Serial.print(longitude,6);
    delay(300);
    sim808.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);  // Delay of 1 second
     sim808.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // Replace x with mobile number
     delay(1000);
    sim808.print("http://maps.google.com/maps?q=loc:");
    sim808.print(latitude);
    sim808.print(",");
    sim808.print(longitude);
    sim808.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(1000);
     c=c+1;
  }

  }
         else{
         digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
           
  
       if (state == 'F') {
 
 // Running Robot Forward
 digitalWrite(RMF, HIGH);
 digitalWrite(RMB, LOW);
 digitalWrite(LMF, HIGH);
 digitalWrite(LMB, LOW);
 }

 else  if (state == 'B') {
 // Running Robot Backward
 
 digitalWrite(RMF, LOW);
 digitalWrite(RMB, HIGH);
 digitalWrite(LMF, LOW);
 digitalWrite(LMB, HIGH);
 }

 
 // Taking Right Turn
 else if (state == 'R') {
 digitalWrite(RMF, LOW);
 digitalWrite(RMB, LOW);
 digitalWrite(LMF, HIGH);
 digitalWrite(LMB, LOW);
 }
 
 
 // Taking Left Turn
 else if (state == 'L') {
 digitalWrite(RMF, HIGH);
 digitalWrite(RMB, LOW);
 digitalWrite(LMF, LOW);
 digitalWrite(LMB, LOW);
} 

 
 // Stop the Robot
 else if (state == 'S') { 
 digitalWrite(RMF, LOW);
 digitalWrite(RMB, LOW);
 digitalWrite(LMF, LOW);
 digitalWrite(LMB, LOW);
 }
 
 }
  }
