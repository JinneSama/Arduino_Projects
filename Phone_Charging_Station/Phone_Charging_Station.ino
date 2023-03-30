#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "fingerprint.h"

#define COIN_INTERRUPT 2

#define CHARGE_RELAY1 5
#define CHARGE_RELAY2 6
#define CHARGE_RELAY3 7

#define SECURITY_IR1 A0
#define SECURITY_IR2 A1
#define SECURITY_IR3 A2

#define STORAGE_BTN1 8
#define STORAGE_BTN2 9
#define STORAGE_BTN3 10

#define SERVO1 11
#define SERVO2 12
#define SERVO3 13

#define SERVO_CLOSE 60
#define SERVO_OPEN 0

int BUZZPIN = A3;

long COSTPERHOUR = 12;
bool intFired = false; 
bool isPaying = false;
bool btn1Pressed = false;
bool btn2Pressed = false;
bool btn3Pressed = false;

bool storage1State = false;
bool storage2State = false;
bool storage3State = false;

bool alarm1State = false;
bool alarm2State = false;
bool alarm3State = false; 

bool waitForPhone1 = false;
bool waitForPhone2 = false;
bool waitForPhone3 = false;

bool charging1 = false;
bool charging2 = false;
bool charging3 = false;

bool paying1 = false;
bool paying2 = false;
bool paying3 = false;

bool insertPhoneUI = false;

long timeStart1 = 0;
long timeStart2 = 0;
long timeStart3 = 0; 

unsigned int cost1 = 0; 
unsigned int cost2 = 0;
unsigned int cost3 = 0;

int amountPaid1 = 0;
int amountPaid2 = 0;
int amountPaid3 = 0;

long lastTimeStamp1 = 1;
long lastTimeStamp2 = 1;
long lastTimeStamp3 = 1;

int startLoopTime = 0;
bool startUp = true;

long timeToPay1 = 0;
long timeToPay2 = 0;
long timeToPay3 = 0;

Servo servo1;
Servo servo2;
Servo servo3;

bool verifying1 = false;
bool verifying2 = false;
bool verifying3 = false;

void defaultAll(){
  
}

void Payment1(){
  digitalWrite(CHARGE_RELAY1 , LOW);
  if(verifying1){
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("VERIFY YOUR");
    lcd1.setCursor(2,1);
    lcd1.print("FINGERPRINT");
    while(1){
      if((millis() - timeToPay1) > 10000){
        clearScreen(1);
        clearScreen(2);
        clearScreen(3);
        Spacer();
        paying1 = false;
        insertPhoneUI = false;
        charging1 = true;
        digitalWrite(CHARGE_RELAY1 , HIGH);
        verifying1 = false;
        return;
      }
      
       if(getFingerprint() == 1){
        verifying1 = false;
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("FINGERPRINT VERIFIED");
        delay(2000);
        lcd1.clear();
        lcd1.setCursor(4,0);
        lcd1.print("BOX1 PAYMENT");
        
        lcd1.setCursor(1,2);
        lcd1.print("COST:         P");
        lcd1.setCursor(16,2);
        lcd1.print(cost1);
        
        lcd1.setCursor(1,3);
        lcd1.print("PAYMENT:      P");
        lcd1.setCursor(16,3);                                                                                                                                                                                                                                                                                                                                           
        lcd1.print(amountPaid1);
        timeToPay1 = millis();
        break;
      }
    }
  }
  
  if((millis() - timeToPay1) > 10000){
    clearScreen(1);
    clearScreen(2);
    clearScreen(3);
    Spacer();
    paying1 = false;
    insertPhoneUI = false;
    charging1 = true;
    digitalWrite(CHARGE_RELAY1 , HIGH);
  }
  
  if(intFired){
    amountPaid1 += 1;
    timeToPay1 = millis();
    
    lcd1.clear();
    lcd1.setCursor(4,0);
    lcd1.print("BOX1 PAYMENT");
    
    lcd1.setCursor(1,2);
    lcd1.print("COST:         P");
    lcd1.setCursor(16,2);
    lcd1.print(cost1);
    
    lcd1.setCursor(1,3);
    lcd1.print("PAYMENT:      P");
    lcd1.setCursor(16,3);
    lcd1.print(amountPaid1);
    
    if(amountPaid1 >= cost1){
      lcd1.clear();
      lcd1.setCursor(5,0);
      lcd1.print("COST PAID");
      lcd1.setCursor(5,1);
      lcd1.print("THANK YOU");
      
      delay(2000);
      
      storage1State = false;
      servo1.write(SERVO_CLOSE);
      paying1 = false;

      cost1 = 0;
      amountPaid1 = 0;
      lastTimeStamp1 = 0;
      delay(200);
      deleteFingerprint(1);
      
      digitalWrite(CHARGE_RELAY1 , LOW);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("SYSTEM UNLOCKED");

      delay(3000);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("YOU MAY NOW GET");
      lcd1.setCursor(4,1);
      lcd1.print("YOUR PHONE");
      
      delay(3000);
      clearScreen(1);
      clearScreen(2);
      clearScreen(3);
      Spacer();
      insertPhoneUI = false;
    }
    intFired = false;
  }
}

