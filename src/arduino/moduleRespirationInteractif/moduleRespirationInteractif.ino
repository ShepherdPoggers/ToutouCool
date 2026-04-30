#include <Servo.h>

Servo servoToutou; // Permet de gérer le servo dans le code.

float minDelay = 4;
float servoDelay = 18; // Delai du moteur
float maxDelay = 18;
int pos = 0; // Position inital du moteur

int angle1 = 180;
int angle2 = 0;


unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 200; // en ms

// Definition des pins
int powerPinI = 2;

const int buttonPin = 13;
const int potPin = A3;
const int tiltPin = 7;
const int servoPin = 9;
const float stress = 1;

// L'état du bouton
bool state = false; 
int buttonState = 0;

// L'état du tilt
int tiltState = 0;
int lastTilt = 0;

int potVal = 0;
int lastPotVal = 0;

void setup()
{
  // Set des pins
  pinMode(powerPinI, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(tiltPin, INPUT);

  // Set du toutou
  servoToutou.attach(servoPin);
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
      Serial.print(state);

    }
  }
}

void tiltEcoute()
{
  tiltState = digitalRead(tiltPin);
  if(tiltState != lastTilt)
  {
    lastTilt = tiltState;
    if((servoDelay - stress) > minDelay)
    {
      servoDelay -= stress;
      Serial.print("ServoDelay = ");
      Serial.println(servoDelay);
    }
  }
  
}

void retablissement()
{
  if(servoDelay < maxDelay)
    {
      servoDelay += 0.1;
      Serial.print("ServoDelay = ");
      Serial.println(servoDelay);

    }
}
 
/* void potEcoute()
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
 */


void loop()
{
  digitalWrite(powerPinI, HIGH);

  if(state)
  {
    for (pos; pos >= angle2; pos--)
    {
      servoToutou.write(pos);
      boutonEcoute();
      //potEcoute();
      tiltEcoute();
      //Serial.print("Angle de rotation = ");
      //Serial.println(pos);
      delay(servoDelay);
    }
    
    retablissement();
    
    for(pos; pos <= angle1; pos++) // effectue une rotation de 
    {
      servoToutou.write(pos);
      boutonEcoute();
      //potEcoute();
      tiltEcoute();
      //Serial.print("Angle de rotation = ");
      //Serial.println(pos);
      delay(servoDelay);
      
    }
    
    retablissement();
  }
  else
  {
    boutonEcoute();
    servoDelay = maxDelay;
    //potEcoute();
  }

}








