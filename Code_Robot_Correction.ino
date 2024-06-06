//Alexandros Chrissantakis
//5TQA - INRACI
//Code_Robot_Antête
//Feather M0 Express - HC08 - Dual Moteur + Driver Dual Moteur - Capteur
//Librairie Utilisées - <Adafruit_SH110X.h> - <Adafruit_NeoPixel.h>

//**Librairie**
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>

//**Définition des Ports**
#define NEOPIXEL_PIN 8

#define TRIG_PIN_G A0
#define ECHO_PIN_G A1
#define TRIG_PIN_D A2
#define ECHO_PIN_D A3

#define IN1 10
#define IN2 13
#define IN3 11
#define IN4 12

#define BP_A 9
#define BP_B 6
#define BP_C 5

//**Définition des variables**
byte Mode = 0;
byte Mode_BT = 1;
byte Mode_AUTO = 2;
byte Mode_TEST = 3;

byte vitesse = 255;
byte vitesse1 = 250;
byte vitesse2 = 250;

int data = 0;

unsigned long Impulsion_Echo_G;
unsigned long Impulsion_Echo_D;

unsigned int Distance_D;
unsigned int Distance_G;


//**Object**
Adafruit_NeoPixel NeoPixel(1, NEOPIXEL_PIN, NEO_GRB);
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void avant() {
  analogWrite(IN1, vitesse1);
  digitalWrite(IN2, 0);
  analogWrite(IN3, vitesse2);
  digitalWrite(IN4, 0);
}

void frein() {
  analogWrite(IN1, 255);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 255);
  digitalWrite(IN4, 1);
}

void gauche() {
  analogWrite(IN1, 255);
  digitalWrite(IN2, 0);
  analogWrite(IN3, 0);
  digitalWrite(IN4, 1);
}

void droite() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 255);
  digitalWrite(IN4, 0);
}

void reculer() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 0);
  digitalWrite(IN4, 1);
}

void Rotation_G() {
  analogWrite(IN1, 255);
  digitalWrite(IN2, 0);
  analogWrite(IN3, 0);
  digitalWrite(IN4, 1);
}
void Rotation_D() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 255);
  digitalWrite(IN4, 0);
}

void setup() {
  NeoPixel.begin();

  display.begin(0x3C, true);  // Initialisation du display à l'addresse 0x3C
  display.display();          // Affichage du buffer par défaut
  delay(750);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);              // Choix de la taille du texte
  display.setTextColor(SH110X_WHITE);  // Choix de la couleur
  display.setRotation(1);              // Choix de l'orientation du texte
  display.setCursor(0, 0);             // Positionnement du curseur
  display.display();

  delay(1000);

  display.clearDisplay();
  display.display();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BP_A, INPUT_PULLUP);
  pinMode(BP_B, INPUT_PULLUP);
  pinMode(BP_C, INPUT_PULLUP);

  pinMode(TRIG_PIN_G, OUTPUT);
  pinMode(ECHO_PIN_G, INPUT);
  pinMode(TRIG_PIN_D, OUTPUT);
  pinMode(ECHO_PIN_D, INPUT);

  Serial.begin(9600);  // Initialiser la communication série
  Serial1.begin(9600);

  display.clearDisplay();
  display.setTextSize(1);
  display.setRotation(1);
  display.setCursor(0, 0);
  display.println(" ");
  display.println("Bouton A = Mode BT");
  display.println(" ");
  display.println("Bouton B = Mode AUTO");
  display.println(" ");
  display.println("Bouton C = Moteur V.");
  display.println(" ");
  display.display();

  while (Mode == 0) {
    if (!digitalRead(BP_A)) {
      Mode = Mode_BT;
    }
    if (!digitalRead(BP_B)) {
      Mode = Mode_AUTO;
    }
    if (!digitalRead(BP_C)) {
      Mode = Mode_TEST;
    }
  }
}

void loop() {
  if (Mode == Mode_AUTO) {
    digitalWrite(TRIG_PIN_D, 1);  //mesure capteur Droit
    delayMicroseconds(100);
    digitalWrite(TRIG_PIN_D, 0);

    Impulsion_Echo_D = pulseIn(ECHO_PIN_D, HIGH);
    Serial.println(Impulsion_Echo_D);

    Distance_D = Impulsion_Echo_D * 0.034 / 2;
    Serial.println(Distance_D);
    delay(1);


    digitalWrite(TRIG_PIN_G, 1);  //mesure capteur gauche
    delayMicroseconds(100);
    digitalWrite(TRIG_PIN_G, 0);

    Impulsion_Echo_G = pulseIn(ECHO_PIN_G, HIGH);
    Serial.println(Impulsion_Echo_G);

    Distance_G = Impulsion_Echo_G * 0.034 / 2;
    Serial.println(Distance_G);
    delay(1);

    analogWrite(IN1, vitesse);
    digitalWrite(IN2, 0);
    analogWrite(IN3, vitesse);
    digitalWrite(IN4, 0);

    if (Distance_D < 3) {
      Rotation_G();
      delay(50);
    }
    if (Distance_G < 3) {
      Rotation_D();
      delay(50);
    }
  }





  if (Mode == Mode_BT) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setRotation(1);
    display.setCursor(0, 0);

    display.println("*Mode BT*");
    display.display();

    if (Serial1.available()) {
      int Data = Serial1.read();
      Serial.print(Data);
      switch (Data) {
        case 48:  // frein
          Serial.println("frein");
          frein();
          break;

        case 49:  // avant
          Serial.println("avant");
          avant();
          break;

        case 50:  // tourné à droite
          Serial.println("droite");
          droite();
          break;

        case 51:  // reculer
          Serial.println("reculer");
          reculer();
          break;

        case 52:  // tourné à gauche
          Serial.println("gauche");
          gauche();
          break;
      }
    }
  }

  if (Mode == Mode_TEST) {
    if (!digitalRead(BP_A)) {
      vitesse1 = vitesse1 + 10;
      vitesse2 = vitesse2 + 10;
    }

    if (!digitalRead(BP_B)) {
      vitesse1 = 150;
      vitesse2 = 150;
    }

    if (!digitalRead(BP_C)) {
      vitesse1 = vitesse1 - 10;
      vitesse2 = vitesse2 - 10;
    }

    display.setCursor(0, 0);
    display.println("*Test Vitesse*");
    display.println(" ");
    display.print("Vitesse1 = ");
    display.println(vitesse1);
    display.print("Vitesse2 = ");
    display.println(vitesse2);
    display.display();
    display.clearDisplay();

    delay(100);

    analogWrite(IN3, vitesse1);  // avec vitesse variant de 0 à 255 et IN1=10
    digitalWrite(IN4, 0);
    analogWrite(IN1, vitesse1);  // avec vitesse variant de 0 à 255 et IN1=10
    digitalWrite(IN2, 0);
  }
}
