TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle

QT += core
QT += testlib

SOURCES += main.cpp \
    tom.cpp

HEADERS += \
    tom.h

win32 {
    INCLUDEPATH += C:\Users\Samuelko\Documents\GitHub\boost_1_63_0\
                   C:\Users\eag\Documents\GitHub\boost_1_62_0\
}
