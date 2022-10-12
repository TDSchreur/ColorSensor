#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ArduinoJson.h>
#include <az_core.h>
#include "ESP32Servo.h"

#include "seriallogger.h"
#include "timeconfig.h"
#include "wificonfig.h"
#include "AzIoTClient.h"
#include "mandm.h"

char buffer[1024];
float r, g, b;

Servo myservo; // create servo object to control a servo

// Integration time en gain hebben veel invloed op de snelheid en nauwkeurigheid van de sensor, deze beetje tweaken voor het beste resultaat
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X);

// Dit zijn de marge's van de sensor.
// red_min, red_max, green_min, green_max, blue_min, blue_max
MandM *red = new MandM(160, 170, 45, 55, 25, 35);
MandM *geel = new MandM(120, 130, 80, 95, 20, 35);
MandM *groen = new MandM(75, 85, 110, 120, 40, 50);
MandM *oranje = new MandM(130, 140, 65, 75, 25, 40);
MandM *blauw = new MandM(50, 65, 90, 100, 85, 100);
MandM *bruin = new MandM(110, 120, 80, 90, 40, 50);

static void
receivedCallback(String payload)
{
  // todo => M&M laten valleen wanneer het bakje klaar staat
}

static void getRGBColors(az_span payload, az_span *out_payload)
{
  // draaien maar
  myservo.write(86);

  String kleur;
  bool colorFound = false;

  // WHOOOOOOOOOOOOOOOOOOOT Lekker ranzige hackaton code!
  while (!colorFound)
  {

    tcs.getRGB(&r, &g, &b);

    snprintf(buffer, sizeof(buffer), "R: %.0f G: %.0f B: %.0f", r, g, b);
    Logger.Info(buffer);

    if (red->IsMatch(r, g, b))
    {
      Logger.Info("ROOD");
      kleur = "rood";
      colorFound = true;
    }
    else if (geel->IsMatch(r, g, b))
    {
      Logger.Info("GEEL");
      kleur = "geel";
      colorFound = true;
    }
    else if (groen->IsMatch(r, g, b))
    {
      Logger.Info("GROEN");
      kleur = "groen";
      colorFound = true;
    }
    else if (oranje->IsMatch(r, g, b))
    {
      Logger.Info("ORANJE");
      kleur = "oranje";
      colorFound = true;
    }
    else if (blauw->IsMatch(r, g, b))
    {
      Logger.Info("BLAUW");
      kleur = "blauw";
      colorFound = true;
    }
    else if (bruin->IsMatch(r, g, b))
    {
      Logger.Info("BRUIN");
      kleur = "bruin";
      colorFound = true;
    }
    else
    {
      // Logger.Error("KLEUR NIET KUNNEN BEPALEN");
      kleur = "leeg";
    };
  }

  // servo stoppen
  myservo.write(90);

  // String json;
  StaticJsonDocument<256> document;
  char buffer[256];

  document["red"] = r;
  document["green"] = g;
  document["blue"] = b;
  document["kleur"] = kleur;

  serializeJson(document, buffer);

  az_span original_payload = payload;
  payload = az_span_copy(payload, az_span_create_from_str(buffer));
  payload = az_span_copy_u8(payload, '\0');
  *out_payload = az_span_slice(original_payload, 0, az_span_size(original_payload) - az_span_size(payload) - 1);

  // even wachten en dan de M7M weer doordraaien
  // eigenlijk willen we dit niet.
  // We willen een cloud-to-device message wanneer het juiste bakje klaar staat.
  sleep(6);
  myservo.write(86);
}

void setup(void)
{
  WifiHelper.InitializeWifi();

  TimeHelper.InitializeTime();

  myservo.attach(27); // attaches the servo on pin 25 to the servo object

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