QT += core
QT -= gui

TARGET = final
CONFIG += console
CONFIG -= app_bundle

target.path = /home/root
INSTALLS += target


LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc


TEMPLATE = app

SOURCES += main.cpp \
    xbee.cpp

HEADERS += \
    xbee.h

