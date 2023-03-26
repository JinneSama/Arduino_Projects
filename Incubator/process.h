#include "SDCard.h"
#include <Wire.h>

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
double saveCounter = 0;

int heater1Relay = 2;
int heater2Relay = 3;    
int exhaustRelay = 4;
int blowerRelay = 5;
int coolingFanRelay = 6;
int circulatingFanRelay = 7;
int humidifierRelay = 8;
boolean _canToggle = true;

boolean _done = false;
double _doneTime = 0;

boolean resetScreen = false;
boolean tempStable = false;
boolean humidStable = false;

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
  pinMode(heater1Relay , OUTPUT);
  pinMode(heater2Relay , OUTPUT);
  pinMode(exhaustRelay , OUTPUT);
  pinMode(blowerRelay , OUTPUT);
  pinMode(coolingFanRelay , OUTPUT);
  pinMode(circulatingFanRelay , OUTPUT);
  pinMode(humidifierRelay , OUTPUT);
}

void setOffState(){
  digitalWrite(heater1Relay , HIGH);
  digitalWrite(heater2Relay , HIGH);
  digitalWrite(exhaustRelay , HIGH);
  digitalWrite(blowerRelay , HIGH);
  digitalWrite(coolingFanRelay , HIGH);
  digitalWrite(circulatingFanRelay , HIGH);
  digitalWrite(humidifierRelay , HIGH);
}

void setState(){
  digitalWrite(heater1Relay , !heater1State);
  digitalWrite(heater2Relay , !heater2State);
  digitalWrite(exhaustRelay , !exhaustFanState);
  digitalWrite(blowerRelay , !blowerFanState);
  digitalWrite(coolingFanRelay , !coolingFanState);
  digitalWrite(circulatingFanRelay , !circulatingFanState);
  digitalWrite(humidifierRelay , !humidifierState);
}

void tempAboveMax(){
  if(millis() > saveCounter){
    saveCounter += 5000;
    writeDataLog(tempValue , humidValue);
  }
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
  if(millis() > saveCounter){
    saveCounter += 5000;
    writeDataLog(tempValue , humidValue);
    }
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
  tempStable = true;
}

void humidInRange(){
  exhaustFanState = false;
  circulatingFanState = false;
  humidifierState = false;
  humidStable = true;

  if(humidStable && tempStable){
    if (millis() > timeCounter){
      timeCounter = millis() + 1000;
      stableTime += 1000;
    }
      
    if (stableTime >= 300000){
      exhaustFanState = false;
      blowerFanState = false;
      coolingFanState = false;
      circulatingFanState = false;
      humidifierState = false;
      _done = true;
      _doneTime = millis() + 60000;
      resetScreen = true;
    }
  }
}


void humidCheck(float humid){
  if (inRange(humid , minHumid , maxHumid)){
    humidInRange();
  }else{
    if (humid > maxHumid){
      exhaustFanState = true;
      humidifierState = false;
      circulatingFanState = false;
      if(millis() > saveCounter){
        saveCounter += 5000;
        writeDataLog(tempValue , humidValue);
      }
    }else{
      humidifierState = true;
      circulatingFanState = true;
      exhaustFanState = false;
       if(millis() > saveCounter){
        saveCounter += 5000;
        writeDataLog(tempValue , humidValue);
      }
    }
  }
}

void tempCheck(float temp){
  tempValue = temp;
  if (inRange(temp, minTemp , maxTemp)){
    tempInRange();
  }else{
    if (temp > maxTemp){
      tempAboveMax();
    }else{
      tempBelowMin();
    }
  }
  humidCheck(humidValue);
}
