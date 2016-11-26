#include <Arduino.h>

const int ledCount = 7;
const int ledPins[ledCount] = {12,11,10,9,8,7,6};
const int buttonPin = 5;

int buttonState = 0;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int gameState = 0;
int currentLed = 0;
int lives = 3;
int countingUp = 1;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long gameSpeed = 500;

void LightsController(long delaySeconds);
void Game(int led);
void Reset(void);
void Flash(void);
void TurnOff(void);

void setup(void)
{
  Serial.begin(9600);
  //Wait for the serial connection to be active
  while (!Serial){; }
  Serial.println("Serial Active");
  for(int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(A1,OUTPUT);
  pinMode(buttonPin,INPUT);
}

void loop()
{
  LightsController(gameSpeed);
  Game(currentLed);
}

/**
  * Function for dealing with the player control and scoring
*/
void Game(int led)
{
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;
      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH)
      {
        if(led == 3 && gameSpeed > 0)
        {
          digitalWrite(A1,HIGH);
          gameSpeed -= 100;
          delay(100);
          digitalWrite(A1,LOW);
        }
        else if(gameSpeed <= 500)
        {
          gameSpeed += 100;
        }
        else if(led != 3)
        {
          lives -= 1;
        }
      }
    }
  }
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  if(gameSpeed == 0 || lives <= 0)
  {
    delay(500);
    Flash();
    Reset();
  }
}

/**
 * Function for controlling the leds
 * @param delaySeconds seconds between each light change.
 */
void LightsController(long delaySeconds)
{
  unsigned long currentMillis  = millis();

  digitalWrite(ledPins[currentLed],HIGH);
  if(currentMillis  - previousMillis1 > delaySeconds)
  {
    previousMillis1 = currentMillis;
    digitalWrite(ledPins[currentLed],LOW);
    if(currentLed <= 7 && countingUp == 1)
    {
      currentLed += 1;
      if(currentLed == 6)
      {
        countingUp = 0;
      }
    }
    else if(countingUp == 0)
    {
      currentLed -= 1;
      if(currentLed == 0)
      {
        countingUp = 1;
      }
    }
  }

  if(currentMillis  - previousMillis1 > delaySeconds)
  {
    previousMillis1 = currentMillis;
    digitalWrite(ledPins[currentLed],LOW);
  }
}

/**
 * Resets the game back to the original state dependant on player lives
 */
void Reset(void)
{
    currentLed = 0;
    lives = 3;
    gameSpeed = 500;
    for(int i = 0; i < ledCount; i++)
    {
      digitalWrite(ledPins[i],LOW);
    }
}

/**
 * Flashes the leds on then off
 */
void Flash(void)
{
  for(int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i],HIGH);
  }
  delay(200);
  for(int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i],LOW);
  }
}

/**
 * Turns off all leds
 */
void TurnOff(void)
{
  for(int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i],LOW);
  }
}
