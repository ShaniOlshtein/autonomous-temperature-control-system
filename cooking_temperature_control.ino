#include <SPI.h>
#include <TFT_eSPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// 1. Screen dimensions
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 170;
const int COL_WIDTH = SCREEN_WIDTH / 3;
const int HEADER_HEIGHT = SCREEN_HEIGHT / 4;
const int CONTENT_START_X = 1;
const int CONTENT_START_Y = HEADER_HEIGHT + 1;
const int SPR_WIDTH = COL_WIDTH - 2;
const int SPR_HEIGHT = SCREEN_HEIGHT - CONTENT_START_Y;

// 2. Objects
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprTimer = TFT_eSprite(&tft);
TFT_eSprite sprTarget = TFT_eSprite(&tft);
TFT_eSprite sprCurrent = TFT_eSprite(&tft);

#define LCD_BL 38
#define ADKEY_OUT_PIN 2
#define TEMP_SENSOR_PIN 3
#define TEMP_SENSOR_COUNT 2

// 3. Variables
unsigned long previousMillis = 0;
unsigned long temperatureRequestMillis = 0;
int remainingSeconds = 230;
bool timerRunning = true;
float currentTemp = NAN;
float targetTemp = 65.0;
bool isHot = true;
bool temperatureConversionPending = false;
enum SelectionField
{
  TIMER_POWER,
  TIMER_VALUE,
  TARGET_VALUE,
  TARGET_MODE,
  SELECTION_FIELD_COUNT
};

SelectionField selectedField = TIMER_POWER;
bool selectedFieldVisible = true;
unsigned long lastBlinkMillis = 0;
unsigned long lastAdKeyDebounceMillis = 0;
int lastAdKeyReading = 0;
int adKeyState = 0;
const unsigned long ADKEY_DEBOUNCE_MS = 35;
const unsigned long SELECTION_BLINK_MS = 400;
const unsigned long LONG_PRESS_MS = 600;
const unsigned long LONG_PRESS_REPEAT_MS = 250;
unsigned long actionPressMillis = 0;
unsigned long lastLongActionMillis = 0;
bool longActionStarted = false;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature temperatureSensor(&oneWire);

// 4. Render functions (defined once)
void renderTimer(bool isOn, String timeStr)
{
  sprTimer.fillSprite(TFT_BLACK);
  sprTimer.setTextDatum(MC_DATUM);

  if (isOn && (selectedField != TIMER_POWER || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_GREEN, TFT_BLACK);
    sprTimer.drawString("ON", COL_WIDTH / 2, 35, 4);
  }
  else if (!isOn && (selectedField != TIMER_POWER || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_RED, TFT_BLACK);
    sprTimer.drawString("OFF", COL_WIDTH / 2, SPR_HEIGHT / 2, 4);
  }

  if (isOn && (selectedField != TIMER_VALUE || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_WHITE, TFT_BLACK);
    sprTimer.drawString(timeStr, COL_WIDTH / 2, 85, 4);
  }

  sprTimer.pushSprite(CONTENT_START_X, CONTENT_START_Y);
}

void renderTargetTemp(float temp, bool hot)
{
  sprTarget.fillSprite(TFT_BLACK);
  sprTarget.setTextDatum(MC_DATUM);

  String str = String((int)temp) + "C";
  if (selectedField != TARGET_VALUE || selectedFieldVisible)
  {
    sprTarget.setTextColor(TFT_WHITE, TFT_BLACK);
    sprTarget.drawString(str, COL_WIDTH / 2, 38, 6);
  }

  if (selectedField != TARGET_MODE || selectedFieldVisible)
  {
    sprTarget.setTextColor(hot ? TFT_RED : TFT_CYAN, TFT_BLACK);
    sprTarget.drawString(hot ? "HOT" : "COLD", COL_WIDTH / 2, 88, 4);
  }

  sprTarget.pushSprite(COL_WIDTH + CONTENT_START_X, CONTENT_START_Y);
}

void renderCurrentTemp(float temp)
{
  sprCurrent.fillSprite(TFT_BLACK);
  sprCurrent.setTextDatum(MC_DATUM);
  sprCurrent.setTextColor(TFT_WHITE, TFT_BLACK);

  String str = isnan(temp) ? "ERR" : String((int)temp) + "C";
  sprCurrent.drawString(str, COL_WIDTH / 2, SPR_HEIGHT / 2, 6);

  sprCurrent.pushSprite((COL_WIDTH * 2) + CONTENT_START_X, CONTENT_START_Y);
}

void updateTemperature()
{
  unsigned long now = millis();

  if (temperatureConversionPending && now - temperatureRequestMillis >= 750)
  {
    float temperatureSum = 0.0;
    int validSensorCount = 0;

    for (int sensorIndex = 0; sensorIndex < TEMP_SENSOR_COUNT; sensorIndex++)
    {
      float measuredTemp = temperatureSensor.getTempCByIndex(sensorIndex);
      if (measuredTemp != DEVICE_DISCONNECTED_C && !isnan(measuredTemp))
      {
        temperatureSum += measuredTemp;
        validSensorCount++;
      }
    }

    currentTemp = validSensorCount > 0 ? temperatureSum / validSensorCount : NAN;

    temperatureConversionPending = false;
  }

  if (!temperatureConversionPending && now - temperatureRequestMillis >= 2000)
  {
    temperatureSensor.requestTemperatures();
    temperatureRequestMillis = now;
    temperatureConversionPending = true;
  }
}

