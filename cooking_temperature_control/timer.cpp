// Cooking state and countdown timer (runtime state only). Owner: control-logic.
// Stage 0: moved verbatim from cooking_temperature_control.ino (countdown block of loop(),
// initializeTimerForCooking, state part of setCookingActive). No behavior change (decision 15).
// This module never calls display code.
#include <Arduino.h>
#include "timer.h"

static unsigned long previousMillis = 0;
static int remainingSeconds = 230;
static bool cookingActive = false;
static bool timerEnabled = false;

void timerUpdate()
{
  if (cookingActive && timerEnabled && remainingSeconds > 0)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000)
    {
      previousMillis = currentMillis;
      remainingSeconds--;
    }
  }
}

bool timerIsEnabled()
{
  return timerEnabled;
}

void timerToggleEnabled()
{
  timerEnabled = !timerEnabled;
}

int timerGetRemainingSeconds()
{
  return remainingSeconds;
}

void timerAddSeconds(int delta)
{
  remainingSeconds += delta;
  if (remainingSeconds < 0)
  {
    remainingSeconds = 0;
  }
}

bool timerIsCookingActive()
{
  return cookingActive;
}

void timerSetCookingActive(bool active)
{
  cookingActive = active;
}

void timerInitializeForCooking()
{
  if (timerEnabled)
  {
    previousMillis = millis();
  }
}
