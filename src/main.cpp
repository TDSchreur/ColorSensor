#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ArduinoJson.h>
#include <az_core.h>

#include "seriallogger.h"
#include "timeconfig.h"
#include "wificonfig.h"
#include "AzIoTClient.h"

char buffer[1024];
float r, g, b;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

static void receivedCallback(String payload)
{
  // StaticJsonDocument<256> document;
  // DeserializationError err = deserializeJson(document, payload);

  // if (err)
  // {
  //   Logger.Info("Failed to deserialize payload: ");
  //   Logger.Info(payload);
  // }

  // if (document["DeviceOn"] == true)
  // {
  //   digitalWrite(LEDPIN, HIGH);
  // }
  // else if (document["DeviceOn"] == false)
  // {
  //   digitalWrite(LEDPIN, LOW);
  // }
}

static void getTemperaturePayload(az_span payload, az_span *out_payload)
{
  // String json;
  StaticJsonDocument<256> document;
  char buffer[256];

  // // Reading temperature or humidity takes about 250 milliseconds!
  // // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // float h = dht.readHumidity();
  // // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();
  // // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);

  tcs.getRGB(&r, &g, &b);

  document["red"] = r;
  document["green"] = g;
  document["blue"] = b;

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

  AzIoTClient::establishConnection(receivedCallback, getTemperaturePayload);
}

void loop(void)
{
  AzIoTClient::processTelemetry(true);

  // tcs.getRGB(&r, &g, &b);
  // snprintf(buffer, sizeof(buffer), "R: %.0f G: %.0f B: %.0f", r, g, b);
  // Logger.Info(buffer);
}