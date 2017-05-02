QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = file_copier
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    copythread.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    copythread.h

FORMS    += mainwindow.ui \
    dialog.ui
