QT += core
QT -= gui
QT += sql

CONFIG += c++11

TARGET = Skil2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    interface.cpp \
    datahandler.cpp

HEADERS += \
    interface.h \
    datahandler.h
