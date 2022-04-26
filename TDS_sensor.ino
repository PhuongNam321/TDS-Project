/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"

#define BLYNK_TEMPLATE_ID "TMPLGcoTTy8q"
#define BLYNK_DEVICE_NAME "TDS"
#define BLYNK_AUTH_TOKEN "sqc7cSxQUE-cnEwftsvDhKVaawRa4sMf"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "sqc7cSxQUE-cnEwftsvDhKVaawRa4sMf";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nguyen Cuong";
char pass[] = "nguyencuong379";

int DSPIN = D5;
int TDS_Sensor = A0;
float Aref = 3.3;

float ec_Val = 0;
unsigned int tds_value = 0;
float ecCal = 1;

char warning1[] = "Thông báo: Chất lượng nước ở mức an toàn dùng được trong ăn uống";
char warning2[] = "Thông báo: Chất lượng nước ở mức trung bình chỉ dùng trong sinh hoạt";
char warning3[] = "Thông báo: Chất lượng nước ở mức nguy hiểm nước bị ô nhiễm";

void mainFunction()
{
  double wTemp = TempRead()* 0.0625;  // conversion accuracy is 0.0625 / LSB
  float V_level= Aref / 1024.0;
  float rawEc = analogRead(TDS_Sensor) * V_level;  // Raw  data of EC
  float T_Cof = 1.0 + 0.02 * (wTemp - 25.0);  // Temperature Coefficient

  int counter = 0;
  ec_Val = (rawEc / T_Cof) * ecCal;// temperature and calibration compensation
  tds_value = (133.42 * pow(ec_Val, 3) - 255.86 * ec_Val * ec_Val + 857.39 * ec_Val) * 0.5; 
  
  counter++;
  if(counter >= 10){
    Serial.print("TDS:"); Serial.println(tds_value);
    Serial.print("EC:"); Serial.println(ec_Val, 2);
    Serial.print("Temperature (oC):"); Serial.println(wTemp,2);   
    Serial.print("");
    Blynk.virtualWrite(V0, wTemp);
    Blynk.virtualWrite(V2, tds_value);
    Blynk.virtualWrite(V3, ec_Val);
    if(tds_value <= 80){
      Blynk.virtualWrite(V4, warning1);
    }
    else if(tds_value > 80 && tds_value < 150){
      Blynk.virtualWrite(V4, warning2);
    }
    else if(tds_value >= 150){
      Blynk.virtualWrite(V4, warning3);
    }
    counter = 0;
  }
  
}

void setup()
{
  // Debug console
  long t = millis();
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  mainFunction();
  delay(100);
}

boolean DS18B20_Init()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(DSPIN, LOW);
  delayMicroseconds(750);//480-960
  digitalWrite(DSPIN, HIGH);
  pinMode(DSPIN, INPUT);
  int t = 0;
  while(digitalRead(DSPIN))
  {
    t++;
    if(t > 60) return false;
    delayMicroseconds(1);
  }
  t = 480 - t;
  pinMode(DSPIN, OUTPUT);
  delayMicroseconds(t);
  digitalWrite(DSPIN, HIGH);
  return true;
}
 
void DS18B20_Write(byte data)
{
  pinMode(DSPIN, OUTPUT);
  for(int i=0; i<8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(10);
    if(data & 1) digitalWrite(DSPIN, HIGH);
    else digitalWrite(DSPIN, LOW);
    data >>= 1;
    delayMicroseconds(50);
    digitalWrite(DSPIN, HIGH);
  }
}
 
byte DS18B20_Read()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(2);
  byte data = 0;
  for(int i=0; i<8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);
    delayMicroseconds(5);
    data >>= 1;
    if(digitalRead(DSPIN)) data |= 0x80;
    delayMicroseconds(55);
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
  }
  return data;
}
 
int TempRead()
{
  if(!DS18B20_Init()) return 0;
   DS18B20_Write (0xCC); // Send skip ROM command
   DS18B20_Write (0x44); // Send reading start conversion command
  if(!DS18B20_Init()) return 0;
   DS18B20_Write (0xCC); // Send skip ROM command
   DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
   int waterTemp = DS18B20_Read (); // Low byte
   waterTemp |= DS18B20_Read () << 8; // High byte
  return waterTemp;
}
