#include <Wire.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "process.h"

#define DHTPIN A2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

#define ONE_WIRE_BUS A3 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int toggleCuring = A0;
int toggleStorage = A1;

float humidValues = 0;
void setup() {
  initRelays();
  setOffState();
  
  dht.begin();
  sensors.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  initSD();

  pinMode(toggleCuring , INPUT_PULLUP);
  pinMode(toggleStorage , INPUT_PULLUP);
}

void loop() {
  if (digitalRead(toggleCuring) == LOW){
    minTemp = 34;
    maxTemp = 38;
    minHumid = 65;
    maxHumid = 75;
    if (_canToggle){
      stableTime = 0;
      lcd.clear();
      _canToggle = false;
      writeHeaderTime("CURING");
      writeToFile("Time          Temperature     Humidity");
    }
    lcd.setCursor(4, 0);
    lcd.print("CURING MODE");
  }else if(digitalRead(toggleStorage) == LOW){
    minTemp = 25;
    maxTemp = 30;
    minHumid = 65;
    maxHumid = 75;
    if (_canToggle){
      stableTime = 0;
      lcd.clear();
      _canToggle = false;
      writeHeaderTime("STORAGE");
      delay(100);
      writeToFile("Time          Temperature     Humidity");
      delay(100);
    }
    lcd.setCursor(3, 0);
    lcd.print("STORAGE MODE");
  }else{
    if(!_canToggle){
      stableTime = 0;
      lcd.clear();
      _canToggle = true;
    }
    _done = false;    
    lcd.setCursor(3, 0); 
    lcd.print("None Selected");
    setOffState();
    return;
  }

  sensors.requestTemperatures();
  float _temperature = sensors.getTempCByIndex(0);
  humidValue = dht.readHumidity();
  
  if(!_done){
      tempCheck(_temperature);
      setState();
      printTemp(_temperature);
  }else{
    if(millis() > _doneTime){
      _canToggle = true;
      _done = false;
    }else{
      if(resetScreen){
        resetScreen = false;
        clearLCD(0,1,1,1); 
      }
      lcd.setCursor(2, 2);
      lcd.print("Resetting in ");
      int tm = (_doneTime - millis())/1000;
      lcd.setCursor(15, 2);
      lcd.print("  ");
      lcd.setCursor(15, 2);
      lcd.print(String(tm));
    }
  }
}


void printTemp(float t){
  lcd.setCursor(0, 1);
  lcd.print("Temp  :");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print("Humid :");
  lcd.print(humidValue);
  lcd.print(" %");
  
  lcd.setCursor(0, 3);
  lcd.print("Stable Time >");
  int seconds = stableTime / 1000;
  int minutes = seconds/60;
  seconds %= 60;

  lcd.setCursor(14, 3);
  if(minutes < 10){
    lcd.print("0" + String(minutes) + ":");
  }else{
    lcd.print(String(minutes) + ":");
  }
  lcd.setCursor(17, 3);
  if(seconds < 10){
    lcd.print("0" + String(seconds));
  }else{
    lcd.print(String(seconds));
  }
}
