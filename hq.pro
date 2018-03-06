#-------------------------------------------------
#
# HQ
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hq
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
include(src/src.pri)

DISTFILES += \
    .travis.yml \
    appveyor.yml \
    README.md