void Payment2(){
  digitalWrite(CHARGE_RELAY2 , LOW);
  if(verifying2){
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("VERIFY YOUR");
    lcd1.setCursor(2,1);
    lcd1.print("FINGERPRINT");
    while(1){
      if((millis() - timeToPay2) > 10000){
        clearScreen(1);
        clearScreen(2);
        clearScreen(3);
        Spacer();
        paying2 = false;
        insertPhoneUI = false;
        charging2 = true;
        digitalWrite(CHARGE_RELAY2 , HIGH);
        verifying2 = false;
        return;
      }
      
       if(getFingerprint() == 2){
        verifying2 = false;lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("FINGERPRINT VERIFIED");
        delay(2000);
        lcd1.clear();
        lcd1.setCursor(4,0);
        lcd1.print("BOX2 PAYMENT");
        
        lcd1.setCursor(1,2);
        lcd1.print("COST:         P");
        lcd1.setCursor(16,2);
        lcd1.print(cost2);
        
        lcd1.setCursor(1,3);
        lcd1.print("PAYMENT:      P");
        lcd1.setCursor(16,3);                                                                                                                                                                                                                                                                                                                                           
        lcd1.print(amountPaid2);
        timeToPay2 = millis();
        break;
      }
    }
  }
  
  if((millis() - timeToPay2) > 10000){
    clearScreen(1);
    clearScreen(2);
    clearScreen(3);
    Spacer();
    paying2 = false;
    insertPhoneUI = false;
    charging2 = true;
    digitalWrite(CHARGE_RELAY2 , HIGH);
  }
    
  if(intFired){
    amountPaid2 += 1;
    timeToPay2 = millis();
    lcd1.clear();
    lcd1.setCursor(4,0);
    lcd1.print("BOX2 PAYMENT");
    
    lcd1.setCursor(1,2);
    lcd1.print("COST:         P");
    lcd1.setCursor(16,2);
    lcd1.print(cost2);
    
    lcd1.setCursor(1,3);
    lcd1.print("PAYMENT:      P");
    lcd1.setCursor(16,3);                                                                                                                                                                                                                                                                                                                                           
    lcd1.print(amountPaid2);
    
    if(amountPaid2 >= cost2){
      lcd1.clear();
      lcd1.setCursor(5,0);
      lcd1.print("COST PAID");
      lcd1.setCursor(5,1);
      lcd1.print("THANK YOU");
      
      delay(2000);
      
      storage2State = false;
      servo2.write(SERVO_CLOSE);
      paying2 = false;

      cost2 = 0;
      amountPaid2 = 0;
      lastTimeStamp2 = 0;
      delay(200);
      deleteFingerprint(2);
      
      digitalWrite(CHARGE_RELAY2 , LOW);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("SYSTEM UNLOCKED");

      delay(3000);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("YOU MAY NOW GET");
      lcd1.setCursor(4,1);
      lcd1.print("YOUR PHONE");
      
      delay(3000);
      
      clearScreen(2);
      clearScreen(1);
      clearScreen(3);
      Spacer();
      insertPhoneUI = false;
    }
    intFired = false;
  }
  
}

