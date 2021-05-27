QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    dialog2.cpp \
    main.cpp \
    shadowbasedialog.cpp \
    shadowbasewidget.cpp \
    shadowwidget.cpp \
    testdialog.cpp \
    testwidget1.cpp \
    widget.cpp \
    widget2.cpp

HEADERS += \
    dialog.h \
    dialog2.h \
    shadowbasedialog.h \
    shadowbasewidget.h \
    shadowwidget.h \
    testdialog.h \
    testwidget1.h \
    widget.h \
    widget2.h

FORMS += \
    dialog.ui \
    dialog2.ui \
    testdialog.ui \
    testwidget1.ui \
    widget.ui \
    widget2.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
