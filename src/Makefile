MAC = 1
RELEASE = 1
CC = g++

ifdef RELEASE
CFLAGS += -Wall -O3
else
CFLGAS += -g -Wall
endif

ifdef MAC
# if boost was installed by port
CFLAGS += -I/opt/local/include/
LFLAGS += -L/opt/local/lib
endif

LFLAGS += -Wall

INCLUDES  = -I.

OBJDIR  = ../obj
BINDIR  = ../bin

all: dirs $(BINDIR)/dleft $(BINDIR)/singh
	
.PHONY: dirs clean

################################################################################
################################################################################
$(BINDIR)/singh: $(OBJDIR)/singh.o
	$(CC) -o $(BINDIR)/singh $(OBJDIR)/singh.o $(LFLAGS)

$(BINDIR)/dleft: $(OBJDIR)/dleft.o
	$(CC) -o $(BINDIR)/dleft $(OBJDIR)/dleft.o $(LFLAGS)

$(OBJDIR)/singh.o: singh.cpp Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c singh.cpp -o $(OBJDIR)/singh.o

$(OBJDIR)/dleft.o: dleft.cpp Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c dleft.cpp -o $(OBJDIR)/dleft.o

dirs:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

clean:
	\rm -f $(OBJDIR)/*
	\rm -f $(BINDIR)/*
