#include <Wire.h>
#include "src/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "src/DS3231/DS3231.h"

LiquidCrystal_I2C lcd(0x27,16,2); // déclaration de l'afficheur
DS3231 rtc(SDA, SCL); // déclaration de l'horloge
Time time; // déclaration d'un objet Time

void setup() {
  Serial.begin(9600); // initialiser le port série avec un débit de 9600 bps
  Wire.begin(); // initialiser les transmissions
  lcd.init(); // initialiser l'afficheur 
  rtc.begin(); // initialiser l'horloge

  rtc.setDOW(); // initialiser le jour de la semaine
  rtc.setTime(19, 26, 0); // initialiser l'heure sous la forme hh, mm, ss
  rtc.setDate(17, 3, 2020); // initialiser la date sous la forme jj, mm, aaaa
}
 
void loop() {
  time = rtc.getTime(); // récupération de l'heure de l'horloge

  Wire.beginTransmission(0x27);
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(time.dow);
  lcd.print(time.sec);
  lcd.print(time.sec);
  lcd.print(time.sec);
  lcd.print(time.sec);
  Wire.endTransmission();
  delay(100);  
}
