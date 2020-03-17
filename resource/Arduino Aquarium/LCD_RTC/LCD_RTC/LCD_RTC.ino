#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ds3231.h>

LiquidCrystal_I2C lcd(0x27,16,2);

struct ts t; //déclaration variable t 
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
 
 /* t.hour=12; // données pour mettre à l'heure l'horloge
  t.min=22;
  t.sec=0;
  t.mday=18;
  t.mon=12;
  t.year=2016; 
 
  DS3231_set(t); // mise à l'heure de l'horloge */

lcd.init();                      // initialize the lcd 











}
 
void loop() {
  
   Wire.beginTransmission(0x68);
  
  
  // put your main code here, to run repeatedly:
  DS3231_get(&t);
  Serial.print("date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Heure : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
 
  delay(500);

  Wire.endTransmission();


Wire.beginTransmission(0x27);


 
  // Print a message to the LCD.
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(t.mday);
  lcd.print("/");
  lcd.print(t.mon);
  lcd.print("/");
  lcd.print(t.year);
  lcd.setCursor(0, 1);
  lcd.print(t.hour);
  lcd.print(":");
  lcd.print(t.min);
  lcd.print(":");
  lcd.print(t.sec);
  
  Wire.endTransmission();

delay(500);





  
}
