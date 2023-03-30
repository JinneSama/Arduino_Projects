#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(3, 4);


LiquidCrystal_I2C lcd1(0x27, 20, 4);
long timeStart = 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void initFingerprint()  {
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

bool registerFingerprint(uint8_t id){
  restart_finger:
  int p = -1;
  timeStart = millis();
  while (p != FINGERPRINT_OK) {
    if(millis() - timeStart > 10000){
      return false;
    }
    p = finger.getImage();
  }
  p = finger.image2Tz(1);
  
  lcd1.clear();
  lcd1.setCursor(2,0);
  lcd1.print("REMOVE FINGER");
  delay(2000);
  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  
  lcd1.clear();
  lcd1.setCursor(3,0);
  lcd1.print("PLACE SAME");
  lcd1.setCursor(2,1);
  lcd1.print("FINGER AGAIN"); 
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  p = finger.image2Tz(2);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd1.clear();
    lcd1.setCursor(1,0);
    lcd1.print("FINGER MATCHED");
    lcd1.setCursor(5,1);
    lcd1.print("SAVING"); 
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd1.clear();
    lcd1.setCursor(1,0);
    lcd1.print("COMMUNICATION");
    lcd1.setCursor(5,1);
    lcd1.print("ERROR"); 
    goto restart_finger;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("FINGEPRINT");
    lcd1.setCursor(1,1);
    lcd1.print("DID NOT MATCH"); 
    goto restart_finger;
  } else {
    lcd1.clear();
    lcd1.setCursor(1,0);
    lcd1.print("UNKNOWN ERROR");
    lcd1.setCursor(3,1);
    lcd1.print("TRY AGAIN"); 
    goto restart_finger;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd1.clear();
    lcd1.setCursor(4,0);
    lcd1.print("SUCCESS");
    lcd1.setCursor(1,1);
    lcd1.print("FINGER STORED"); 
    delay(2000);
    return true;
  } else {
    lcd1.clear();
    lcd1.setCursor(1,0);
    lcd1.print("UNKNOWN ERROR");
    lcd1.setCursor(3,1);
    lcd1.print("TRY AGAIN"); 
    goto restart_finger;
  }   
}

int getFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}
