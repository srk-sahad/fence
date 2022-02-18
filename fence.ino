#include<Wire.h>
#include<ds3231.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900(10, 11);

char incomingChar;
void send_message();

boolean SMSRequest();

struct ts t;

int timeCheck=0;

// 1 min timer
int timePeriod=1;

void setup() {
  Serial.begin(9600);
  SIM900.begin(9600);
  delay(10000);
  Serial.println("SIM900 ready...");
  Wire.begin();
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  
  DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  t.hour=12;
  t.min=30;
  t.sec=0;
  t.mday=25;
  t.mon=12;
  t.year=2019;

 timeCheck=t.min;
  DS3231_set(t);
}

void loop() {
 
  if(SIM900.available()>0)
  {
    while(SIM900.read()!='S')              //Waiting for message
    {
      if(t.min==timeCheck+timePeriod){
      //type message sending code below               //RTC code will run if the message not received 
       SIM900.println("AT + CMGS = \"+919746957905\"");
    delay(100);
    String dataMessage = ("SUPPLY is ON");


    SIM900.print(dataMessage);
      Serial.println(dataMessage);
      delay(100);


      SIM900.println((char)26); 
      delay(100);
      SIM900.println();
      // Give module time to send SMS
      delay(5000);  
    
    Serial.println("hello????????????");
  
  

      //reseting timer
      }
      if(timeCheck+timePeriod>=60){
      t.min=0;
      timeCheck=(t.min);
      }
      else
      timeCheck=t.min;
   
   
   

   
 
    DS3231_get(&t);
    Serial.print("Date : ");
    Serial.print(t.mday);
    Serial.print("/");
    Serial.print(t.mon);
    Serial.print("/");
    Serial.print(t.year);
    Serial.print("\t Hour : ");
    Serial.print(t.hour);
    Serial.print(":");
    Serial.print(t.min);
    Serial.print(".");
    Serial.println(t.sec);
 
    delay(1000);
      }

      incomingChar=SIM900.read();            //change
      delay(10);
      Serial.print(incomingChar);            //reading each character from the message

      
      incomingChar=SIM900.read();
      if(incomingChar =='T') {
        delay(10);
        Serial.print(incomingChar);

        
        incomingChar=SIM900.read();
        if(incomingChar=='A') {
          delay(10);
          Serial.print(incomingChar);

          
          incomingChar=SIM900.read();
          if(incomingChar=='T') {
            delay(10);
            Serial.print(incomingChar);

            
            incomingChar=SIM900.read();
            if(incomingChar=='E') {
              delay(10);
              Serial.print(incomingChar);
              Serial.print("...Request Received \n");
              }
          }
        }
      }

      send_message();                      //Sending message back to the cellular mobile in corresponds to the incoming message 
    }
    else  {
      Serial.println("GSM Module isn't connected ");                 //if the GSM Module is not connected
      }
  
  
}





void send_message() {

  delay(10);
    
    SIM900.println("AT + CMGS = \"+91xxxxxxxxxx\"");
    delay(100);
    String dataMessage = ("SUPPLY is ON");


    SIM900.print(dataMessage);
      delay(100);


      SIM900.println((char)26); 
      delay(100);
      SIM900.println();
      // Give module time to send SMS
      delay(5000);  
    }
