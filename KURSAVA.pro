#-------------------------------------------------
#
# Project created by QtCreator 2011-03-31T21:02:44
#
#-------------------------------------------------

QT       += core gui

TARGET = KURSAVA
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    my_diagram_scene.cpp \
    my_diagram_item.cpp \
    my_diagram_text_item.cpp \
    my_diagram_arrow.cpp

HEADERS  += mainwindow.h \
    my_diagram_scene.h \
    my_diagram_item.h \
    my_diagram_text_item.h \
    my_diagram_arrow.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resours.qrc

RC_FILE = myapp.rc

CONFIG += exception

QMAKE_LFLAGS += -static-libgcc
#Это нужно для статического линка
