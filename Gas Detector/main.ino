#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "mq2.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//treshold t gas PPM
int lpgTreshold = 300;
int lpgDangerTreshold = 400;

//treshold t CO PPM
int coTreshold = 400;
int coDangerTreshold = 500;

//treshold t smoke PPM
int smokeTreshold = 400;
int smokeDangerTreshold = 500;
                                                                      
long iPPM_LPG = 0;
long iPPM_CO = 0;
long iPPM_Smoke = 0;

//pakisukat jai buizzer to d2 analog gmin jai A2 , swap to digita D2
int buzzer=2;
float lowestVolt = 100;

String type = "No Threat";
float voltage = 0;

int btnPin = 3;
int buzzPin = 2;
void setup(){ 
  Serial.begin(9600);
  
  pinMode(buzzer, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, OUTPUT);
  //Battery Sensor Pin A6 , pakisukatan tun nu sabali
  pinMode(A6 , INPUT);
  pinMode(btnPin, INPUT);
  pinMode(A7 , INPUT);
  Serial.println("start");
  CalibrateLED();
  
  Ro = MQCalibration(MQ_PIN);
  Serial.println("done");
  delay(3000);
}
 
void loop(){  
  iPPM_LPG = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_LPG);
  iPPM_CO = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO);
  iPPM_Smoke = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE);

  if(iPPM_LPG < 0){
    iPPM_LPG = 0;
  }
  
  if(iPPM_LPG < 0){
    iPPM_CO = 0;
  }

  if(iPPM_LPG < 0){
    iPPM_Smoke = 0;
  }

  //voltage battery calculation , paki multiply ti 10 or 100 tanu maala jai 100%
  int sensorValue = analogRead(A6);
  float voltage = (sensorValue * (3.7 / 1023.00) * 2) * 10;
  
  drawSensorValue(String(iPPM_LPG) , String(iPPM_CO) , String(iPPM_Smoke) , String(voltage));

  if(digitalRead(btnPin) == HIGH){
    gasType += 1;
    if(gasType > 3){
      gasType = 1;
    }
    Serial.println(gasType);
    delay(200);
  }

  if(gasType == 1){
    if(iPPM_LPG < lpgTreshold){
      type = "No Threat";
      digitalWrite(buzzPin , LOW);
      RGB(0 , 255 , 0);
    }else if(iPPM_LPG > lpgTreshold && iPPM_LPG < lpgDangerTreshold){
      type = "Minimal";
      digitalWrite(buzzPin , HIGH);
      RGB(0 , 0 , 255);
    }else if(iPPM_LPG > lpgDangerTreshold){
      type = "Danger";
      digitalWrite(buzzPin , HIGH);
      RGB(255 , 0 , 0);
    }
  }

  if(gasType == 2){
    if(iPPM_CO < coTreshold){
      type = "No Threat";
      digitalWrite(buzzPin , LOW);
      RGB(0 , 255 , 0);
    }else if(iPPM_CO > coTreshold && iPPM_CO < coDangerTreshold){
      type = "Minimal";
      digitalWrite(buzzPin , HIGH);
      RGB(0 , 0 , 255);
    }else if(iPPM_CO > coDangerTreshold){
      type = "Danger";
      digitalWrite(buzzPin , HIGH);
      RGB(255 , 0 , 0);
    }
  }

  if(gasType == 3){
    if(iPPM_Smoke < smokeTreshold){
      type = "No Threat";
      digitalWrite(buzzPin , LOW);
      RGB(0 , 255 , 0);
    }else if(iPPM_Smoke > smokeTreshold && iPPM_Smoke < smokeDangerTreshold){
      type = "Minimal";
      digitalWrite(buzzPin , HIGH);
      RGB(0 , 0 , 255);
    }else if(iPPM_Smoke > smokeDangerTreshold){
      type = "Danger";
      digitalWrite(buzzPin , HIGH);
      RGB(255 , 0 , 0);
    }
  }
}

void CalibrateLED(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);
}

void RGB(int r, int g, int b){
  analogWrite(9, r);
  analogWrite(10, g);
  analogWrite(11, b);
}

void drawSensorValue(String lpg , String co , String smoke , String volt) {
  display.clearDisplay();
  
  display.setCursor(0 , ((gasType - 1) * 8));
  display.setTextColor(SSD1306_WHITE);
  display.println(">");
  
  //atuy jai agprint t values smoke co ken gas , pakisukat x,y coordinates na nu madi sakto
  display.setCursor(10,0);
  display.setTextColor(SSD1306_WHITE);
  display.println("lpg:   " + lpg + "ppm");

  display.setCursor(10,8);
  display.setTextColor(SSD1306_WHITE);
  display.println("CO:    " + co + "ppm");

  display.setCursor(10,16);
  display.setTextColor(SSD1306_WHITE);
  display.println("Smoke: " + smoke + "ppm");

  //atuy jai pang display battery , pakisukat x,y coordinates na nu madi sakto
  display.setCursor(100,25);
  display.setTextColor(SSD1306_WHITE);
  display.println(volt + "%");

  
  display.setCursor(0,25);
  display.setTextColor(SSD1306_WHITE);
  display.println(type);

  display.display();
}
