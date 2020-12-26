#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
SoftwareSerial mySerial(9, 10);

int sensor=A1;
float temp_read,Temp_alert_val,Temp_shut_val;
int sms_count=0,Fire_Set;
int buzzer = 8;
int LED = 7;
int flame_sensor = 6;
int flame_detected;


void setup()
{
  pinMode(sensor,INPUT);
  mySerial.begin(9600);   
  Serial.begin(9600);    
  lcd.begin(16,2);  
  delay(500);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(flame_sensor, INPUT);
}

void loop()
{
CheckFire();
CheckShutDown();
flame_detected = digitalRead(flame_sensor);
  if (flame_detected == LOW)
  {
    Serial.println("Flame detected...! take action immediately.");
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
    digitalWrite(LED, LOW);
  }
  delay(1000);
}

void CheckFire()
{
lcd.setCursor(0,0);
lcd.print("Fire Scan - ON");
Temp_alert_val=CheckTemp();
if(Temp_alert_val>45)
{
 SetAlert(); // Function to send SMS Alerts
}
}

float CheckTemp()
{
temp_read=analogRead(sensor); // reads the sensor output (Vout of LM35)
temp_read=temp_read*5;    // converts the sensor reading to temperature
temp_read=temp_read/10;  // adds the decimal point
return temp_read; // returns temperature value in degree celsius
}

void SetAlert()
{
while(sms_count<3) //Number of SMS Alerts to be sent
{  
SendTextMessage(); // Function to send AT Commands to GSM module
}
Fire_Set=1; 
lcd.setCursor(0,1);
lcd.print("Fire Alert! SMS Sent!");
}

void CheckShutDown()
{
if(Fire_Set==1)
{

Temp_shut_val=CheckTemp();
if(Temp_shut_val<30)
{
lcd.setCursor(0,1);
lcd.print("Fire Shut! SAFE NOW");
sms_count=0;
Fire_Set=0;
}}}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(2000);
  mySerial.println("AT+CMGS=\"+905360687818\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("FIRE DETECTED!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(5000);
  sms_count++;
}
