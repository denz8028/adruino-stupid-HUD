#include <Wire.h>
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64> oled;

String str1 = "";
String str2 = "";
String str3 = "";
String newData = "";
String additionalStr = "";
unsigned long scrollTimer = 0;
int scrollPosition = 0;
const int scrollDelay = 150;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  oled.init();
  oled.clear();
  
  oled.home();
  oled.setScale(1);
  oled.print("Ожидание");
  oled.setCursor(0, 1);
  oled.print("данных...");
  oled.update();
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n') {
      processIncomingData();
      newData = "";
    } else {
      newData += c;
    }
  }
  
  if (millis() - scrollTimer > scrollDelay) {
    updateDisplay();
    scrollTimer = millis();
  }
}

void processIncomingData() {
  int firstSeparator = newData.indexOf('|');
  int secondSeparator = newData.indexOf('|', firstSeparator + 1);
  int thirdSeparator = newData.indexOf('|', secondSeparator + 1);

  if (firstSeparator != -1 && secondSeparator != -1 && thirdSeparator != -1) {
    String artist = newData.substring(0, firstSeparator);
    String album = newData.substring(firstSeparator + 1, secondSeparator);
    String title = newData.substring(secondSeparator + 1, thirdSeparator);
    String additional = newData.substring(thirdSeparator + 1);
    if (artist != str1 || album != str2 || title != str3 || additional != additionalStr) {
      str1 = artist;
      str2 = album;
      str3 = title;
      additionalStr = additional;
    }
  }
}

void updateDisplay() {
  oled.clear();
  
  oled.setScale(1);
  oled.setCursor(0, 0);
  String str12 = str1 + " - " + str2;
  if (str12.length() > 21) {
    oled.print(str12.substring(0, 21) + "...");
  } else {
    oled.print(str12);
  }
  
  oled.fastLineH(10, 0, 128);
  uint32_t tmr = millis();
  int val = 0;
  oled.setScale(1);
  oled.setCursor(0,7);
  oled.print(additionalStr);

  if (str3.length() > 10) {
    for(;;) {
      oled.setScale(2);
      oled.setCursor(val, 3);
      oled.print(str3);
      oled.update();
      val--;
      if (millis() - tmr > 5000) return;
    }
  } else {
    oled.setScale(2);
    oled.setCursor(0, 3);
    oled.print(str3);
  }
  
  oled.update();
}