# Project: GroupTalk
# This file is part of the "GroupTalk".
# author: TaXueWuHen

CPP  = g++
CC   = gcc
RES  = 
SRCFILE = 
SRCFILE = sources/main.cpp sources/manager.cpp sources/thread.cpp sources/listenThread.cpp \
	sources/mngSock.cpp sources/receiveThread.cpp sources/sendThread.cpp \
	sources/dataProcess.cpp sources/dataManager.cpp sources/userManager.cpp
LINKOBJ  = $(SRCFILE:%.cpp=%.o) $(RES)
LIBPATH = -lpthread
BIN = network
OUTDIR = 
RM = rm -fr
BFLAG = -g -Wall -Werror -Os

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(LINKOBJ) $(BIN)

$(BIN): $(LINKOBJ)
	$(CPP) -o $(OUTDIR)$(BIN) $(LINKOBJ) $(LIBPATH)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(BFLAG)
