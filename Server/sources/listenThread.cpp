
#include <pthread.h>
#include <memory.h>
#include "../include/listenThread.h"

TS_ListenThread::TS_ListenThread()
{
	sockfd = 0;
	epfd = 0;
	mngSock = NULL;
}
TS_ListenThread::~TS_ListenThread()
{
	if (sockfd)
		close(sockfd);
	if (epfd)
		close(epfd);
}

bool TS_ListenThread::setNonBlock(int sockfd)
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

int TS_ListenThread::init(int nport, void *mng)
{
	if (nport < 10 || mng == NULL)
		return -1;

    mngSock = (TS_MngSock *)mng;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return -11;

	struct sockaddr_in serAddr;
	bzero(&serAddr, sizeof(struct sockaddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(nport);
	serAddr.sin_addr.s_addr = INADDR_ANY;

	int nRet = bind(sockfd, (sockaddr *)&serAddr, sizeof(sockaddr));
	if (nRet == -1)
		return -12;
	nRet = listen(sockfd, 128);
	if (nRet == -1)
		return -13;

	// epoll
	epfd = epoll_create(256);
	if (epfd == -1)
		return -21;

	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET;

	nRet = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	if (nRet == -1)
		return false;

	return true;

}

void TS_ListenThread::run()
{
	if (epfd == 0)
		return;
	int ndfs, intLoop, acptSock;
	struct epoll_event events[10];
	while (isRunning)
	{
		ndfs = epoll_wait(epfd, events, 10, 500);  //actually.just one socket; ndfs == 1;
		for (intLoop = 0; intLoop < ndfs; intLoop ++)
		{
			if (events[intLoop].data.fd != sockfd)	//why? impossible!
				continue;
			acptSock = accept(sockfd , NULL , NULL);
			if (mngSock != NULL)
            	mngSock->allocateSock(acptSock);
		}
	}
}


