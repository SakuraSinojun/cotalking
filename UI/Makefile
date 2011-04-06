

CC	= cl
CXX	= cl
LD	= link
RC	= rc


COUT	= /Fo
LOUT	= /OUT:

CFLAGS	= /c /O2 /nologo /W3 /WX /D_CRT_SECURE_NO_WARNINGS /I.\WTL_lib /EHsc
LDFLAGS	= /nologo

# nmake居然不支持vpath 0 0
# VPATH	= .\GrouptalkClient_Win32
WUPATH	= .\GrouptalkClient_Win32

OUTPATH	= ..\..\Release


TARGET	= $(OUTPATH)\win32ui.exe

OBJS	= $(WUPATH)\GrouptalkApp.o \
	  $(WUPATH)\stdafx.o \
	  $(WUPATH)\res.RES



$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LOUT)"$@"
	
.cpp.o:
	$(CXX) $< $(CFLAGS) $(COUT)$@
	
.c.o:
	$(CC) $< $(CFLAGS) $(COUT)$@
	
%.RES: %.rc
	$(RC) $<


.PHONY: clean

clean:
	del GrouptalkClient_Win32\*.o GrouptalkClient_Win32\*.RES /Q

