void Payment3(){
  digitalWrite(CHARGE_RELAY3 , LOW);
  if(verifying3){
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("VERIFY YOUR");
    lcd1.setCursor(2,1);
    lcd1.print("FINGERPRINT");
    while(1){
      if((millis() - timeToPay3) > 10000){
        clearScreen(1);
        clearScreen(2);
        clearScreen(3);
        Spacer();
        paying3 = false;
        insertPhoneUI = false;
        charging3 = true;
        digitalWrite(CHARGE_RELAY3 , HIGH);
        verifying3 = false;
        return;
      }
      
       if(getFingerprint() == 3){
        verifying3 = false;
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("FINGERPRINT VERIFIED");
        delay(2000);
        lcd1.clear();
        lcd1.setCursor(4,0);
        lcd1.print("BOX3 PAYMENT");
        
        lcd1.setCursor(1,2);
        lcd1.print("COST:         P");
        lcd1.setCursor(16,2);
        lcd1.print(cost3);
        
        lcd1.setCursor(1,3);
        lcd1.print("PAYMENT:      P");
        lcd1.setCursor(16,3);                                                                                                                                                                                                                                                                                                                                           
        lcd1.print(amountPaid3);
        timeToPay3 = millis();
        break;
      }
    }
  }
  
  if((millis() - timeToPay3) > 10000){
    clearScreen(1);
    clearScreen(2);
    clearScreen(3);
    Spacer();
    paying3 = false;
    insertPhoneUI = false;
    charging3 = true;
    digitalWrite(CHARGE_RELAY3 , HIGH);
  }
    
  if(intFired){
    amountPaid3 += 1;
    timeToPay3 = millis();
    lcd1.clear();
    lcd1.setCursor(4,0);
    lcd1.print("BOX3 PAYMENT");
    
    lcd1.setCursor(1,2);
    lcd1.print("COST:         P");
    lcd1.setCursor(16,2);
    lcd1.print(cost3);
    
    lcd1.setCursor(1,3);
    lcd1.print("PAYMENT:      P");
    lcd1.setCursor(16,3);                                                                                                                                                                                                                                                                                                                                           
    lcd1.print(amountPaid3);
    
    if(amountPaid3 >= cost3){
      lcd1.clear();
      lcd1.setCursor(5,0);
      lcd1.print("COST PAID");
      lcd1.setCursor(5,1);
      lcd1.print("THANK YOU");
      
      delay(2000);
      
      storage3State = false;
      servo3.write(SERVO_CLOSE);
      paying3 = false;

      cost3 = 0;
      amountPaid3 = 0;
      lastTimeStamp3 = 0;
      delay(200);
      deleteFingerprint(3);
      
      digitalWrite(CHARGE_RELAY3 , LOW);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("SYSTEM UNLOCKED");

      delay(3000);
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("YOU MAY NOW GET");
      lcd1.setCursor(4,1);
      lcd1.print("YOUR PHONE");
      
      delay(3000);
      
      clearScreen(2);
      clearScreen(1);
      clearScreen(3);
      Spacer();
      insertPhoneUI = false;
    }
    intFired = false;
  }
  
}



