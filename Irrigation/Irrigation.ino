#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

#define relay1 3
#define relay2 4
#define relay3 5
#define relay4 6
#define relay5 7
#define relay6 8

#define sensor1 A0
#define sensor2 A1
#define sensor3 A2
#define sensor4 A3
#define sensor5 A6
#define sensor6 A7

#define flSwitch 9
#define flSwitchRelay 10

int sensor1val;
int sensor2val;
int sensor3val;
int sensor4val;
int sensor5val;
int sensor6val;

int moistTreshold = 600;

byte sensor1Char[] = {
  B11011,
  B10001,
  B11001,
  B01001,
  B11001,
  B00000,
  B00000,
  B00000
};

byte sensor2Char[] = {
  B11011,
  B10001,
  B11011,
  B01010,
  B11011,
  B00000,
  B00000,
  B00000
};

byte sensor3Char[] = {
  B11011,
  B10001,
  B11011,
  B01001,
  B11011,
  B00000,
  B00000,
  B00000
};

byte sensor4Char[] = {
  B11011,
  B10011,
  B11011,
  B01001,
  B11001,
  B00000,
  B00000,
  B00000
};

byte sensor5Char[] = {
  B11011,
  B10010,
  B11011,
  B01001,
  B11011,
  B00000,
  B00000,
  B00000
};

byte sensor6Char[] = {
  B11011,
  B10010,
  B11011,
  B01011,
  B11011,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  pinMode(sensor1 , INPUT);
  pinMode(sensor2 , INPUT);
  pinMode(sensor3 , INPUT);
  pinMode(sensor4 , INPUT);
  pinMode(sensor5 , INPUT);
  pinMode(sensor6 , INPUT);

  pinMode(relay1 , OUTPUT);
  pinMode(relay2 , OUTPUT);
  pinMode(relay3 , OUTPUT);
  pinMode(relay4 , OUTPUT);
  pinMode(relay5 , OUTPUT);
  pinMode(relay6 , OUTPUT);

  
  pinMode(flSwitch , INPUT);
  pinMode(flSwitchRelay , OUTPUT);

  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("System");
  lcd.setCursor(4,1);
  lcd.print("Starting....");

  lcd.createChar(0 , sensor1Char);
  lcd.createChar(1 , sensor2Char);
  lcd.createChar(2 , sensor3Char);
  lcd.createChar(3 , sensor4Char);
  lcd.createChar(4 , sensor5Char);
  lcd.createChar(5 , sensor6Char);
  lcd.home();
  
  delay(2000);
  lcd.clear();
}

void loop() {
  sensor1val = analogRead(sensor1);
  sensor2val = analogRead(sensor2);
  sensor3val = analogRead(sensor3);
  sensor4val = analogRead(sensor4);
  sensor5val = analogRead(sensor5);
  sensor6val = analogRead(sensor6);

  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(2,0);
  lcd.print(map(sensor1val , 0 , 1023 , 99 , 0));

  lcd.setCursor(6,0);
  lcd.write(byte(1));
  Serial.print("/S2 - ");
  lcd.setCursor(8,0);
  lcd.print(map(sensor2val , 0 , 1023 , 99 , 0));

  lcd.setCursor(12,0);
  lcd.write(byte(2));
  Serial.print("/S3 - ");
  lcd.setCursor(14,0);
  lcd.print(map(sensor3val , 0 , 1023 , 99 , 0));

  
  Serial.print("/S4 - ");
  lcd.setCursor(0,1);
  lcd.write(byte(3));
  lcd.setCursor(2,1);
  lcd.print(map(sensor4val , 0 , 1023 , 99 , 0));
  
  Serial.print("/S5 - ");
  lcd.setCursor(6,1);
  lcd.write(byte(4));
  lcd.setCursor(8,1);
  lcd.print(map(sensor5val , 0 , 1023 , 99 , 0));
  
  Serial.print("/S6 - ");
  lcd.setCursor(12,1);
  lcd.write(byte(5));
  lcd.setCursor(14,1);
  lcd.print(map(sensor6val , 0 , 1023 , 99 , 0));
  Serial.println("");

  if(sensor1val > moistTreshold){
    digitalWrite(relay1 , LOW);
  }else{
    digitalWrite(relay1 , HIGH);
  }

  if(sensor2val > moistTreshold){
    digitalWrite(relay2 , LOW);
  }else{
    digitalWrite(relay2 , HIGH);
  }

  if(sensor3val > moistTreshold){
    digitalWrite(relay3 , LOW);
  }else{
    
    digitalWrite(relay3 , HIGH);
  }

  if(sensor4val > moistTreshold){
    digitalWrite(relay4 , LOW);
  }else{
    digitalWrite(relay4 , HIGH);
  }

  if(sensor5val > moistTreshold){
    digitalWrite(relay5 , LOW);
  }else{
    digitalWrite(relay5 , HIGH);
  }

  if(sensor6val > moistTreshold){
    digitalWrite(relay6 , LOW);
  }else{
    digitalWrite(relay6 , HIGH);
  }

  if(digitalRead(flSwitch) == HIGH){
    digitalWrite(flSwitchRelay , HIGH);
  }else{
    digitalWrite(flSwitchRelay , LOW);
  }
  delay(1000);
}

float _map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
