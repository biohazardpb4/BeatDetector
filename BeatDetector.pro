#-------------------------------------------------
#
# Project created by QtCreator 2012-06-28T00:03:12
#
#-------------------------------------------------

QT       += core gui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libmpg123

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += sndfile

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += phonon

TARGET = BeatDetector
TEMPLATE = app

SOURCES += main.cpp\
        qtmain.cpp \
    Algorithm/Algorithm.cpp \
    Decoder/MP3Decoder.cpp

HEADERS  += qtmain.h \
    Algorithm/Algorithm.h \
    Decoder/mpg123.h \
    Decoder/MP3Decoder.h

FORMS    += qtmain.ui
