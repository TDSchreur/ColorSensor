#ifndef AZIOTCLIENT_H
#define AZIOTCLIENT_H

#include <Arduino.h>

class AzIoTClient
{
    // Event Type callbacks
    using ReceivedDataEvent = void (*)(String data);
    using TelemetryPayloadEvent = void (*)(az_span payload, az_span* out_payload);

    public:
        AzIoTClient();            
        // Event callbacks
        static ReceivedDataEvent receivedDataEvent;
        static TelemetryPayloadEvent telemetryPayLoadEvent;

        static void establishConnection(ReceivedDataEvent inputReceivedDataEvent, TelemetryPayloadEvent inputTelemetryPayLoadEvent);
        static void processTelemetry(bool sendTelemetry);
    private:
        // static functions
        static void initializeTime();
        static void initializeIoTHubClient();
        static int initializeMqttClient();
        static void sendTelemetry();
};

#endif // AZIOTCLIENT_H