void ChargeStatus1(){
  if(insertPhoneUI){
    return;
  }
  long currentTimeDiff = (millis() - timeStart1) / 1000;
  cost1 = (currentTimeDiff * COSTPERHOUR) / 3600; 
  if(cost1  < 1){
    cost1 = 1;
  }
  
  if((millis() - lastTimeStamp1) > 2000){
    lastTimeStamp1 = millis();
    
    lcd1.setCursor(1,0);
    lcd1.print("BOX1");
    
    lcd1.setCursor(0,1);
    lcd1.print("IN USE");
    
    lcd1.setCursor(0,2);
    lcd1.print("      ");
    lcd1.setCursor(0,2);
    
    int _hour = currentTimeDiff / 3600;
    int _minute = currentTimeDiff / 60;
    if(_minute >= 60){
      _minute = _minute % 60;
    }
    String h = "";
    String m = "";
    if(_hour < 10){
      h = "0" + String(_hour);
    }else{
      h = String(_hour);
    }

    if(_minute < 10){
      m = "0" + String(_minute);
    }else{
      m = String(_minute);
    }
    String tm = h + ":" + m;
    
    lcd1.print(tm);
    lcd1.setCursor(0,3);
    lcd1.print("      ");
    lcd1.setCursor(0,3);
    lcd1.print("CT");
    
    if(cost1 < 10){
      lcd1.setCursor(5,3);
    }else if(cost1 >= 10 && cost1 < 100){
      lcd1.setCursor(4,3);
    }else{
      lcd1.setCursor(3,3);
    }
    lcd1.print(cost1);
  }

  if(!digitalRead(STORAGE_BTN1)){
    charging1 = false;
    paying1 = true;
    delay(200);

    lcd1.clear();
    insertPhoneUI = true;
    timeToPay1 = millis();
    verifying1 = true;
  }
}

void ChargeStatus2(){
  if(insertPhoneUI){
    return;
  }
  long currentTimeDiff = (millis() - timeStart2) / 1000;
  cost2 = (currentTimeDiff * COSTPERHOUR) / 3600;
  
  if(cost2  < 1){
    cost2 = 1;
  }
  
  if((millis() - lastTimeStamp2) > 1000){
    lastTimeStamp2 = millis();
    
    lcd1.setCursor(8,0);
    lcd1.print("BOX2");
    
    lcd1.setCursor(7,1);
    lcd1.print("IN USE");
    
    lcd1.setCursor(7,2);
    lcd1.print("      ");
    lcd1.setCursor(7,2);
    int _hour = currentTimeDiff / 3600;
    int _minute = currentTimeDiff / 60;
    if(_minute >= 60){
      _minute = _minute % 60;
    }
    String h = "";
    String m = "";
    if(_hour < 10){
      h = "0" + String(_hour);
    }else{
      h = String(_hour);
    }

    if(_minute < 10){
      m = "0" + String(_minute);
    }else{
      m = String(_minute);
    }
    String tm = h + ":" + m;
    
    
    lcd1.print(tm);
    lcd1.setCursor(7,3);
    lcd1.print("      ");
    lcd1.setCursor(7,3);
    lcd1.print("CT");
    
    if(cost2 < 10){
      lcd1.setCursor(12,3);
    }else if(cost2 >= 10 && cost2 < 100){
      lcd1.setCursor(11,3);
    }else{
      lcd1.setCursor(10,3);
    }
    lcd1.print(cost2);
  }

  if(!digitalRead(STORAGE_BTN2)){
    charging2 = false;
    paying2 = true;
    delay(200);

    lcd1.clear();
    insertPhoneUI = true;
    timeToPay2 = millis();
    verifying2 = true;
  }
  
}

