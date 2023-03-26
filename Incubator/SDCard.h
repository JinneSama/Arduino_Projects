#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <RTClib.h>

RTC_DS3231 rtc;

const int chipSelect = 10;
String fileName = "datalog";
File dataFile;

void initSD(){
  if (!SD.begin(chipSelect)) {
    lcd.setCursor(2,0);
    lcd.print("Initializing SD!");
    while (1);
  }
}

void writeToFile(String sentence){
  dataFile = SD.open(fileName + ".txt", FILE_WRITE);
  
  if (dataFile){
    dataFile.println(sentence);
    dataFile.close();
  }
}

void writeHeaderTime(String type){
  dataFile = SD.open(fileName + ".txt", FILE_WRITE);
  DateTime now = rtc.now();
  if (dataFile){
    dataFile.println("*******************************************************************************************");
    dataFile.println("");
    dataFile.print(type);
    dataFile.print(" - ");
    dataFile.print(now.year() , DEC);
    dataFile.print("/");
    dataFile.print(now.month() , DEC);
    dataFile.print("/");
    dataFile.print(now.day() , DEC);
    dataFile.print("  ");
    dataFile.print(now.hour() , DEC);
    dataFile.print(":");
    dataFile.print(now.minute() , DEC);
    dataFile.println("");
    dataFile.close();
  }
}

void writeDataLog(float t , float h){
  dataFile = SD.open(fileName + ".txt", FILE_WRITE);
  DateTime now = rtc.now();
  if (dataFile){
    dataFile.print(now.hour() , DEC);
    dataFile.print(":");
    dataFile.print(now.minute() , DEC);
    dataFile.print(":");
    dataFile.print(now.second() , DEC);
    dataFile.print("          ");
    dataFile.print(t);
    dataFile.print("C         ");
    dataFile.print(h);
    dataFile.print("%");
    dataFile.println("");
    dataFile.close();
  }
}

void clearLCD(boolean l1, boolean l2, boolean l3, boolean l4){
  if (l1){
      lcd.setCursor(0, 0);
      lcd.print("                    ");
  }
  
  if (l2){
      lcd.setCursor(0, 1);
      lcd.print("                    ");
  }

  if (l3){
      lcd.setCursor(0, 2);
      lcd.print("                    ");
  }

  if (l4){
      lcd.setCursor(0, 3);
      lcd.print("                    ");
  }
}
