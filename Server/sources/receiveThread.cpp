
#include <pthread.h>
#include <memory.h>
#include "../include/listenThread.h"

TS_ReceiveThread::TS_ReceiveThread()
{
	epfd = epoll_create(256);
	sockCount = 0;
}
TS_ReceiveThread::~TS_ReceiveThread()
{
	if (epfd)
		close(epfd);
}

bool TS_ReceiveThread::setNonBlock(int sockfd)
{
	if (sockfd < 1)
		return false;

	int opts = fcntl(sockfd, F_GETFL);
	if (opts == -1)
		return false;

	opts = opts | O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, opts) < 0)
		return false;

	return true;
}

void TS_ReceiveThread::addSock(int sock)
{
	if (epfd < 1 || sock < 1)
		return;

	setNonBlock(sock);
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.fd = sock;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);

	sockCount ++;
}        
int TS_ReceiveThread::receiveData(int sock)
{     
	if (sock < 1)
		return false;
	int intLength = 0, intRet = 0;
	void* dataBuf = dataProcess.getAddr(sock);
    while (true)
    {
        intRet = recv(sock, dataBuf, C_SCDATA_LENGTH, MSG_NOSIGNAL);
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
        else
        {
            intLength += intRet;
			dataProcess.dataAnalyse(sock, intRet);
        }
    }
    return intLength;
}

void TS_ReceiveThread::run()
{
	if (epfd == 0)
		return;
	int ndfs, intLoop;
	struct epoll_event events[30], ev;
	while (isRunning)
	{
		ndfs = epoll_wait(epfd, events, 30, 500);
		for (intLoop = 0; intLoop < ndfs; intLoop ++)
		{
			if (events[intLoop].data.fd < 0)
				continue;
            if (receiveData(events[intLoop].data.fd) == -1) //closed;
            {
                ev.data.fd= events[intLoop].data.fd;
                epoll_ctl(epfd , EPOLL_CTL_DEL , events[intLoop].data.fd, &ev);
            }
		}
	}
}
void TS_ReceiveThread::setDataManager(TS_dataManager *pdm)
{
  dataMng = pdm;
}


