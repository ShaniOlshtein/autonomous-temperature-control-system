#include <SPI.h>
#include <TFT_eSPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// 1. Screen dimensions
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 170;
const int COL_WIDTH = SCREEN_WIDTH / 3;
const int HEADER_HEIGHT = SCREEN_HEIGHT / 4;
const int SPR_HEIGHT = (SCREEN_HEIGHT - HEADER_HEIGHT) - 2;

// 2. Objects
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprTimer = TFT_eSprite(&tft);
TFT_eSprite sprTarget = TFT_eSprite(&tft);
TFT_eSprite sprCurrent = TFT_eSprite(&tft);

#define LCD_BL 38
#define BTN_LEFT 0
#define BTN_RIGHT 14
#define TEMP_SENSOR_PIN 3

// 3. Variables
unsigned long previousMillis = 0;
unsigned long temperatureRequestMillis = 0;
int remainingSeconds = 230;
bool timerRunning = true;
float currentTemp = NAN;
float targetTemp = 65.0;
bool temperatureConversionPending = false;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature temperatureSensor(&oneWire);

// 4. Render functions (defined once)
void renderTimer(bool isOn, String timeStr)
{
  sprTimer.fillSprite(TFT_BLACK);
  sprTimer.setTextDatum(MC_DATUM);

  if (isOn)
  {
    sprTimer.setTextColor(TFT_GREEN, TFT_BLACK);
    sprTimer.drawString("ON", COL_WIDTH / 2, 35, 4);

    sprTimer.setTextColor(TFT_WHITE, TFT_BLACK);
    sprTimer.drawString(timeStr, COL_WIDTH / 2, 85, 4);
  }
  else
  {
    sprTimer.setTextColor(TFT_RED, TFT_BLACK);
    sprTimer.drawString("OFF", COL_WIDTH / 2, SPR_HEIGHT / 2, 4);
  }

  sprTimer.pushSprite(0, HEADER_HEIGHT);
}

void renderTargetTemp(float temp)
{
  sprTarget.fillSprite(TFT_BLACK);
  sprTarget.setTextDatum(MC_DATUM);
  sprTarget.setTextColor(TFT_WHITE, TFT_BLACK);

  String str = String((int)temp) + "C";
  sprTarget.drawString(str, COL_WIDTH / 2, SPR_HEIGHT / 2, 6);

  sprTarget.pushSprite(COL_WIDTH, HEADER_HEIGHT);
}

void renderCurrentTemp(float temp)
{
  sprCurrent.fillSprite(TFT_BLACK);
  sprCurrent.setTextDatum(MC_DATUM);
  sprCurrent.setTextColor(TFT_WHITE, TFT_BLACK);

  String str = isnan(temp) ? "ERR" : String((int)temp) + "C";
  sprCurrent.drawString(str, COL_WIDTH / 2, SPR_HEIGHT / 2, 6);

  sprCurrent.pushSprite(COL_WIDTH * 2, HEADER_HEIGHT);
}

void updateTemperature()
{
  unsigned long now = millis();

  if (temperatureConversionPending && now - temperatureRequestMillis >= 750)
  {
    float measuredTemp = temperatureSensor.getTempCByIndex(0);
    if (measuredTemp != DEVICE_DISCONNECTED_C)
    {
      currentTemp = measuredTemp;
    }
    else
    {
      currentTemp = NAN;
    }

    temperatureConversionPending = false;
  }

  if (!temperatureConversionPending && now - temperatureRequestMillis >= 2000)
  {
    temperatureSensor.requestTemperatures();
    temperatureRequestMillis = now;
    temperatureConversionPending = true;
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
  Serial.begin(115200);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  temperatureSensor.begin();
  temperatureSensor.setWaitForConversion(false);
  temperatureSensor.requestTemperatures();
  temperatureRequestMillis = millis();
  temperatureConversionPending = true;

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  sprTimer.createSprite(COL_WIDTH - 20, SPR_HEIGHT);
  sprTarget.createSprite(COL_WIDTH - 2, SPR_HEIGHT);
  sprCurrent.createSprite(COL_WIDTH - 2, SPR_HEIGHT);

  drawStaticUI();
}

void loop()
{
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
  renderTargetTemp(targetTemp);
  renderCurrentTemp(currentTemp);
}
