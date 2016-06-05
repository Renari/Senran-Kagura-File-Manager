#-------------------------------------------------
#
# Project created by QtCreator 2016-06-03T03:30:05
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SKFM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gxtfile.cpp \
    catfile.cpp \
    resourcefile.cpp

HEADERS  += mainwindow.h \
    gxtfile.h \
    catfile.h \
    resourcefile.h

FORMS    += mainwindow.ui
