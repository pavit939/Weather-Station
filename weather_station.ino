#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//######################################I_O_T-----P_A_R_T##########################################
//libraries needed
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include<SD.h>
 
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//###############################################BASIC DETAILS OF FIREBASE ACCOUNT AND WIFI ACCESS##########################################
#define FIREBASE_HOST "rpibasic.firebaseio.com"  //should not have http:// and the end /
#define FIREBASE_AUTH "X9ZCyDhzZQBl6SfzK56XC8D3XQRGNHtJysDzY0v2"  // go to settings in firebase,select project settings and service account, secrets auth key
#define WIFI_SSID "Sudesh"
#define WIFI_PASSWORD "9176750174"


//#define BME_SDA D3
//#define BME_SDL D4
 
#define SEALEVELPRESSURE_HPA (1013.25)
 
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
 
unsigned long delayTime;
float t, p, h, d, he;
delayTime = 1000;

//Define Firebase Data object
FirebaseData firebaseData;


float temp()
{

    float fTemp = bme.readTemperature()*9.0/5.0+32;
    return fTemp;
  
}


float pressure()

{
  float bPress = bme.readPressure() / 100.0F / 33.8638866667;
  return bPress;
}


float humid()
{
float rH = bme.readHumidity();
return rH;
  
}


float heat()
{
  float fTemp = bme.readTemperature()*9.0/5.0+32;
  float rH = bme.readHumidity();
  float hIndex = 0.5 * (fTemp + 61.0 + ((fTemp-68.0)*1.2) + (rH*0.094));
  return hIndex;
  
}

float dew()
{
  float fTemp = bme.readTemperature()*9.0/5.0+32;
  float rH = bme.readHumidity();
  float dPoint = fTemp - ((100-rH)/5);
  return dPoint;
  
}
 
void setup() {
    Serial.begin(9600);
    /*Serial.println("BME280 test");
 
    bool status;
   
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
   
    Serial.println("-- Default Test --");
    delayTime = 1000;
 
    Serial.println();
 
    delay(100); // let sensor boot up*/

//#############################################STARTING UP FIREBASE AND CONNECTING TO WIFI##############################
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
//##############################################CONNECTING TO THE DESIRED FIREBASE ACCOUNT#######################################
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  

}
 
 
void loop() 
{
  t = temp();
  p = pressure();
  h = humid();
  d = dew();
  he = heat();
  Firebase.pushFloat(firebaseData, "/WEATHER/TEMPERATURE", t);// posting temperature
  
  Firebase.pushFloat(firebaseData, "/WEATHER/PRESSURE", p);// posting pressure
  
  Firebase.pushFloat(firebaseData, "/WEATHER/HUMIDITY", h); //posting humidity
  
  Firebase.pushFloat(firebaseData, "/WEATHER/DEW_POINT", d);//posting dewpoint
  
  Firebase.pushFloat(firebaseData, "/WEATHER/HEAT_INDEX", he);//posting heat index
  delay(10000);

  Firebase.deleteNode(firebaseData, "/WEATHER");//deleting the node after 10 seconds
}
 
 
/*void printValues() {
    Serial.print("Temperature = ");
    //Serial.print(bme.readTemperature());
    //Serial.println(" *C");
    float fTemp = bme.readTemperature()*9.0/5.0+32;
    Serial.print(fTemp);
    Serial.println(" *F");
   
 
    Serial.print("Pressure = ");
 
    //Serial.print(bme.readPressure() / 100.0F);
    //Serial.println(" hPa");
    float bPress = bme.readPressure() / 100.0F / 33.8638866667;
    Serial.print(bPress);
    Serial.println(" inHg");
 
    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");
 
    Serial.print("Humidity = ");
    //Serial.print(bme.readHumidity());
    float rH = bme.readHumidity();
    Serial.print(rH);
    Serial.println(" %");
   
 
    Serial.print("Dew Point = ");
    float dPoint = fTemp - ((100-rH)/5);
    Serial.print(dPoint);
    Serial.println(" *F");
 
 
    Serial.print("Heat Index = ");
    float hIndex = 0.5 * (fTemp + 61.0 + ((fTemp-68.0)*1.2) + (rH*0.094));
    Serial.print(hIndex);
    Serial.println(" *F");
   
    Serial.println();*/
