#include "SDCard.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
File currentFile;

float maxTemp = 30;
float minTemp = 25;

float minHumid = 65;
float maxHumid = 70;

float humidValue = 0;
float tempValue = 0;

bool heater1State = false;
bool heater2State = false;

bool exhaustFanState = false;
bool blowerFanState = false;
bool coolingFanState = false;
bool circulatingFanState = false;
bool humidifierState = false;

double stableTime = 0;
double timeCounter = 0;

int exhaustRelay = 4;
int blowerRelay = 5;
int coolingFanRelay = 6;
int circulatingFanRelay = 7;
int humidifierRelay = 8;
boolean _canToggle = true;

bool inRange(float val, float minimum, float maximum)
{
  return ((minimum <= val) && (val <= maximum));
}

void initRTC(){
  Wire.begin();
  if (!rtc.begin()) {
    while (1);
  }  
}

void initRelays(){
  initRTC();
  pinMode(exhaustRelay , OUTPUT);
  pinMode(blowerRelay , OUTPUT);
  pinMode(coolingFanRelay , OUTPUT);
  pinMode(circulatingFanRelay , OUTPUT);
  pinMode(humidifierRelay , OUTPUT);
}

void setState(){
  digitalWrite(exhaustRelay , exhaustFanState);
  digitalWrite(blowerRelay , blowerFanState);
  digitalWrite(coolingFanRelay , coolingFanState);
  digitalWrite(circulatingFanRelay , circulatingFanState);
  digitalWrite(humidifierRelay , humidifierRelay);
}

void tempAboveMax(){
  DateTime now = rtc.now();
  writeToFile(currentFile , String(now.hour()) + "-" + String(now.minute()) + "-" + String(now.second()) + "          " + String(tempValue) + "C" + "          " + String(humidValue) + "%" );
  if (exhaustFanState){
    if (heater1State){
        heater2State = false;
        blowerFanState = false;
    }else{
      heater1State = false;
      blowerFanState = false;
    }
  }else{
    coolingFanState = true;
    exhaustFanState = false;
  }
} 

void tempBelowMin(){
  DateTime now = rtc.now();
  writeToFile(currentFile , String(now.hour()) + "-" + String(now.minute()) + "-" + String(now.second()) + "          " + String(tempValue)  + "C" + "          " + String(humidValue) + "%" );
  if (heater1State){
    heater2State = true;
    blowerFanState = true;
  }else{
    heater1State = true;
    blowerFanState = true;
  }
} 

void tempInRange(){
  heater1State = false;
  heater2State = false;
  coolingFanState = false;
  blowerFanState = false;
  exhaustFanState = false;
}

void humidInRange(){
  exhaustFanState = false;
  circulatingFanState = false;
  humidifierState = false;

  if (millis() > timeCounter){
    timeCounter = millis() + 100;
    stableTime += 100;
  }
    
  if (stableTime >= 50000){
    exhaustFanState = false;
    blowerFanState = false;
    coolingFanState = false;
    circulatingFanState = false;
    humidifierState = false;
    _canToggle = true;
  }
}


void humidCheck(float humid){
  if (inRange(humid , minHumid , maxHumid)){
    humidInRange();
  }else{
    if (humid > maxHumid){
      exhaustFanState = true;
    }else{
      humidifierState = true;
      circulatingFanState = true;
    }
  }
}

void tempCheck(float temp){
  tempValue = temp;
  if (inRange(temp, minTemp , maxTemp)){
    tempInRange();
    humidCheck(humidValue);
  }else{
    if (temp > maxTemp){
      tempAboveMax();
    }else{
      tempBelowMin();
    }
  }
}
