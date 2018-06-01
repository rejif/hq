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
    icon.rc \
    app.icon \
    README.md

win32 {
    RC_FILE += icon.rc
}
