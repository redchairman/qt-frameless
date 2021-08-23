QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QFramelessDialog.cpp \
    demodialog.cpp \
    demodialog2.cpp \
    demowidget.cpp \
    iconhelper.cpp \
    main.cpp \
    myframelessiconctrl.cpp \
    widget.cpp

HEADERS += \
    QFramelessDialog.h \
    demodialog.h \
    demodialog2.h \
    demowidget.h \
    iconhelper.h \
    myframelessiconctrl.h \
    widget.h

FORMS += \
    demodialog.ui \
    demodialog2.ui \
    demowidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QtFrameless.qrc

include($$PWD/frameless/frameless.pri)

