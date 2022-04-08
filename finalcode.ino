#include <SoftwareSerial.h>
#include <Wire.h>
#include <ds3231.h>

SoftwareSerial SIM900(10, 11);

String readString;
char inchar;
struct ts t;

double read_function();
void SendMessage();
double read_val;
long count;
long mincount = 100;
double temp;
int pin = A0;
String alert_message;
int minCheck;
int minPeriod;
int hourCheck;
int hourPeriod;
int R1,R2;


void setup() {
  Serial.begin(9600);
  SIM900.begin(9600);
  pinMode(pin, INPUT);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  //Setting time
  t.hour=0; 
  t.min=0;
  t.sec=0;
  DS3231_set(t);

  
  pinMode(pin,INPUT);
  minCheck = t.min;
  minPeriod = 5;        //Setting minute interval
  hourCheck = t.hour;
  hourPeriod = 1;       //Setting hour interval
  alert_message = "";       //Alert message


  delay(10000);
  Serial.println("SIM900 ready...");

  SIM900.print("AT+CMGF=1\r");  //text mode 
  delay(100);

  SIM900.print("AT+CNMI=2,2,0,0,0\r");  //recieve mode
  delay(100);

}

void loop() {
  if((t.min==minCheck+minPeriod))       //Checking for 5 minutes of interval for counting codes
  {
    if((t.hour==hourCheck+hourPeriod))        //Checking for 1 hour of interval for sending message
    {
      if(count>=mincount)
      {
        alert_message = "Supply is in good condition";
      }
      else
      {
        alert_message = "Supply is in critical";
      }
      
      SIM900.println("AT+CMGF=1");         //Sets the GSM Module in Text Mode
      delay(1000);         // Delay of 1000 milli seconds or 1 second
      SIM900.println("AT+CMGS=\"+917511140298\"\r");     // Replace x with mobile number
      delay(1000);
      SIM900.println("SUPPLY IS ON\n(Reply message)");      // The SMS text you want to send
      SIM900.println(read_val);
      SIM900.println(alert_message);
      delay(100);
      SIM900.println((char)26);     // ASCII code of CTRL+Z
      delay(1000);
      
      count=0;
      if(hourCheck+hourPeriod==24)
      {
        t.hour=0;
        hourCheck=t.hour;
      }
      else
      hourCheck=t.hour;
    }
    read_val=read_function();       //Reading voltage
    if(read_val >=800)
    {
      count++;
    }

    if ((minCheck+minPeriod==60)){
    t.min=0;
    minCheck=(t.min);
    }
    else
    minCheck=t.min;
  }
  DS3231_get(&t);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  while(SIM900.available() > 0) {inchar = SIM900.read(); readString+=inchar;delay(1);} ///can be a delay up to (10) so you can get a clear reading
    
    Serial.print(readString);  /// Declare a string    " String readString; "
    
    for (int i=0; i<200; i++){  /// Serch for the txt you sent up to (200) times it depends how long your ""readString" is
      if(readString.substring(i,i+6)=="STATUS"){ //// I am looking for the word RING sent from my phone
        SendMessage();
        break;
      }
    }
    readString = "";
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

double read_function()       //Reading function
{
  temp = analogRead(pin);
  temp = (temp*R1)/R2;
  return temp;
  
}

void SendMessage()        //Sending message
{
  SIM900.println("AT+CMGF=1");         //Sets the GSM Module in Text Mode
  delay(1000);         // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+917511140298\"\r");
  delay(1000);
  SIM900.println("SUPPLY IS ON\n(Reply message)");      // The SMS text you want to send
  SIM900.println(read_val);
  delay(100);
  SIM900.println((char)26);     // ASCII code of CTRL+Z
  delay(1000);


}
