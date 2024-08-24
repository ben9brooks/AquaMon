#include "SensorType.h"
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Sensor Setup (Temp - Red to VCC, Blue to GND, Yellow to A0. Data and VCC need pull up resistor)
#define ONE_WIRE_BUS 14 //This number is the unique "D" identifier for each pin out 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LCD Setup 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// WiFi Setup
char ssid[] = "ENTER WIFI";
char pass[] = "ENTER PASSWORD";
int keyIndex = 0; 
int status = WL_IDLE_STATUS;
WiFiServer server(80); //port 80
WiFiClient client = server.available();

void setup() {
  Serial.begin(9600);
  while(!Serial);

  sensors.begin();

  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWiFiStatus();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("AquaMon");
}

void loop() {
  client = server.available();

  if(client){
    
    printWEB();
  }
  //Serial.print(readTemperatureFahrenheit());
  //Serial.print("\n");
}


float readTemperatureFahrenheit()
{
  float reading = analogRead(A0); //For UNO Rev 2 this is a digital pin (D14) or Analog (A0) 
  sensors.requestTemperatures();
  float temp = (sensors.getTempCByIndex(0)); //celsius reading
  temp = (temp * 1.8) +32; // converting to farenheit
  if (temp < 0)
  {
    return -1.0; //error
  }
  return temp;
}