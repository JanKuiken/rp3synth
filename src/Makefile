
APP         := rp3synth

EXEDIR      := ../run

OBJS        := main.o            \
               adsr.o            \
               wave.o            \
               utils.o           \
               voice.o           \
               filter.o          \
               chorus.o          \
               rp3synth.o        \
               voiceglobals.o    \
               voicesettings.o


CC          := g++
LINK        := g++

CCLAGS      := -std=c++14 -Wall -O2
#CXXFLAGS    := -std=c++11 -Wall -Wextra -O2
#CXXFLAGS    := -std=c++11 -Wwrite-strings -fpermissive -O2

LIBS        := -lm -lasound -lpthread
LINKFLAGS   := -O2

EXECUTABLE  := $(EXEDIR)/$(APP)


all: $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) 
	$(LINK) $(LINKFLAGS) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

