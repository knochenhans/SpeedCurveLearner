#-------------------------------------------------
#
# Project created by QtCreator 2013-08-12T13:27:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeedCurveLearner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RtMidi.cpp \
    midi.cpp \
    about.cpp \
    exercise.cpp \
    exercisepage.cpp \
    designerpage.cpp \
    track.cpp \
    note.cpp \
    tabber.cpp \
    instrument.cpp \
    tabbertablemodel.cpp \
    speedcurvedisplay.cpp \
    xmlserialization.cpp \
    collection.cpp

HEADERS  += mainwindow.h \
    RtMidi.h \
    midi.h \
    about.h \
    exercise.h \
    exercisepage.h \
    designerpage.h \
    track.h \
    note.h \
    midiDefines.h \
    tabber.h \
    instrument.h \
    tabbertablemodel.h \
    speedcurvedisplay.h \
    xmlserialization.h \
    collection.h

DEFINES += __LINUX_ALSA__

LIBS += -lasound -lpthread