void ChargeStatus3(){
  if(insertPhoneUI){
    return;
  }
  long currentTimeDiff = (millis() - timeStart3) / 1000;
  cost3 = (currentTimeDiff * COSTPERHOUR) / 3600;

  if(cost3  < 1){
    cost3 = 1;
  }
  
  if((millis() - lastTimeStamp3) > 2000){
    lastTimeStamp3 = millis();
    
    lcd1.setCursor(15,0);
    lcd1.print("BOX3");
    
    lcd1.setCursor(14,1);
    lcd1.print("IN USE");
    
    lcd1.setCursor(14,2);
    lcd1.print("      ");
    lcd1.setCursor(14,2);

    int _hour = currentTimeDiff / 3600;
    int _minute = currentTimeDiff / 60;
    if(_minute >= 60){
      _minute = _minute % 60;
    }
    String h = "";
    String m = "";
    if(_hour < 10){
      h = "0" + String(_hour);
    }else{
      h = String(_hour);
    }

    if(_minute < 10){
      m = "0" + String(_minute);
    }else{
      m = String(_minute);
    }
    String tm = h + ":" + m;
    
    lcd1.print(tm);
    lcd1.setCursor(14,3);
    lcd1.print("      ");
    lcd1.setCursor(14,3);
    lcd1.print("CT");
    
    if(cost3 < 10){
      lcd1.setCursor(19,3);
    }else if(cost3 >= 10 && cost3 < 100){
      lcd1.setCursor(18,3);
    }else{
      lcd1.setCursor(17,3);
    }
    lcd1.print(cost3);
  }

  if(!digitalRead(STORAGE_BTN3)){
    charging3 = false;
    paying3 = true;
    delay(200);

    lcd1.clear();
    insertPhoneUI = true;
    timeToPay3 = millis();
    verifying3 = true;
  }
  
}

void waitingForPhone1(){
  if(!digitalRead(SECURITY_IR1)){
    delay(200);
    waitForPhone1 = false;
    storage1State = true;
    servo1.write(SERVO_OPEN);
    
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("SYSTEM IS LOCKED");

    delay(3000);
    
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("REGISTER YOUR");
    lcd1.setCursor(4,1);
    lcd1.print("FINGERPRINT");
    uint8_t id = 1;
    
    bool registered = false;
    if(registerFingerprint(id)){
      registered = true;
    }else{
      waitForPhone1 = false;
      storage1State = false;
      insertPhoneUI = false;
      servo1.write(SERVO_CLOSE);
      
      clearScreen(1);
      clearScreen(3);
      clearScreen(2);
      Spacer();
    }
    if(registered){
      lcd1.clear();
      lcd1.setCursor(3,0);
      lcd1.print("REGISTER DONE");
      lcd1.setCursor(2,1);
      lcd1.print("TURNING ON POWER"); 
      delay(2000);
  
      clearScreen(2);
      clearScreen(3);
      clearScreen(1);
      Spacer();
      
      digitalWrite(CHARGE_RELAY1 , HIGH);
      charging1 = true; 
      insertPhoneUI = false;
      timeStart1 = millis();
    }
  }
}

void waitingForPhone2(){
  if(!digitalRead(SECURITY_IR2)){
    delay(200);
    waitForPhone2 = false;
    storage2State = true;
    servo2.write(SERVO_OPEN);
    
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("SYSTEM IS LOCKED");

    delay(3000);
    
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("REGISTER YOUR");
    lcd1.setCursor(4,1);
    lcd1.print("FINGERPRINT");
    uint8_t id = 2;
    bool registered = false;
    if(registerFingerprint(id)){
      registered = true;
    }else{
      waitForPhone2 = false;
      storage2State = false;
      insertPhoneUI = false;
      servo2.write(SERVO_CLOSE);
      
      clearScreen(1);
      clearScreen(3);
      clearScreen(2);
      Spacer();
    }
    if(registered){
      lcd1.clear();
      lcd1.setCursor(3,0);
      lcd1.print("REGISTER DONE");
      lcd1.setCursor(2,1);
      lcd1.print("TURNING ON POWER"); 
      delay(2000);
  
      clearScreen(1);
      clearScreen(3);
      clearScreen(2);
      Spacer();
      
      digitalWrite(CHARGE_RELAY2 , HIGH);
      charging2 = true; 
      insertPhoneUI = false;
      timeStart2 = millis();
    }
  }
  
}

