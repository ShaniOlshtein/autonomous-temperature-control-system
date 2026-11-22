// input.cpp — ADKeyboard input module (owner: display-ui).
// Stage 0: code moved verbatim from cooking_temperature_control.ino, no behavior change (decision 15).
// Buttons only write state/settings through timer.* and config_store.* setters; never relays.
#include <Arduino.h>
#include "pins.h"
#include "input.h"
#include "display_ui.h"
#include "timer.h"
#include "config_store.h"

static unsigned long lastAdKeyDebounceMillis = 0;
static int lastAdKeyReading = 0;
static int adKeyState = 0;
static int lastAdcValue = 0;
static unsigned long lastAdcDebugMillis = 0;
static const unsigned long ADKEY_DEBOUNCE_MS = 35;
static const unsigned long LONG_PRESS_MS = 600;
static const unsigned long LONG_PRESS_REPEAT_MS = 250;
static const unsigned long SW5_STOP_PRESS_MS = 1200;
static unsigned long actionPressMillis = 0;
static unsigned long lastLongActionMillis = 0;
static bool longActionStarted = false;
static unsigned long sw5PressMillis = 0;
static bool sw5LongPressHandled = false;

static void moveSelection(int direction)
{
  int nextField = static_cast<int>(displayUiGetSelectedField()) + direction;

  if (nextField < 0)
  {
    nextField = SELECTION_FIELD_COUNT - 1;
  }
  else if (nextField >= SELECTION_FIELD_COUNT)
  {
    nextField = 0;
  }

  // Decision 38: TIMER_VALUE is not drawn while the timer is OFF, so skip it.
  if (nextField == TIMER_VALUE && !timerIsEnabled())
  {
    nextField += direction;

    if (nextField < 0)
    {
      nextField = SELECTION_FIELD_COUNT - 1;
    }
    else if (nextField >= SELECTION_FIELD_COUNT)
    {
      nextField = 0;
    }
  }

  displayUiSelectField(static_cast<SelectionField>(nextField));
}

static int readAdKey()
{
  int adcValue = analogRead(ADKEY_OUT_PIN);
  lastAdcValue = adcValue;

  if (adcValue <= 300)
  {
    return 1; // SW1: approximately 0 ohms
  }
  if (adcValue <= 950)
  {
    return 2; // SW2: approximately 2 kohms
  }
  if (adcValue <= 2250)
  {
    return 3; // SW3: measured approximately 1703 (2026-11-22)
  }
  if (adcValue <= 3000)
  {
    return 4; // SW4: measured approximately 2664
  }
  if (adcValue <= 3400)
  {
    return 5; // SW5: approximately 20 kohms
  }
  return 0; // No button pressed
}

static void changeSelectedValue(int direction, bool largeStep)
{
  if (displayUiGetSelectedField() == TIMER_POWER)
  {
    timerToggleEnabled();
  }
  else if (displayUiGetSelectedField() == TIMER_VALUE)
  {
    int step = largeStep ? 60 : 1;
    timerAddSeconds(direction * step);
  }
  else if (displayUiGetSelectedField() == TARGET_VALUE)
  {
    float step = largeStep ? 5.0 : 1.0;
    configSetTargetTemp(configGetTargetTemp() + direction * step);
  }
  else if (displayUiGetSelectedField() == TARGET_MODE)
  {
    configSetIsHot(!configGetIsHot());
  }
}

static void handleActionButton(int button)
{
  if (button != 2 && button != 3)
  {
    return;
  }

  int direction = button == 2 ? -1 : 1;
  changeSelectedValue(direction, false);
  actionPressMillis = millis();
  lastLongActionMillis = actionPressMillis;
  longActionStarted = false;
}

static void handleSw5Press()
{
  Serial.println("SW5 press: START");
  timerSetCookingActive(true);
  displayUiSetCookingColor(true);
  timerInitializeForCooking();
}

static void handleSw5LongPress()
{
  Serial.println("SW5 long press: STOP");
  timerSetCookingActive(false);
  displayUiSetCookingColor(false);
}

static void updateLongPress()
{
  if (adKeyState == 5)
  {
    if (!sw5LongPressHandled && millis() - sw5PressMillis >= SW5_STOP_PRESS_MS)
    {
      handleSw5LongPress();
      sw5LongPressHandled = true;
    }
    return;
  }

  if (adKeyState != 2 && adKeyState != 3)
  {
    return;
  }

  if (displayUiGetSelectedField() != TIMER_VALUE && displayUiGetSelectedField() != TARGET_VALUE)
  {
    return;
  }

  unsigned long now = millis();
  if (now - actionPressMillis < LONG_PRESS_MS)
  {
    return;
  }

  if (!longActionStarted || now - lastLongActionMillis >= LONG_PRESS_REPEAT_MS)
  {
    int direction = adKeyState == 2 ? -1 : 1;
    changeSelectedValue(direction, true);
    lastLongActionMillis = now;
    longActionStarted = true;
  }
}

void inputInit()
{
  pinMode(ADKEY_OUT_PIN, INPUT);
  analogSetPinAttenuation(ADKEY_OUT_PIN, ADC_11db);
}

void inputUpdate()
{
  unsigned long now = millis();
  int adKeyReading = readAdKey();

  if (now - lastAdcDebugMillis >= 250)
  {
    lastAdcDebugMillis = now;
    Serial.print("ADC=");
    Serial.print(lastAdcValue);
    Serial.print(" decoded=SW");
    Serial.println(adKeyReading);
  }

  if (adKeyReading != lastAdKeyReading)
  {
    lastAdKeyDebounceMillis = now;
    lastAdKeyReading = adKeyReading;
  }

  if (now - lastAdKeyDebounceMillis >= ADKEY_DEBOUNCE_MS && adKeyReading != adKeyState)
  {
    adKeyState = adKeyReading;
    Serial.print("Button state: SW");
    Serial.println(adKeyState);
    if (adKeyState == 1)
    {
      moveSelection(-1);
    }
    else if (adKeyState == 4)
    {
      moveSelection(1);
    }
    else if (adKeyState == 2 || adKeyState == 3)
    {
      handleActionButton(adKeyState);
    }
    else if (adKeyState == 5)
    {
      sw5PressMillis = now;
      sw5LongPressHandled = false;
      handleSw5Press();
    }
    else if (adKeyState == 0)
    {
      sw5PressMillis = 0;
      longActionStarted = false;
    }
  }

  updateLongPress();
}
