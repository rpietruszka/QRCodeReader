QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRCodeReader
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE -= -O1
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        qrcodereader.cpp \
    updatethread.cpp \
    camera.cpp

HEADERS  += qrcodereader.h \
    updatethread.h \
    camera.h

FORMS    += qrcodereader.ui

#libs required by RPi

LIBS += -lraspicam -lzbar -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lwiringPi -lopencv_video \
        -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util

