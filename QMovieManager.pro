#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T22:32:35
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMovieManager
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    addmoviedialog.cpp \
    filedownloader.cpp \
    moviedetaildialog.cpp

HEADERS  += mainwindow.h \
    addmoviedialog.h \
    filedownloader.h \
    moviedetaildialog.h

FORMS    += mainwindow.ui \
    addmoviedialog.ui \
    moviedetaildialog.ui
