#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T14:41:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatterBox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connection.cpp

HEADERS  += mainwindow.h \
    connection.h

FORMS    += mainwindow.ui \
    profile.ui \
    connection.ui
