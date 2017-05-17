#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T12:08:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = air-conditioning-client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientsocket.cpp \
    login.cpp

HEADERS  += mainwindow.h \
    protocol.h \
    clientsocket.h \
    login.h

FORMS    += mainwindow.ui \
    login.ui
