#include <Servo.h>

Servo servoToutou; // Permet de gérer le servo dans le code.

int servoDelay = 15; // Delai du moteur
int pos = 50; // Position inital du moteur

unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 200; // en ms

// Definition des pins
int powerPinI = 12; 
int powerPinII = 2;
int buttonPin = 13;
int potPin = A3;

// L'état du bouton
bool state = false; 
int buttonState = 0;


int potVal = 0;
int lastPotVal = 0;

void setup()
{
  // Set des pins
  pinMode(powerPinI, OUTPUT);
  pinMode(powerPinII, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Set du toutou
  servoToutou.attach(9);
  Serial.begin(9600);
}

void boutonEcoute()
{
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    unsigned long currentTime = millis();

    if(currentTime - lastPressTime > debounceDelay)
    {
      state = !state;
      lastPressTime = currentTime;
      Serial.print("Changment \n");
    }
  }
}
void potEcoute()
{
  potVal = analogRead(potPin);   
  if(abs(potVal - lastPotVal) > 5)
  {
    servoDelay = map(potVal, 0, 1023, 1, 50);
    lastPotVal = potVal;
     Serial.print("Pot = ");
    Serial.print(potVal);
    Serial.print(" | Delai = ");
    Serial.println(servoDelay);
  }
}



void loop()
{
  digitalWrite(powerPinI, HIGH);
  digitalWrite(powerPinII, HIGH);

  if(state)
  {
    for(pos; pos <= 140; pos++) // effectue une rotation de 
    {
      servoToutou.write(pos);
      boutonEcoute();
      potEcoute();
      delay(servoDelay);
      
    }
    for (pos; pos >= 50; pos--)
    {
      servoToutou.write(pos);
      boutonEcoute();
      potEcoute();
      delay(servoDelay);
    }
  }
  else
  {
    boutonEcoute();
    //potEcoute();
  }

}








