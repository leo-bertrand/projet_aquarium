#include <Wire.h>
#include "src/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "src/DS3231/DS3231.h"

const int pin_bouton_1 = 2;
const int pin_bouton_2 = 3;
const int pin_potentiometre = A0;

LiquidCrystal_I2C lcd(0x27,16,2); // déclaration de l'afficheur
DS3231 rtc(SDA, SCL); // déclaration de l'horloge
Time time; ; // déclaration d'un objet Time

int nourriture = 1; // nombre de poissons à nourrir
int temperature = 20; // température de l'eau idéale
int eclairage = 2; // éclairage -> 0: ON | 1: OFF | 2: Auto

void setup() {
  Serial.begin(9600); // initialiser le port série avec un débit de 9600 bps
  
  Wire.begin(); // initialiser les transmissions
  
  lcd.init(); // initialiser l'afficheur
  lcd.backlight(); // allumer le rétroéclairage de l'afficheur

  rtc.begin(); // initialiser l'horloge
  rtc.setDOW(MONDAY); // initialiser le jour de la semaine
  rtc.setTime(19, 26, 0); // initialiser l'heure sous la forme hh, mm, ss
  rtc.setDate(17, 3, 2020); // initialiser la date sous la forme jj, mm, aaaa

  pinMode(pin_bouton_1, INPUT_PULLUP); // initialiser le bouton sur le pin 2 avec la résistance de rappel interne activée
  pinMode(pin_bouton_2, INPUT_PULLUP); // initialiser le bouton sur le pin 2 avec la résistance de rappel interne activée
}
 
void loop() {
  time = rtc.getTime(); // récupération de l'heure de l'horloge
  
  // afficher infos  
  
  if (digitalRead(pin_bouton_2) == LOW) {
    choisir_action_menu_principal();
  }

  delay(100);  
}

void choisir_action_menu_principal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU PRINCIPAL");
  delay(1500);

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (digitalRead(pin_bouton_1) == LOW) {
      return;
    }

    int valeur_potentiometre = analogRead(pin_potentiometre);

    if (valeur_potentiometre < 333) {
      lcd.print("Reg. nourriture");
    }
    else if (valeur_potentiometre < 666) {
      lcd.print("Reg. temperature");
    }
    else {
      lcd.print("Reg. eclairage");
    }
    
    if (digitalRead(pin_bouton_2) == LOW) {
      if (valeur_potentiometre < 333) {
        choisir_nourriture();
      }
      else if (valeur_potentiometre < 666) {
        choisir_temperature();
      }
      else {
        choisir_eclairage();
      }
    }
    delay(100);
  }
}

void choisir_nourriture() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU NOURRITURE");
  delay(1500);

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (digitalRead(pin_bouton_1) == LOW) {
      return;
    }

    int valeur_potentiometre = analogRead(pin_potentiometre);
    
    if (valeur_potentiometre < 333) {
      lcd.print("1 POISSON");
    }
    else if (valeur_potentiometre < 666) {
      lcd.print("2 POISSONS");
    }
    else {
      lcd.print("3 POISSONS");
    }
    
    if (digitalRead(pin_bouton_2) == LOW) {
      if (valeur_potentiometre < 333) {
        nourriture = 1;
        return;
      }
      else if (valeur_potentiometre < 666) {
        nourriture = 2;
        return;
      }
      else {
        nourriture = 3;
        return;
      }
    }
    delay(100);
  }
}

void choisir_temperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU TEMPERATURE");
  delay(1500);

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (digitalRead(pin_bouton_1) == LOW) {
      return;
    }

    int valeur_potentiometre = analogRead(pin_potentiometre);

    if (valeur_potentiometre < 140) {
      lcd.print("18");
    }
    else if (valeur_potentiometre < 280) {
      lcd.print("19");
    }
    else if (valeur_potentiometre < 420) {
      lcd.print("20");
    }
    else if (valeur_potentiometre < 560) {
      lcd.print("21");
    }
    else if (valeur_potentiometre < 700) {
      lcd.print("22");
    }
    else if (valeur_potentiometre < 840) {
      lcd.print("23");
    }
    else {
      lcd.print("24");
    }
    
    if (valeur_potentiometre == LOW) {  
      if (valeur_potentiometre < 140) {
        temperature = 18;
        return;
      }
      else if (valeur_potentiometre < 280) {
        temperature = 19;
        return;
      }
      else if (valeur_potentiometre < 420) {
        temperature = 20;
        return;
      }
      else if (valeur_potentiometre < 560) {
        temperature = 21;
        return;
      }
      else if (valeur_potentiometre < 700) {
        temperature = 22;
        return;
      }
      else if (valeur_potentiometre < 840) {
        temperature = 23;
        return;
      }
      else {
        temperature = 24;
        return;
      }
    }
    delay(100);
  }
}

void choisir_eclairage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU ECLAIRAGE");
  delay(1500);

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (digitalRead(pin_bouton_1) == LOW) {
      return;
    }

    int valeur_potentiometre = analogRead(pin_potentiometre);

    if (valeur_potentiometre < 333) {
      lcd.print("ON");
    }
    else if (valeur_potentiometre < 666) {
      lcd.print("OFF");
    }
    else {
      lcd.print("Auto");
    }

    if (digitalRead(pin_bouton_2) == LOW) {
      if (valeur_potentiometre < 333) {
        eclairage = 0;
        return;
      }
      else if (valeur_potentiometre < 666) {
        eclairage = 1;
        return;
      }
      else {
        eclairage = 2;
        return;
      }
    }
    delay(100);
  }
}