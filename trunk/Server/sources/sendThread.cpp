
#include "../include/sendThread.h"

TS_SendThread::TS_SendThread()
{
	pthread_mutex_init(&mutex_sock , NULL);
	intTemp = 0;
}
TS_SendThread::~TS_SendThread()
{
	pthread_mutex_destroy(&mutex_sock);
}

void TS_SendThread::addSock(int sock)
{
	if (sock < 1)
		return;
	pthread_mutex_lock(&mutex_sock);
	listSock.push_back(sock);
	pthread_mutex_unlock(&mutex_sock);
}

int TS_SendThread::sendData(int sock, void *data, int datalen)
{     
	if (sock < 1)
		return false;
	int intRet = 0;
    while (true)
    {
        intRet = send(sock, data, datalen, MSG_NOSIGNAL);
        if (intRet == 0) // the connect is closed;
        {
            return -1;
        }
        else if (intRet < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            break;
        }
        break;
    }
    return intRet;
}

void TS_SendThread::run()
{
	list<int>::iterator it = listSock.begin();
	void* dataSend;
	int intDataLen;
	while (isRunning)
	{
		if (it == listSock.end())
		{
			it = listSock.begin();
			sleep(1);
			continue;
		}
		dataSend = dataMng->getSendData(*it,intDataLen);
		if (dataSend == NULL)
		{
			it ++;
			continue;
		}
		if (sendData(*it, dataSend, intDataLen) == -1)
		{
			pthread_mutex_lock(&mutex_sock);
			it = listSock.erase(it);
			pthread_mutex_unlock(&mutex_sock);
		}
		else
		{
			it ++;
		}
		
	}

}
void TS_SendThread::setDataManager(TS_dataManager *pdm)
{
  dataMng = pdm;
}


