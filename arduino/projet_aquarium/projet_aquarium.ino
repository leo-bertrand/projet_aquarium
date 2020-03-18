#include <Wire.h>
#include "src/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "src/DS3231/DS3231.h"

const int pin_bouton_1 = 2;
const int pin_bouton_2 = 3;
const int pin_potentiometre = A0;

const int plage_horaire_eclairage_auto_matin[2] = {8, 10}; // entre 8h et 10h
const int plage_horaire_eclairage_auto_midi[2] = {14, 16}; // entre 14h et 16h
const int plage_horaire_eclairage_auto_soir[2] = {20, 22}; // entre 20h et 22h
const int horaire_nourrissage_matin = 8; // à 8h
const int horaire_nourrissage_midi = 14; // à 14h
const int horaire_nourrissage_soir = 20; // à 20h

int nourriture = 1; // nombre de poissons à nourrir
int temperature_souhaite = 21; // température de l'eau idéale
int eclairage = 2; // éclairage -> 0: ON | 1: OFF | 2: Auto

LiquidCrystal_I2C lcd(0x27,16,2); // déclaration de l'afficheur
DS3231 rtc(SDA, SCL); // déclaration de l'horloge
Time temps_courant; // déclaration d'un objet Time pour avoir l'heure actuelle

bool est_effectue_nourrissage_matin = false;
bool est_effectue_nourrissage_midi = false;
bool est_effectue_nourrissage_soir = false;

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
  temps_courant = rtc.getTime(); // récupération de l'heure de l'horloge
  proceder_actions_automatatises();

  ecouter_commandes();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(temps_courant.date);
  lcd.print("/");
  lcd.print(temps_courant.mon);
  lcd.print("/");
  lcd.print(temps_courant.year);
  lcd.setCursor(0, 1);
  lcd.print(temps_courant.hour);
  lcd.print(":");
  lcd.print(temps_courant.min);
  lcd.print(":");
  lcd.print(temps_courant.sec);
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
  lcd.print(temperature_souhaite);
  lcd.print(" °C");
  delay(1500);

  ecouter_commandes();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eclairage: ");
  lcd.print((eclairage == 0) ? "ON" : (eclairage == 1) ? "OFF" : "Auto");
  delay(1500);
}

void proceder_actions_automatatises() {

  float temperature_courante = 21; // TODO: récupérer la température courante via le capteur à la place de 21

  if (eclairage == 2) {
    if ((temps_courant.hour >= plage_horaire_eclairage_auto_matin[0] && temps_courant.hour <= plage_horaire_eclairage_auto_matin[1])
      || (temps_courant.hour >= plage_horaire_eclairage_auto_midi[0] && temps_courant.hour <= plage_horaire_eclairage_auto_midi[1])
      || (temps_courant.hour >= plage_horaire_eclairage_auto_soir[0] && temps_courant.hour <= plage_horaire_eclairage_auto_soir[1])) {
      allumer_lampe();
    }
    else {
      eteindre_lampe();
    }
  }
  else if (eclairage == 1) {
    eteindre_lampe();
  }
  else {
    allumer_lampe();
  }
  
  if ((temps_courant.hour == horaire_nourrissage_matin && !est_effectue_nourrissage_matin)) {
    distribuer_nourriture_auto();
    est_effectue_nourrissage_matin = true;
  }

  if ((temps_courant.hour == horaire_nourrissage_midi && !est_effectue_nourrissage_midi)) {
    distribuer_nourriture_auto();
    est_effectue_nourrissage_midi = true;
  }

  if ((temps_courant.hour == horaire_nourrissage_soir && !est_effectue_nourrissage_soir)) {
    distribuer_nourriture_auto();
    est_effectue_nourrissage_soir = true;
  }

  if (int(temperature_courante) < temperature_souhaite) {
    allumer_indicateur_temperature_led(-1);
    allumer_chauffage();
  }

  if (int(temperature_courante) == temperature_souhaite) {
    allumer_indicateur_temperature_led(0);
    eteindre_chauffage();
  }

  if (int(temperature_courante) > temperature_souhaite) {
    allumer_indicateur_temperature_led(1);
    eteindre_chauffage();
  }

  // réinitialiser des booléens pour la journée suivante
  if (temps_courant.hour == 23 && temps_courant.min == 59) {
    est_effectue_nourrissage_matin = false;
    est_effectue_nourrissage_midi = false;
    est_effectue_nourrissage_soir = false;
  }
}

void distribuer_nourriture_manuel() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distribution ...");

  while (true) {
    // TODO: code de distribution de nourriture
    if (digitalRead(pin_bouton_1) == HIGH) {
      break;
    }
  } 
}

void ecouter_commandes() {
  if (digitalRead(pin_bouton_1) == LOW) {
    distribuer_nourriture_manuel();
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
        temperature_souhaite = 18;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 280) {
        temperature_souhaite = 19;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 420) {
        temperature_souhaite = 20;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 560) {
        temperature_souhaite = 21;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 700) {
        temperature_souhaite = 22;
        delay(1000);
        return;
      }
      else if (valeur_potentiometre < 840) {
        temperature_souhaite = 23;
        delay(1000);
        return;
      }
      else {
        temperature_souhaite = 24;
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

void distribuer_nourriture_auto() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distribution ...");

  // TODO : allumer moteur 5 / 10 / 15 secondes en fonction de la variable nourriture
}

void eteindre_lampe() {
  // TODO
}

void allumer_lampe() {
  // TODO
}

void allumer_indicateur_temperature_led(int numero_led) {
  switch (numero_led) {
    case -1:
      // TODO: allumer diode jaune et éteindre les autres
      break;
    case 0:
      // TODO: allumer diode verte et éteindre les autres diodes
      break;
    case 1:
      // TODO: allumer diode rouge et eteindre les autres
      break;
  }
}

void allumer_chauffage(){
  // TODO
}

void eteindre_chauffage() {
  // TODO
}