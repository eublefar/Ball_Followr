#ifndef SERIAL_H
#define SERIAL_H

#include "serialcommunicatoradapter.h"
#define ArduinoSerialCommunicator SerialCommunicatorAdapter


enum State{ DRIVING = 1,
            DRIVING_REVERSE = 2,
            STOP = 0,
            TURNING_LEFT = 4,
            TURNING_RIGHT = 8,
            MOVING = 16
        };

class Serial
{
    ArduinoSerialCommunicator *arduinoCOM;
public:

    Serial(std::string port = "COM9", unsigned long baudRate = 115200);
    ~Serial()
    {
        delete arduinoCOM;
    }
    void println(std::string msg);
    std::string readln();
    int get_bearing();
};

#endif // SERIAL_H
