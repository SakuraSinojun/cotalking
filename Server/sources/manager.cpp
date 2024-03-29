
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include "../include/manager.h"


static bool isRunning;

TS_Manager::TS_Manager()
{
	isRunning = true;
}

TS_Manager::~TS_Manager()
{
}

void handler_sigalrm(int sig)
{
	isRunning = false;
}

bool TS_Manager::init(int argc, char **argv)
{
	if(getcwd(appPath, D_MAX_PATH) == NULL)
        {
                perror("getcwd");
                return false;
        }

	// 对参数进行处理;
	// ......
	signal(SIGALRM, handler_sigalrm);
	signal(SIGTERM, handler_sigalrm);
	mngSock.setDataManager(&dataMng);
    mngSock.init();
    if (!thdListen.init(D_LISTEN_PORT, &mngSock))
    	return false;

    return true;
}

void TS_Manager::run()
{               
	thdListen.start();
	while (isRunning)
	{
		usleep(1000);
	}
    mngSock.stopSock();
	usleep(1000);
}

