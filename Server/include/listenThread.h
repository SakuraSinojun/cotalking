/***************************************************
Project: grouptalk
author: TaXueWuHen
listen thread;
***************************************************/


#ifndef __TALK_LISTEN_THREAD_H__
#define __TALK_LISTEN_THREAD_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "../include/thread.h"
#include "../include/mngSock.h"

class TS_ListenThread: public TS_Thread
{
	public:
		TS_ListenThread();
		~TS_ListenThread();
		// init listen socket.
		// return 0 when no error;
		int init(int nport, void *mng);
		void run();
	private:
		int sockfd;			//socket file descriptor;
		int epfd;			//epoll file descriptor;
        TS_MngSock *mngSock;
	private:
		bool setNonBlock(int sockfd);


};


#endif

