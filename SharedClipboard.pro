#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T21:28:45
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharedClipboard
TEMPLATE = app


SOURCES += main.cpp\
        sharedclipboard.cpp \
    clipboardservice.cpp \
    control.cpp \
    encoder.cpp \
    fileservice.cpp \
    sessionmanager.cpp \
    tcpservice.cpp \
    udpservice.cpp

HEADERS  += sharedclipboard.h \
    clipboardservice.h \
    common.h \
    control.h \
    encoder.h \
    fileservice.h \
    sessionmanager.h \
    shared_clipboard_exception.h \
    tcppackages.h \
    tcpservice.h \
    udpservice.h

FORMS    += sharedclipboard.ui

CONFIG += c++14

DISTFILES += \
    README.md
