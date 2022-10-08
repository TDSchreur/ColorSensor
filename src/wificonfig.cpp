#include <WiFi.h>

#include "wificonfig.h"
#include "seriallogger.h"

WifiConfig::WifiConfig() {}

void WifiConfig::InitializeWifi()
{
    Serial.begin(115200);
    delay(2000);
    Logger.Info("Setup start");

    WiFi.begin("LENNIE", "/G2a3845");
    Logger.Info("Connecting to WIFI SSID " + String("LENNIE"));

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Logger.Info("WiFi connected, IP address: " + WiFi.localIP().toString());
}

WifiConfig WifiHelper;