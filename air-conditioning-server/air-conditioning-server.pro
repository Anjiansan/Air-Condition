#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T12:05:12
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = air-conditioning-server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversocket.cpp \
    clientconn.cpp \
    socketthread.cpp \
    dbmanager.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    protocol.h \
    serversocket.h \
    clientconn.h \
    socketthread.h \
    dbmanager.h \
    logindialog.h

FORMS    += mainwindow.ui \
    logindialog.ui
