#include <Servo.h>

/**
 * Programme de contrôle d'un servo moteur avec :
 * - un bouton ON/OFF (avec anti-rebond)
 * - un capteur de tilt (accélère le mouvement)
 * - un retour progressif à la vitesse normale
 */

// ==================== OBJET SERVO ====================
Servo servoToutou; // Permet de gérer le servo dans le code

// ==================== CONSTANTES ====================

// Délais du servo (en ms)
float minDelay = 4;     // délai minimum (vitesse max)
float servoDelay = 18;  // délai actuel du servo
float maxDelay = 18;    // délai maximum (vitesse lente)

// Angles du servo
int angle1 = 180;
int angle2 = 0;

// Anti-rebond bouton
const unsigned long debounceDelay = 200; // en ms

// Pins
int powerPinI = 2;
const int buttonPin = 13;
const int potPin = A3;
const int tiltPin = 7;
const int servoPin = 9;

// Facteur d'accélération lors d'un tilt
const float stress = 1;

// ==================== VARIABLES D'ÉTAT ====================

// Servo
int pos = 0; // position initiale du servo

// Bouton
bool state = false;     // état ON/OFF
int buttonState = 0;    // lecture actuelle
unsigned long lastPressTime = 0; // dernière activation (debounce)

// Tilt
int tiltState = 0; 
int lastTilt = 0;

// Potentiomètre (non utilisé actuellement)
int potVal = 0;
int lastPotVal = 0;


// ==================== SETUP ====================
void setup()
{
  // Configuration des pins
  pinMode(powerPinI, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(tiltPin, INPUT);

  // Initialisation du servo
  servoToutou.attach(servoPin);

  // Communication série pour debug
  Serial.begin(9600);
}


// ==================== GESTION BOUTON ====================
/**
 * Détecte un appui bouton avec anti-rebond
 * Inverse l'état "state" à chaque appui valide
 */
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


// ==================== GESTION TILT ====================
/**
 * Détecte un changement du capteur tilt
 * Accélère le servo en réduisant le délai
 */
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


// ==================== RETABLISSEMENT ====================
/**
 * Ralentit progressivement le servo jusqu'à sa vitesse normale
 */
void retablissement()
{
  if(servoDelay < maxDelay)
  {
    servoDelay += 0.1;

    Serial.print("ServoDelay = ");
    Serial.println(servoDelay);
  }
}


// ==================== BOUCLE PRINCIPALE ====================
void loop()
{
  // Alimentation activée
  digitalWrite(powerPinI, HIGH);

  if(state)
  {
    // Rotation vers angle minimum
    for (pos; pos >= angle2; pos--)
    {
      servoToutou.write(pos);

      boutonEcoute();
      //potEcoute();
      tiltEcoute();

      delay(servoDelay);
    }

    retablissement();

    // Rotation vers angle maximum
    for(pos; pos <= angle1; pos++)
    {
      servoToutou.write(pos);

      boutonEcoute();
      //potEcoute();
      tiltEcoute();

      delay(servoDelay);
    }

    retablissement();
  }
  else
  {
    // Mode repos
    boutonEcoute();
    servoDelay = maxDelay;
    //potEcoute();
  }
}
