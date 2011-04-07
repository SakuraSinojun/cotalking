



CC      = gcc
CXX     = g++
LD      = g++
RM	= rm -frv


CFLAGS  ?= -c -Os -Werror -Wall -I../common
LDFLAGS ?= -lpthread

COUT	= -o
LOUT	= -o


TARGET = dlltest

#$(wildcard *.o)


OBJS =  clientdll.o dlltest.o net.o network.o pack.o


all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LOUT)$@

.cpp.o:
	$(CXX) $< $(CFLAGS) $(COUT)$@
	
.c.o:
	$(CC) $< $(CFLAGS) $(COUT)$@

.PHONY: all clean

clean:
	$(RM) $(OBJS) $(TARGET)















