#include <SD.h>
#include <SPI.h>

const int chipSelect = 10;

void initSD(){
  if (!SD.begin(chipSelect)) {
    return;
  }
}

File createFile(String fileName){
  File dataFile = SD.open(fileName + ".txt", FILE_WRITE);
  return dataFile;
}

void writeToFile(File file , String sentence){
  if (file){
    file.println(sentence);
    file.close();
  }
}
