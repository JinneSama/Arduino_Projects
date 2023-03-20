#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "process.h"

#define DHTPIN 0
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define ONE_WIRE_BUS 1
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 20, 4);

int toggleCuring = A0;
int toggleStorage = A1;

void setup() {
  initRelays();
  initSD();
  
  dht.begin();
  sensors.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(toggleCuring , INPUT_PULLUP);
  pinMode(toggleStorage , INPUT_PULLUP);
}

void loop() {
  if (digitalRead(toggleCuring) == HIGH){
    minTemp = 34;
    maxTemp = 38;
    minHumid = 65;
    maxHumid = 75;
    lcd.setCursor(7, 1);
    lcd.print("Curing");
    if (_canToggle){
      _canToggle = false;
      DateTime now = rtc.now();
      currentFile = createFile(String(now.year()) + "_" + String(now.month()) + "_" + String(now.day()) + "  " + String(now.hour()) + "-" + String(now.minute()) + "-" + String(now.second()));
      writeToFile(currentFile , "Curing Process");
      writeToFile(currentFile , "Time          Temperature     Humidity");
    }
  }else if(digitalRead(toggleStorage) == HIGH){
    minTemp = 25;
    maxTemp = 30;
    minHumid = 65;
    maxHumid = 75;
    lcd.setCursor(7, 1);
    lcd.print("Storage");
    if (_canToggle){
      _canToggle = false;
      DateTime now = rtc.now();
      currentFile = createFile(String(now.year()) + "_" + String(now.month()) + "_" + String(now.day()) + "  " + String(now.hour()) + "-" + String(now.minute()) + "-" + String(now.second()));
      writeToFile(currentFile , "Storage Process");
    }
  }else{
      _canToggle = true;
    return;
  }
  
  sensors.requestTemperatures();
  float _temperature = sensors.getTempCByIndex(0);
  humidValue = dht.readHumidity();
  float heatIndex = dht.computeHeatIndex(_temperature, humidValue);

  tempCheck(_temperature);
  setState();
  printTemp(_temperature , heatIndex);
}

void printTemp(float t,float h){
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print("Humidity: ");
  lcd.print(humidValue);
  lcd.print(" %");
  lcd.setCursor(0, 3);
  lcd.print("Heat index: ");
  lcd.print(h);
  lcd.print(" C");
}
