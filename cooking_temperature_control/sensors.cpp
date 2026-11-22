// DS18B20 temperature sensors (non-blocking reads). Owner: control-logic.
// Stage 0: moved verbatim from cooking_temperature_control.ino (updateTemperature and the
// sensor block of setup()). No behavior change (decision 15).
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "pins.h"
#include "sensors.h"

static OneWire oneWire(TEMP_SENSOR_PIN);
static DallasTemperature temperatureSensor(&oneWire);

static float currentTemp = NAN;
static unsigned long temperatureRequestMillis = 0;
static bool temperatureConversionPending = false;

void sensorsInit()
{
  temperatureSensor.begin();
  Serial.print("OneWire devices found: ");
  Serial.println(temperatureSensor.getDeviceCount());
  temperatureSensor.setWaitForConversion(false);
  temperatureSensor.requestTemperatures();
  temperatureRequestMillis = millis();
  temperatureConversionPending = true;
}

void sensorsUpdate()
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
        Serial.print("Temperature sensor ");
        Serial.print(sensorIndex + 1);
        Serial.print(": ");
        Serial.print(measuredTemp);
        Serial.println(" C");
      }
      else
      {
        Serial.print("Temperature sensor ");
        Serial.print(sensorIndex + 1);
        Serial.println(": DISCONNECTED");
      }
    }

    currentTemp = validSensorCount > 0 ? temperatureSum / validSensorCount : NAN;

    Serial.print("Valid temperature sensors: ");
    Serial.print(validSensorCount);
    Serial.print("/");
    Serial.println(TEMP_SENSOR_COUNT);

    temperatureConversionPending = false;
  }

  if (!temperatureConversionPending && now - temperatureRequestMillis >= 2000)
  {
    temperatureSensor.requestTemperatures();
    temperatureRequestMillis = now;
    temperatureConversionPending = true;
  }
}

float sensorsGetCurrentTemp()
{
  return currentTemp;
}
