targets = BeatDetector

all: $(targets)

MPG123_CFLAGS  := $(shell pkg-config --cflags libmpg123)
MPG123_LDFLAGS := $(shell pkg-config --libs   libmpg123)

SND_CFLAGS     := $(shell pkg-config --cflags sndfile)
SND_LDFLAGS    := $(shell pkg-config --libs   sndfile)

# Oder of libs not that important here...
compile = $(CXX) $(CPPFLAGS) $(CFLAGS) $(MPG123_CFLAGS)
linkflags = $(MPG123_LDFLAGS) $(LDFLAGS)

BeatDetector: BeatDetector.cpp
		$(compile) -o BeatDetector BeatDetector.cpp Decoder/MP3Decoder.cpp Algorithm/Algorithm.cpp $(linkflags)

clean:
		rm -vf $(targets)
