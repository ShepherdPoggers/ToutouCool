
#include <Servo.h>

Servo servoToutou; // Permet de gérer le servo dans le code.

int pos = 0;

void setup() {
  servoToutou.attach(9);
  Serial.begin(9600);
}

void loop() {
  for(pos; pos <= 180; pos++) // effectue une rotation de 
  {
    Serial.print("Rotation : ");
    Serial.println(pos);
    servoToutou.write(pos);
    delay(15);
  }
  for (pos; pos >= 0; pos--)
  {
    Serial.print("Rotation : ");
    Serial.println(pos);
    servoToutou.write(pos);
    delay(15);
  }
}