void waitingForPhone3(){
  if(!digitalRead(SECURITY_IR3)){
    delay(200);
    
    timeStart3 = millis();
    waitForPhone3 = false;
    storage3State = true;
    servo3.write(SERVO_OPEN);
    
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("SYSTEM IS LOCKED");

    delay(3000);
    
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("REGISTER YOUR");
    lcd1.setCursor(4,1);
    lcd1.print("FINGERPRINT");
    uint8_t id = 3;
    bool registered = false;
    if(registerFingerprint(id)){
      registered = true;
    }else{
      waitForPhone3 = false;
      storage3State = false;
      insertPhoneUI = false;
      servo3.write(SERVO_CLOSE);
      
      clearScreen(1);
      clearScreen(3);
      clearScreen(2);
      Spacer();
    }
    if(registered){
      lcd1.clear();
      lcd1.setCursor(3,0);
      lcd1.print("REGISTER DONE");
      lcd1.setCursor(2,1);
      lcd1.print("TURNING ON POWER"); 
      delay(2000);
  
      clearScreen(1);
      clearScreen(3);
      clearScreen(2);
      Spacer();
      
      digitalWrite(CHARGE_RELAY3 , HIGH);
      charging3 = true; 
      insertPhoneUI = false;
      timeStart3 = millis();
    }
  }
  
}
void checkStorage1State(){
  if(!storage1State){
    lcd1.clear();
    lcd1.setCursor(2,0);    
    lcd1.print("Plug-in your Phone");


    lcd1.setCursor(3,1);
    lcd1.print("Inside Box #1");
    waitForPhone1 = true;
    insertPhoneUI = true;
  }
}

void checkStorage2State(){
  if(!storage2State){
    lcd1.clear();
    lcd1.setCursor(2,0); 
    lcd1.print("Plug-in your Phone");


    lcd1.setCursor(3,1);
    lcd1.print("Inside Box #2");
    waitForPhone2 = true;
    insertPhoneUI = true;
  }
}

void checkStorage3State(){
  if(!storage3State){
    lcd1.clear();
    lcd1.setCursor(2,0);
    lcd1.print("Plug-in your Phone");

    lcd1.setCursor(3,1);
    lcd1.print("Inside Box #3");
    waitForPhone3 = true;
    insertPhoneUI = true;
  }
}

void checkAlarm(){
  if(charging1){
    if(digitalRead(SECURITY_IR1)){
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("BOX1 FORCEFULLY");
  
      lcd1.setCursor(6,1);
      lcd1.print("OPENED!");
      
      lcd1.setCursor(2,3);
      lcd1.print("ALARM!! ALARM!!");
      
      digitalWrite(BUZZPIN , HIGH);
      delay(5000);

      digitalWrite(BUZZPIN , LOW);

      clearScreen(2);
      clearScreen(3);
      Spacer();
      clearScreen(1);
    }
  }

  if(charging2){
    if(digitalRead(SECURITY_IR2)){
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("BOX2 FORCEFULLY");
  
      lcd1.setCursor(6,1);
      lcd1.print("OPENED!");
      
      lcd1.setCursor(2,3);
      lcd1.print("ALARM!! ALARM!!");
      
      digitalWrite(BUZZPIN , HIGH);
      delay(5000);

      
      digitalWrite(BUZZPIN , LOW);

      clearScreen(1);
      clearScreen(3);
      Spacer();
      clearScreen(2);
    }
  }

  if(charging3){
    if(digitalRead(SECURITY_IR3)){
      lcd1.clear();
      lcd1.setCursor(2,0);
      lcd1.print("BOX3 FORCEFULLY");
  
      lcd1.setCursor(6,1);
      lcd1.print("OPENED!");
      
      lcd1.setCursor(2,3);
      lcd1.print("ALARM!! ALARM!!");
      
      digitalWrite(BUZZPIN , HIGH);
      delay(5000);

      
      digitalWrite(BUZZPIN , LOW);

      clearScreen(1);
      clearScreen(2);
      Spacer();
      clearScreen(3);
    }
  }
}

void lcdPaying(){
  
}

