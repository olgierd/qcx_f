#-------------------------------------------------
#
# Project created by QtCreator 2019-01-05T12:15:59
#
#-------------------------------------------------

QT       += core gui multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCX_QT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiorecorder.cpp \
    audiorecorderthread.cpp \
    eden.cpp \
    wavefile.cpp \
    progtimer.cpp \
    audioplayer.cpp \
    audioplayerthread.cpp \
    stopwatch.cpp \
    configfile.cpp

HEADERS  += mainwindow.h \
    audiorecorder.h \
    audiorecorderthread.h \
    eden.h \
    wavefile.h \
    progtimer.h \
    audioplayer.h \
    audioplayerthread.h \
    stopwatch.h \
    configfile.h

FORMS    += mainwindow.ui

CONFIG += c++11
