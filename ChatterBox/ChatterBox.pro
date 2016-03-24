#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T14:41:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatterBox
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    connection.cpp \
    client.cpp \
    clientsrc.cpp

HEADERS  += mainwindow.h \
    connection.h \
    client.h \
    clientsrc.h \
    Value.h

FORMS    += mainwindow.ui \
    connection.ui \
    client.ui
