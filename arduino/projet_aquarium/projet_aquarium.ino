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
  ecouter_commandes();

  time = rtc.getTime(); // récupération de l'heure de l'horloge
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(time.date);
  lcd.print("/");
  lcd.print(time.mon);
  lcd.print("/");
  lcd.print(time.year);
  lcd.setCursor(0, 1);
  lcd.print(time.hour);
  lcd.print(":");
  lcd.print(time.min);
  lcd.print(":");
  lcd.print(time.sec);
  delay(1500);

  ecouter_commandes();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nourriture: ");
  lcd.print(nourriture);
  delay(1500);
 
  ecouter_commandes();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print(" °C");
  delay(1500);

  ecouter_commandes();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eclairage: ");
  lcd.print((eclairage == 0) ? "ON" : (eclairage == 1) ? "OFF" : "Auto");
  delay(1000);
}

void distribuer_nourriture() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distribution...");
  while (true) {
    // TODO: code de distribution de nourriture
    if (digitalRead(pin_bouton_1) == HIGH) {
      break;
    }
  } 
}

void ecouter_commandes() {
  if (digitalRead(pin_bouton_1) == LOW) {
    distribuer_nourriture();
  }

  if (digitalRead(pin_bouton_2) == LOW) {
    choisir_action_menu_principal();
  }
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
        delay(1000);
      }
      else if (valeur_potentiometre < 666) {
        choisir_temperature();
        delay(1000);
      }
      else {
        choisir_eclairage();
        delay(1000);
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
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 666) {
        nourriture = 2;
        delay(1000);
        return;
      }
      else {
        nourriture = 3;
        delay(1000);
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
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 280) {
        temperature = 19;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 420) {
        temperature = 20;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 560) {
        temperature = 21;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 700) {
        temperature = 22;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 840) {
        temperature = 23;
        delay(1000);
        return;
      }
      else {
        temperature = 24;
        delay(1000);
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
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 666) {
        eclairage = 1;
        delay(1000);
        return;
      }
      else {
        eclairage = 2;
        delay(1000);
        return;
      }
    }
    delay(100);
  }
}