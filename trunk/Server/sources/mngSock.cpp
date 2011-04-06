
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "../include/mngSock.h"


TS_MngSock::TS_MngSock()
{

}

TS_MngSock::~TS_MngSock()
{
}

bool TS_MngSock::init()
{
    thdReceive.start();
	thdSend.start();
}
bool TS_MngSock::allocateSock(int sock)
{
    thdReceive.addSock(sock);
	thdSend.addSock(sock);
}
bool TS_MngSock::stopSock()
{
    thdReceive.stop();
	thdSend.stop();
}
void TS_MngSock::setDataManager(TS_dataManager *pdm)
{
  dataMng = pdm;
}




