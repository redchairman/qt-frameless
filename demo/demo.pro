QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET      = frameless
TEMPLATE    = app
DESTDIR     = $$PWD/../bin

CONFIG      += warn_off
HEADERS     += head.h \
    dialog.h \
    mainwindow.h \
    widget.h
SOURCES     += main.cpp \
    dialog.cpp \
    mainwindow.cpp \
    widget.cpp

INCLUDEPATH += $$PWD/../qframeless
include ($$PWD/../qframeless/qframeless.pri)

RESOURCES += skin/skin.qrc

FORMS += \
    dialog.ui \
    mainwindow.ui \
    widget.ui
