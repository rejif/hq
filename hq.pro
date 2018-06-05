#-------------------------------------------------
#
# HQ
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hq
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
include(src/src.pri)

RESOURCES += \
    resources/resource.qrc

DISTFILES += \
    .travis.yml \
    appveyor.yml \
    strartup.vbs \
    hq.rc \
    hq.icon \
    hq.exe.manifest \
    README.md

CONFIG(debug, debug|release) {
    #message("debug")
    win32 {
        RC_FILE += hq.rc
    }
}

