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
int potPin = A1;

// valoare potentiometru
int currentSpeed = 0; 

// butonul de start/stop
int startPin = 21;
int stopPin = 20;
int backPin = 19;

// jocul a inceput
volatile boolean inGame = false;

// se doreste finalizarea jocului
volatile boolean endingGame = false;

// pozitionarea masinutei utilizatorului
int posX = 0;
int posY = 1;

// pentru a inapoia cu masina
boolean backwards = false;

// semafor
int redPin = 11;
int yellowPin = 10;
int greenPin = 9;

boolean semafor = false;
boolean driving = false;

// numar vieti
int life = 3;

void setup() {
  // contrast lcd
  analogWrite(8, 100);
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
  
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.write(byte(0));

  lcd.setCursor(10, 0);
  lcd.write(byte(1));
  
  lcd.setCursor(6, 0);
  lcd.print("Cars");
  lcd.setCursor(3, 1);
  lcd.print("START GAME");
  delay(1500);

  // setare pini semafor
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  cli();
  // butonul de start/stop intrerupe jocul
  pinMode(startPin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(startPin), startGame, FALLING);
  attachInterrupt(digitalPinToInterrupt(stopPin), endGame, FALLING);
  attachInterrupt(digitalPinToInterrupt(backPin), goBack, FALLING);
  
  sei();
}

void loop()
{ 
  if(inGame){
    while(inGame){
      isDriving();
     // semaforFunc();
      redPass();
      lcd.clear();
      lcd.setCursor(posX, posY);
      lcd.write(byte(0));

      lcd.setCursor(11, 0);
      lcd.write(1);

      lcd.setCursor(13, 0);
      lcd.write(2);
      lcd.setCursor(14, 0);
      lcd.write(3);
      lcd.setCursor(15, 0);
      lcd.write(4);

      lifeStatus();
      
      // citire valoare potentiometru
      currentSpeed = analogRead(potPin);
      int getNewY = moveSquares(currentSpeed);
      posX += getNewY; 
      if(posX >= 15){
        posX = 15;
      }
      if(posX <= 0){
        posX = 0;
      }
      lcd.setCursor(12, 1);
      lcd.print(currentSpeed);
    delay(300);
  }
  }else{
    if(endingGame){
      lcd.clear();
      lcd.print("Game ended");
      delay(5000);
      lcd.clear();
      endingGame = false;
    }
  }
}

// intreruperea pentru inceperea jocului
void startGame()
{
  if(digitalRead(startPin) == 0){
      posX = 0;
      posY = 1;
      inGame = true;
      endingGame = false;
  }
}

// intreruperea pentru finalizarea jocului
void endGame()
{
  if(digitalRead(stopPin) == 0){
      endingGame = true;
      inGame = false;
  }
}

// intreruperea pentru a schimba directia masinii
void goBack(){
  if(digitalRead(backPin) == 0){
    backwards = !backwards;
  }
}

// functia ce transforma viteza de la potentiometru in patratele
int moveSquares(int potVal){
  int sign = 1;
  if(backwards){
    sign = -1;
  }
    if(potVal < 100){
      return 0;
    }else{
      if(potVal < 400){
        return 1*sign;
      }else{
          if(potVal < 600){
            return 2*sign;
          }else{
            if(potVal < 900){
              return 3*sign;
            }else{
              if(potVal < 1000){
                return 4*sign;
              }
            }
          }
      }
    }
}

void lifeStatus(){
  for(int i=0; i< life; i++){
      lcd.setCursor(i,0);
      lcd.write(5);
  }
  if(life == 0){
      lcd.clear();
      lcd.print("Game ended");
      delay(5000);
      lcd.clear();
      endingGame = false;
  }
}
void semaforFunc(){
  int randomVal = random(5, 1050);
  if((millis()/1000)*10000 == randomVal*10000){
    semafor = true;
  }else{
    semafor = false;
  }
  if(!semafor){
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
  }else{
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    // delay(5000);
    semafor = false;
  }
}

void isDriving(){
  if(currentSpeed >= 100){
    driving = true;
  }else{
    driving = false;
  }
}
void redPass(){
  if(driving && semafor){
    life--;
  }
}
