#include <Servo.h>

Servo servoToutou; // Permet de gérer le servo dans le code.

int pos = 0;

void setup() {

  servoToutou.attach(9);
  Serial.begin(9600);
  servoToutou.write(180);
}

void loop() {
  
}