#-------------------------------------------------
#
# Project created by QtCreator 2018-03-16T16:47:05
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
QT += xml
QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoCell
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    renderarea.cpp \
    autocell.cpp \
    oned.cpp \
    sleeper.cpp \
    newautocell.cpp \
    etat.cpp \
    quadlife.cpp \
    jeuvie.cpp \
    openautocell.cpp \
    xmldom.cpp

HEADERS += \
        mainwindow.h \
    renderarea.h \
    autocell.h \
    oned.h \
    sleeper.h \
    newautocell.h \
    etat.h \
    quadlife.h \
    jeuvie.h \
    openautocell.h \
    xmldom.h

FORMS += \
        mainwindow.ui \
    newautocell.ui \
    openautocell.ui

RESOURCES += \
    ressources.qrc
