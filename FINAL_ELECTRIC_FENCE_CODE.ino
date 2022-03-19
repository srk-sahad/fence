#include <SoftwareSerial.h>
#include <Wire.h>
#include <ds3231.h>

SoftwareSerial SIM900(10, 11);

String readString;
char inchar;

struct ts t; 
int timeCheck=0;
int timePeriod=1;


void setup() {
  Serial.begin(9600);
  SIM900.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  t.hour=12; 
  t.min=30;
  t.sec=0;
  timeCheck=t.min;
  DS3231_set(t);
  delay(10000);
  Serial.println("SIM900 ready...");

  SIM900.print("AT+CMGF=1\r");  //text mode 
  delay(100);

  SIM900.print("AT+CNMI=2,2,0,0,0\r");  //recieve mode
  delay(100);
}

void loop() {

  if((t.min==timeCheck+timePeriod)){
    
    SIM900.println("AT+CMGF=1");         //Sets the GSM Module in Text Mode
    delay(1000);         // Delay of 1000 milli seconds or 1 second
    SIM900.println("AT+CMGS=\"+919847359440\"\r");     // Replace x with mobile number
    delay(1000);
    SIM900.println("SUPPLY IS ON\n(Timer)");      // The SMS text you want to send
    delay(100);
    SIM900.println((char)26);     // ASCII code of CTRL+Z
    delay(1000);
    
    
    if ((timeCheck+timePeriod>=60)){
    t.min=0;
    timeCheck=(t.min);
    }
    else
    timeCheck=t.min;
    }


    DS3231_get(&t);


    while(SIM900.available() > 0) {inchar = SIM900.read(); readString+=inchar;delay(1);} ///can be a delay up to (10) so you can get a clear reading
    
    Serial.print(readString);  /// Declare a string    " String readString; "
    
    for (int i=0; i<200; i++){  /// Serch for the txt you sent up to (200) times it depends how long your ""readString" is
      if(readString.substring(i,i+6)=="STATUS"){ //// I am looking for the word RING sent from my phone
        SendMessage();
        break;
      }
    }
    readString = "";

}

void SendMessage()
{
  SIM900.println("AT+CMGF=1");         //Sets the GSM Module in Text Mode
  delay(1000);         // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+919847359440\"\r");     // Replace x with mobile number
  delay(1000);
  SIM900.println("SUPPLY IS ON\n(Reply message)");      // The SMS text you want to send
  delay(100);
  SIM900.println((char)26);     // ASCII code of CTRL+Z
  delay(1000);


}
