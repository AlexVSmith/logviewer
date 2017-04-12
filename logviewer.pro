#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T16:35:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = logviewer
TEMPLATE = app

SOURCES += main.cpp\
        core.cpp \
#    ../../logging/logging.cpp \
#    ../../logging/loglistener.cpp \
#    ../../logging/loghelper.cpp \
    logging/logging.cpp \
    logging/loglistener.cpp \
    logging/loghelper.cpp \
    childlog.cpp
#        loghelper.cpp \
#    loglistener.cpp \
#    logging.cpp

HEADERS  += core.h \
#    ../../logging/logging.h \
#    ../../logging/loglistener.h \
#    ../../logging/loghelper.h \
    logging/logging.h \
    logging/loglistener.h \
    logging/loghelper.h \
    childlog.h
#        loghelper.h \
#    loglistener.h \
#    logging.h

#INCLUDEPATH += $$PWD/../../logging
INCLUDEPATH += $$PWD/logging

FORMS    += core.ui
