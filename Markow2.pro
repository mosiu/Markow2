QT += core
QT -= gui

TARGET = Markow2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    world.cpp \
    value_iteration.cpp

HEADERS += \
    world.h \
    value_iteration.h

