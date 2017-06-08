QT += core
QT -= gui

CONFIG += c++11

TARGET = BallFollowr
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    serial.cpp \
    rs232.c \
    WindowsSerialCommunicator.cpp

HEADERS += \
    rs232.h \
    stdafx.h \
    ArduinoSerialCommunicator.hpp \
    serial.h \
    ArduinoSerialCommunicator.hpp \
    rs232.h \
    serialcommunicatoradapter.h \
    WindowsSerialCommunicator.h \
    util_funcs.h


LIBS += -L/usr/local/libs -lopencv_core -lopencv_highgui \
-lopencv_imgcodecs -lopencv_imgproc \
-lopencv_video -lopencv_videoio

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