void setup() {
  lcd1.init();
  lcd1.backlight();
  
  pinMode(CHARGE_RELAY1 , OUTPUT);
  pinMode(CHARGE_RELAY2 , OUTPUT);
  pinMode(CHARGE_RELAY3 , OUTPUT);
  pinMode(BUZZPIN , OUTPUT);
  
  pinMode(STORAGE_BTN1 , INPUT_PULLUP);
  pinMode(STORAGE_BTN2 , INPUT_PULLUP);
  pinMode(STORAGE_BTN3 , INPUT_PULLUP);
  
  pinMode(SECURITY_IR1 , INPUT);
  pinMode(SECURITY_IR2 , INPUT);
  pinMode(SECURITY_IR3 , INPUT);
  
  pinMode(COIN_INTERRUPT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(COIN_INTERRUPT), coinInterrupt, FALLING);

  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo3.attach(SERVO3);

  servo1.write(SERVO_CLOSE);
  servo2.write(SERVO_CLOSE);
  servo3.write(SERVO_CLOSE);

  digitalWrite(CHARGE_RELAY1 , LOW);
  digitalWrite(CHARGE_RELAY2 , LOW);
  digitalWrite(CHARGE_RELAY3 , LOW);
 
  Spacer();
  
  initFingerprint();
}

void coinInterrupt() {
  if(paying1 || paying2 || paying3){
    intFired = true;
  }
}

void Spacer(){
  lcd1.setCursor(1,0);
  lcd1.print("BOX1");
  lcd1.setCursor(1,2);
  lcd1.print("AVLB");
  lcd1.setCursor(0,3);
  lcd1.print("TO USE");

  lcd1.setCursor(8,0);
  lcd1.print("BOX2");
  lcd1.setCursor(8,2);
  lcd1.print("AVLB");
  lcd1.setCursor(7,3);
  lcd1.print("TO USE");

  lcd1.setCursor(15,0);
  lcd1.print("BOX3");
  lcd1.setCursor(15,2);
  lcd1.print("AVLB");
  lcd1.setCursor(14,3);
  lcd1.print("TO USE");
  
  for(int i=0;i < 4; i++){
    lcd1.setCursor(6,i);
    lcd1.print("|");

    lcd1.setCursor(13,i);
    lcd1.print("|");
  }
}

void clearScreen(int index){
  if(index == 1){
    for(int x = 0; x < 4; x++){
      for(int y=0 ; y < 6; y++){
        lcd1.setCursor(y,x);
        lcd1.print(" ");
      }
    }
  }else if(index == 2){
    for(int x = 0; x < 4; x++){
      for(int y=7 ; y < 13; y++){
        lcd1.setCursor(y,x);
        lcd1.print(" ");
      }
    }
  }else if(index == 3){
    for(int x = 0; x < 4; x++){
      for(int y=14 ; y < 20; y++){
        lcd1.setCursor(y,x);
        lcd1.print(" ");
      }
    }
  }
}

void loop() {
  checkAlarm();
  if(paying1){
    Payment1();
  }
  if(paying2){
    Payment2();
  }
  if(paying3){
    Payment3();
  }
  if(charging1){
    ChargeStatus1();
  }
  if(charging2){
    ChargeStatus2();
  }
  if(charging3){
    ChargeStatus3();
  }
  
  if(waitForPhone1){
    waitingForPhone1();
  }
  if(waitForPhone2){
    waitingForPhone2();
  }
  if(waitForPhone3){
    waitingForPhone3();
  }
  
  if(!digitalRead(STORAGE_BTN1) && !btn1Pressed && !insertPhoneUI){
    btn1Pressed = true;
    delay(200);
    checkStorage1State();
  }else if(digitalRead(STORAGE_BTN1) && btn1Pressed){
    btn1Pressed = false;
    delay(200);
  }
  
  if(!digitalRead(STORAGE_BTN2) && !btn2Pressed && !insertPhoneUI){
    btn2Pressed = true;
    delay(200);
    checkStorage2State();
  }else if(digitalRead(STORAGE_BTN2)  && btn2Pressed){
    btn2Pressed = false;
    delay(200);
  }
  
  if(!digitalRead(STORAGE_BTN3) && !btn3Pressed && !insertPhoneUI){
    btn3Pressed = true;
    delay(200);
    checkStorage3State();
  }else if(digitalRead(STORAGE_BTN3) && btn3Pressed){
    btn3Pressed = false;
    delay(200);
  }

}
