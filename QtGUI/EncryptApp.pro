#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T19:12:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncryptApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    secdialog.cpp \
    filedialog.cpp \
    invokeCLI.cpp \
    selectCipher.cpp

HEADERS  += mainwindow.h \
    secdialog.h \
    filedialog.h \
    invokeCLI.h \
    selectCipher.h

FORMS    += mainwindow.ui \
    secdialog.ui \
    selectCipher.ui
