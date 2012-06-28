#-------------------------------------------------
#
# Project created by QtCreator 2012-06-28T00:03:12
#
#-------------------------------------------------

QT       += core gui

TARGET = BeatDetector
TEMPLATE = app


SOURCES += main.cpp\
        qtmain.cpp \
    Algorithm/Algorithm.cpp \
    Decoder/Mpg123Decoder.cpp \
    Decoder/MP3Decoder.cpp

HEADERS  += qtmain.h \
    Algorithm/Algorithm.h \
    Decoder/Mpg123Decoder.h \
    Decoder/mpg123.h \
    Decoder/MP3Decoder.h

FORMS    += qtmain.ui
