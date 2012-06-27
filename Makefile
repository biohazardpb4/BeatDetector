targets = BeatDetector

all: $(targets)

MPG123_CFLAGS  := $(shell pkg-config --cflags libmpg123)
MPG123_LDFLAGS := $(shell pkg-config --libs   libmpg123)

SND_CFLAGS     := $(shell pkg-config --cflags sndfile)
SND_LDFLAGS    := $(shell pkg-config --libs   sndfile)

PHONON_CFLAGS  := $(shell pkg-config --cflags phonon)
PHONON_LDFLAGS := $(shell pkg-config --libs phonon)

INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/phonon -I/usr/include/qt4 -I/usr/include/qt4/phonon_compat -I.

# Oder of libs not that important here...
compile = $(CXX) $(CPPFLAGS) $(CFLAGS) $(MPG123_CFLAGS) $(SND_CFLAGS) $(PHONON_CFLAGS) -lpthread
linkflags = $(MPG123_LDFLAGS) $(LDFLAGS) $(SND_LDFLAGS) $(PHONON_LDFLAGS) $(INCPATH)

BeatDetector: BeatDetector.cpp
		$(compile) -o BeatDetector BeatDetector.cpp Decoder/MP3Decoder.cpp Algorithm/Algorithm.cpp $(linkflags)

clean:
		rm -vf $(targets)
