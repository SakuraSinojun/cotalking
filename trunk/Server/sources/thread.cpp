
#include <pthread.h>
#include "../include/thread.h"

TS_Thread::TS_Thread()
{
	isRunning = true;
}
TS_Thread::~TS_Thread()
{
}

bool TS_Thread::start()
{
	int nRet = pthread_create(&pid, NULL, funcThread, this);
	if (nRet)
	{
		return false;
	}

	return true;
}

bool TS_Thread::stop()
{
	isRunning = false;
        return true;
}

void *TS_Thread::funcThread(void *inst)
{
	TS_Thread *thd = (TS_Thread *)inst;
	thd->run();

        return NULL;
}


