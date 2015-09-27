#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T20:37:58
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcustomplot
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qcustomplot.cpp

HEADERS += qcustomplot.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    changelog.txt \
    GPL.txt
