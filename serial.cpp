#include "serial.h"
#include <iostream>
#include <string>
#include "ArduinoSerialCommunicator.hpp"
#include <sstream>
#include<thread>

Serial::Serial(std::string port, unsigned long baudRate)
{
    arduinoCOM = new ArduinoSerialCommunicator(port, baudRate);
    if (!arduinoCOM->isOpened())
    {
        std::string err = "Could not open COM port";
        std::cout << err << std::endl;
        throw err;
    }


    //Stabilize communication
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

//    testing the communication with RS232 loop-back or arduino echo program
//    for (int i = 0; i < 10; i++)
//    {
//        std::string tempStringToSend = "V(20,30)";

//        arduinoCOM->sendString(tempStringToSend);
//        printf("[test]Sent to Arduino: '%s'\n", tempStringToSend.c_str());

//        std::this_thread::sleep_for(std::chrono::milliseconds(33));

//        std::string temp = arduinoCOM->pollSerialPortForData();
//        std::cout << temp << std::endl;

//        std::this_thread::sleep_for(std::chrono::milliseconds(33));

//    }
}

void Serial::println(std::string msg)
{   std::cout<<"[println] sent: "<<msg;
    arduinoCOM->sendString(msg);
    std::string s;
//    do {
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));
//         s = (arduinoCOM->pollSerialPortForData());
//    }while(s == "");
    std::cout<<", responded: "<<s<<std::endl;
}

std::string Serial::readln()
{
    return arduinoCOM->pollSerialPortForData();
}

int Serial::get_bearing()
{
    println("bearing");
    int x;
    std::stringstream(readln()) >> x;
    return x;
}
