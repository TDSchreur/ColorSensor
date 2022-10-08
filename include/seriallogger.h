#include <Arduino.h>

#define SERIAL_LOGGER_BAUD_RATE 115200

class SerialLogger {
public:
    SerialLogger();

    void Info(String message);

    void Error(String message);
};

extern SerialLogger Logger;