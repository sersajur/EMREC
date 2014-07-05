#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T05:18:37
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Emotion_recognizer
TEMPLATE = app


SOURCES += main.cpp\
    Network/network.cpp \
    Network/neuralnode.cpp \
    Presenter/presenter.cpp \
    View/mainwindow.cpp \
    View/resizableitem.cpp \
    Network/fuzzynode.cpp

HEADERS  += \
    Network/network.h \
    Network/neuralnode.h \
    Presenter/presenter.h \
    View/iView.h \
    View/mainwindow.h \
    View/resizableitem.h \
    Memento/init_data.h \
    Presenter/simplex.h \
    Network/fuzzynode.h

FORMS    += View/mainwindow.ui

RESOURCES +=

