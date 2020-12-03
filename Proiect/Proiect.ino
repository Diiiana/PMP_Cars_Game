#include "avr/interrupt.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// masinuta utilizatorului
byte car[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x0A};

// masinuta de pe banda cealalta
byte carop[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x0A};

// scor
byte sc[8] = {0x1B, 0x12, 0x0A, 0x1B, 0x00, 0x00, 0x00, 0x00};
byte o[8] = {0x0E, 0x0A, 0x0A, 0x0E, 0x00, 0x00, 0x00, 0x00};
byte r[8] = {0x18, 0x14, 0x18, 0x14, 0x00, 0x00, 0x00, 0x00};

// inimioara - viata utilizatorului
byte hrt[8] = {0x0A, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00};

byte obstacol1[8] = {0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18};

// iesirea potentiometrului
int potPin = 1;

// valoare potentiometru
int val = 0; 

// butonul de start/stop
int ssPin = 21;

// jocul a inceput
volatile boolean inGame = false;

//se doreste finalizarea jocului
volatile boolean endingGame = false;

void setup() {
  // contrast lcd
  analogWrite(8, 100);
  // citire valoare potentiometru
  val = analogRead(potPin);
  lcd.begin(16, 2);
  
  // creare caractere CGROM
  lcd.createChar(0, car);
  lcd.createChar(1, carop);
  lcd.createChar(2, sc);
  lcd.createChar(3, o);
  lcd.createChar(4, r);
  lcd.createChar(5, hrt);
  
  // led verificare start
  pinMode(13, OUTPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("Cars");
  delay(3000);
  
  cli();
  // butonul de start/stop intrerupe jocul
  pinMode(ssPin, INPUT);
  digitalWrite(ssPin, HIGH); // SURSA de la intrerupere - rezistenta PULLUP
  attachInterrupt(digitalPinToInterrupt(21), startGame, RISING);
  
  sei();
}

void loop()
{ 
  //while(inGame){
    digitalWrite(13, LOW);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.write(byte(0));

    lcd.setCursor(11, 0);
    lcd.write(1);

    lcd.setCursor(13, 0);
    lcd.write(2);
    lcd.setCursor(14, 0);
    lcd.write(3);
    lcd.setCursor(15, 0);
    lcd.write(4);

    lcd.setCursor(0,0);
    lcd.write(5);
    lcd.setCursor(1,0);
    lcd.write(5);
    lcd.setCursor(2,0);
    lcd.write(5);

    lcd.setCursor(12, 1);
    lcd.print(val);
    delay(300);
 // }
}

// intreruperea
void startGame()
{
  inGame = true;
  digitalWrite(13, HIGH);
}
    
