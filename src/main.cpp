#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ArduinoJson.h>
#include <az_core.h>

#include "seriallogger.h"
#include "timeconfig.h"
#include "wificonfig.h"
#include "AzIoTClient.h"
#include "mandm.h"

char buffer[1024];
float r, g, b;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

MandM *red = new MandM(160, 170, 45, 55, 25, 35);
MandM *geel = new MandM(120, 130, 80, 95, 20, 35);
MandM *groen = new MandM(75, 85, 110, 120, 40, 50);
MandM *oranje = new MandM(130, 140, 65, 75, 25, 40);
MandM *blauw = new MandM(50, 65, 90, 100, 85, 100);
MandM *bruin = new MandM(110, 120, 80, 90, 40, 50);

static void
receivedCallback(String payload)
{
  // todo
}

static void getRGBColors(az_span payload, az_span *out_payload)
{
  // String json;
  StaticJsonDocument<256> document;
  char buffer[256];

  tcs.getRGB(&r, &g, &b);

  snprintf(buffer, sizeof(buffer), "R: %.0f G: %.0f B: %.0f", r, g, b);
  Logger.Info(buffer);

  String kleur;
  if (red->IsMatch(r, g, b))
  {
    Logger.Info("ROOD");
    kleur = "rood";
  }
  else if (geel->IsMatch(r, g, b))
  {
    Logger.Info("GEEL");
    kleur = "geel";
  }
  else if (groen->IsMatch(r, g, b))
  {
    Logger.Info("GROEN");
    kleur = "groen";
  }
  else if (oranje->IsMatch(r, g, b))
  {
    Logger.Info("ORANJE");
    kleur = "oranje";
  }
  else if (blauw->IsMatch(r, g, b))
  {
    Logger.Info("BLAUW");
    kleur = "blauw";
  }
  else if (bruin->IsMatch(r, g, b))
  {
    Logger.Info("BRUIN");
    kleur = "bruin";
  }
  else
  {
    Logger.Error("KLEUR NIET KUNNEN BEPALEN");
    kleur = "leeg"
  };

  document["red"] = r;
  document["green"] = g;
  document["blue"] = b;
  document["kleur"] = kleur;

  serializeJson(document, buffer);

  az_span original_payload = payload;
  payload = az_span_copy(payload, az_span_create_from_str(buffer));
  payload = az_span_copy_u8(payload, '\0');
  *out_payload = az_span_slice(original_payload, 0, az_span_size(original_payload) - az_span_size(payload) - 1);
}

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

  AzIoTClient::establishConnection(receivedCallback, getRGBColors);
}

void loop(void)
{
  AzIoTClient::processTelemetry(true);
}