QT       += core gui

include(QSourceHighlite.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    customthemedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    searchdialog.cpp

HEADERS += \
    customthemedialog.h \
    mainwindow.h \
    searchdialog.h

FORMS += \
    customthemedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md

RESOURCES +=
