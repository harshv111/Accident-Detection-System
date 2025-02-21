#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
SoftwareSerial SIM900(3, 4);
TinyGPSPlus gps;
int value = 0;
SoftwareSerial ss(5, 6);
//char longitude;
//char latitude;
char longitude[16]="77.4981559";
char latitude[16]="28.4571833";
MPU6050 mpu6050(Wire);
long timer = 0;

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Wire.begin();
  mpu6050.begin();  
  delay(2000);
  
}

void SendMessage()
{
  delay(500);
  SIM900.begin(9600);
  delay(3000);
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+919891542888\"\r"); // Replace x with mobile number0
  delay(1000);
  //SIM900.println("\n http://www.google.com/maps/place/"+String(latitude)+","+String(longitude));
  SIM900.print("Accident Detected ");
  SIM900.print("https://www.google.com/maps/place/");
  SIM900.print(latitude);
  SIM900.print(',');
  SIM900.print(longitude);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  Serial.println("SMS sent");
  delay(3000);
}
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  Serial.println();
}

void loop()
{
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  if (gps.location.isValid())
    delay(1000);
    
  
    mpu6050.update();
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
    if (mpu6050.getAccX() >= 0.70 || mpu6050.getAccX() <= -0.60 || mpu6050.getAccY() >= 0.60 || mpu6050.getAccY() <= -0.60 )
    {
      Serial.println("Sending SMS ...");
      SendMessage();
      ss.begin(9600); 
    }
  }