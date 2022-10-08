#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#include "seriallogger.h"
#include "timeconfig.h"
#include "wificonfig.h"

char buffer[1024];
float r, g, b;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void)
{
  WifiHelper.InitializeWifi();

  TimeHelper.InitializeTime();

  Serial.begin(115200);

  Wire.begin(22, 23);
  if (tcs.begin())
  {
    Logger.Info("Found sensor");
  }
  else
  {
    Logger.Error("No TCS34725 found ... check your connections");
    while (1)
      ;
  }
}

void loop(void)
{
  // delay(1000); // takes 50ms to read
  // tcs.getRawData(&red, &green, &blue, &c);
  tcs.getRGB(&r, &g, &b);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  // colorTemp = tcs.calculateColorTemperature_dn40(red, green, blue, c);
  // lux = tcs.calculateLux(red, green, blue);

  snprintf(buffer, sizeof(buffer), "R: %.0f G: %.0f B: %.0f", r, g, b);
  Logger.Info(buffer);
}