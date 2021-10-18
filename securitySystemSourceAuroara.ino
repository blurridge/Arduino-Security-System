#include<Keypad.h>
#include<Servo.h>
#include<OneButton.h>
#include<Password.h>

Servo gate;
Password password = Password("0000");

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','&'}
};

OneButton button(11, true);

char newPass[5];
byte passCont = 0;
byte maxPasswordLength = 4;
byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8,9};
int greenLED = 12;
int redLED = 13;
int count = 0;
int buzzer = A2;
long duration;
int distance;
int trigPin = A0;
int echoPin = A1;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  gate.attach(10);
  gate.write(150);
  button.attachLongPressStop(longpress);
  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  digitalWrite(13, HIGH);
  keypad.addEventListener(keypadEvent);
  Serial.println("Veritas v2 by AuRoarRa");
  Serial.println("Developed for the future of security and anti-terrorism.");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(count == 1){
  keypad.getKey();
  button.tick();
  delay(100);
  }
  else
  {
  ultraPin();
  keypad.getKey();
  button.tick();
  delay(100);
  }

}

//LIGHTS

void red(){
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(100);
}

void green() {
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  delay(100);
}

//BUTTON SETUP

void buzzerOpen(){
  analogWrite(buzzer, 255);
  delay(500);
  analogWrite(buzzer, 0);
}

void buzzerClose(){
  analogWrite(buzzer, 255);
  delay(1000);
  analogWrite(buzzer, 0);
}

void buzzerNewPass(){
  analogWrite(buzzer,255);
  delay(100);
  analogWrite(buzzer,0);
  delay(50);
  analogWrite(buzzer,150);
  delay(100);
  analogWrite(buzzer,0);
}

void singleclick(){
  red();
  analogWrite(buzzer, 255);
  gate.write(150);
  Serial.println("Door is locked.");
  delay(1000);
  analogWrite(buzzer,0);
  delay(500);
  count = 0;
}

void redFlicker(){
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(redLED, LOW);
  delay(100);
}

void longpress(){
  for(int alarm = 1; alarm <= 25; alarm++)
  {
  analogWrite(buzzer, 130);
  redFlicker();
  analogWrite(buzzer, 0);
  redFlicker();
  analogWrite(buzzer, 130);
  redFlicker();
  analogWrite(buzzer, 0);
  redFlicker();
  }
}

void doubleclick(){
  green();
  gate.write(65);
  buzzerOpen();
  Serial.println("Override activated.");
  delay(3000);
  red();
  gate.write(150);
  buzzerClose();
  Serial.println("Override deactivated.");
  delay(1000);
}

//PASSWORD AND KEYPAD FUNCTIONS

void keypadEvent(KeypadEvent eKey){
  switch(keypad.getState())
  {
    case PRESSED:
    Serial.print("Enter:");
    Serial.println(eKey);

    Serial.write(254);
    switch(eKey)
    {
      case '*': checkPassword(); delay(1);
      break;
      case '#': password.reset(); delay(1);
      break;
      case '&': changePassword(); delay(1);
      break;
      default: password.append(eKey); delay(1);
      break;
    }
  }
}

void checkPassword() {
  if(password.evaluate())
  {
    green();
    gate.write(65);
    buzzerOpen();
    Serial.println("Welcome!");
    password.reset();
    count = 1;
  }
  else
  {
    red();
    gate.write(150);
    buzzerClose();
    Serial.println("You have entered the wrong password.");
    password.reset();
    count = 0;
  }
}

void changePassword() {
  if(count == 1)
  {
    password.reset();
    count = 0;
    passCont = 0;
    Serial.println("Please enter the new password.");
    while(passCont < maxPasswordLength)
    {
      char eKey = keypad.getKey();
      if(eKey)
      {
        newPass[passCont] = eKey;
        passCont++;
      }
    }
    gate.write(150);
    red();
    buzzerNewPass();
    password.set(newPass);
    password.reset();
    Serial.print("The new password is ");
    Serial.println(newPass);
  }
  else
  {
    buzzerClose();
    Serial.println("Please enter current password first.");
  }
}

void ultraPin(){
  digitalWrite(trigPin, LOW);
  delay(10);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2; 
  if(distance < 12)
  {
    analogWrite(buzzer, 255);
  }
  else
  {
    analogWrite(buzzer, 0);
  }
}