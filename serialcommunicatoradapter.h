#ifndef SERIALCOMMUNICATORADAPTER_H
#define SERIALCOMMUNICATORADAPTER_H
#include<string>
#include<iostream>
#ifdef _WIN32
#include "WindowsSerialCommunicator.h"
#else
#include "ArduinoSerialCommunicator.hpp"
#endif
class SerialCommunicatorAdapter {
    #ifdef _WIN32
    windows::Serial *serial_impl;
    #else
    linux::ArduinoSerialCommunicator *serial_impl;
    #endif
public:
    SerialCommunicatorAdapter(std::string port, unsigned long baud_rate)
    {
#ifdef _WIN32
        serial_impl = new windows::Serial(port, baud_rate);
#else
        serial_impl = new ArduinoSerialCommunicator(port, 115200);
#endif
    }

    ~SerialCommunicatorAdapter()
    {
        delete serial_impl;
    }

    bool isOpened() {
#ifdef _WIN32
        return serial_impl->IsConnected();
#else
        return serial_impl->isOpened();
#endif
    }

    void sendString(std::string msg)
    {
#ifdef _WIN32
        if(serial_impl->WriteData(msg.c_str(), msg.size()))
        {
            std::cout<<"sent"<<msg.c_str()<<std::endl;
        } else
        {
            std::cout<<"comm error"<<std::endl;
        }
#else
        serial_impl->sendString(msg);
#endif
    }

    std::string pollSerialPortForData()
    {
#ifdef _WIN32
        char buffer[256];
        int x = serial_impl->ReadData(buffer, 256);
        printf("pollfordata: bytes read=%d , message =  %s\n",x,buffer);
        return std::string(buffer);
#else
        return serial_impl->pollSerialPortForData();
#endif
    }

};

#endif // SERIALCOMMUNICATORADAPTER_H
