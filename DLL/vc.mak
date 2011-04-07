



CC      = cl
CXX     = cl
LD      = link
RM	= del /Q /S


CFLAGS  = /nologo /c /O2 /WX /I..\common /I..\..\3rdparty\pthread
LDFLAGS = /nologo ws2_32.lib  /LIBPATH:..\..\3rdparty\pthread pthreadVC2.lib

COUT	= /Fo
LOUT	= /OUT:


TARGET = dlltest.exe

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
	$(RM) $(OBJS) $(TARGET) *.lib  *.exp