void moveSelection(int direction)
{
  int nextField = static_cast<int>(selectedField) + direction;

  if (nextField < 0)
  {
    nextField = SELECTION_FIELD_COUNT - 1;
  }
  else if (nextField >= SELECTION_FIELD_COUNT)
  {
    nextField = 0;
  }

  selectedField = static_cast<SelectionField>(nextField);

  selectedFieldVisible = true;
  lastBlinkMillis = millis();
}

int readAdKey()
{
  int adcValue = analogRead(ADKEY_OUT_PIN);

  if (adcValue <= 300)
  {
    return 1; // SW1: approximately 0 ohms
  }
  if (adcValue <= 950)
  {
    return 2; // SW2: approximately 2 kohms
  }
  if (adcValue <= 1700)
  {
    return 3; // SW3: approximately 5.1 kohms
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

void changeSelectedValue(int direction, bool largeStep)
{
  if (selectedField == TIMER_POWER)
  {
    timerRunning = !timerRunning;
  }
  else if (selectedField == TIMER_VALUE)
  {
    int step = largeStep ? 60 : 1;
    remainingSeconds += direction * step;
    if (remainingSeconds < 0)
    {
      remainingSeconds = 0;
    }
  }
  else if (selectedField == TARGET_VALUE)
  {
    float step = largeStep ? 5.0 : 1.0;
    targetTemp += direction * step;
    if (targetTemp < 0)
    {
      targetTemp = 0;
    }
  }
  else if (selectedField == TARGET_MODE)
  {
    isHot = !isHot;
  }
}

void handleActionButton(int button)
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

void updateLongPress()
{
  if (adKeyState != 2 && adKeyState != 3)
  {
    return;
  }

  if (selectedField != TIMER_VALUE && selectedField != TARGET_VALUE)
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

void updateButtons()
{
  unsigned long now = millis();
  int adKeyReading = readAdKey();

  if (adKeyReading != lastAdKeyReading)
  {
    lastAdKeyDebounceMillis = now;
    lastAdKeyReading = adKeyReading;
  }

  if (now - lastAdKeyDebounceMillis >= ADKEY_DEBOUNCE_MS && adKeyReading != adKeyState)
  {
    adKeyState = adKeyReading;
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
    else if (adKeyState == 0)
    {
      longActionStarted = false;
    }
  }

  updateLongPress();
}

void updateSelectionBlink()
{
  unsigned long now = millis();
  if (now - lastBlinkMillis >= SELECTION_BLINK_MS)
  {
    lastBlinkMillis = now;
    selectedFieldVisible = !selectedFieldVisible;
  }
}

// 5. Static UI drawing
void drawStaticUI()
{
  tft.drawFastHLine(0, HEADER_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
  tft.drawFastVLine(COL_WIDTH, 0, SCREEN_HEIGHT, TFT_WHITE);
  tft.drawFastVLine(COL_WIDTH * 2, 0, SCREEN_HEIGHT, TFT_WHITE);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int centerY = HEADER_HEIGHT / 2;
  tft.drawString("Timer", COL_WIDTH / 2, centerY, 2);
  tft.drawString("Target Temp", COL_WIDTH + (COL_WIDTH / 2), centerY, 2);
  tft.drawString("Current Temp", (COL_WIDTH * 2) + (COL_WIDTH / 2), centerY, 2);
}

// 6. Setup and Loop
void setup()
{
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  pinMode(ADKEY_OUT_PIN, INPUT);
  analogSetPinAttenuation(ADKEY_OUT_PIN, ADC_11db);

  temperatureSensor.begin();
  temperatureSensor.setWaitForConversion(false);
  temperatureSensor.requestTemperatures();
  temperatureRequestMillis = millis();
  temperatureConversionPending = true;

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  sprTimer.createSprite(SPR_WIDTH, SPR_HEIGHT);
  sprTarget.createSprite(SPR_WIDTH, SPR_HEIGHT);
  sprCurrent.createSprite(SPR_WIDTH, SPR_HEIGHT);

  drawStaticUI();
}

void loop()
{
  updateButtons();
  updateSelectionBlink();
  updateTemperature();

  if (timerRunning && remainingSeconds > 0)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000)
    {
      previousMillis = currentMillis;
      remainingSeconds--;
    }
  }

  int minutes = remainingSeconds / 60;
  int seconds = remainingSeconds % 60;
  String timerStr = (minutes < 10 ? "0" : "") + String(minutes) + ":" +
                    (seconds < 10 ? "0" : "") + String(seconds);

  renderTimer(timerRunning, timerStr);
  renderTargetTemp(targetTemp, isHot);
  renderCurrentTemp(currentTemp);
}
