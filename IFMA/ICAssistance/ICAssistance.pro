QT += serialport
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ICAssistance
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    config.cpp \
    connector.cpp

HEADERS  += widget.h \
    config.h \
    connector.h

FORMS    += widget.ui
