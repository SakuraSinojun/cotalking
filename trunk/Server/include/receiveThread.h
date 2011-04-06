/***************************************************
Project: grouptalk
author: TaXueWuHen
sock recive thread
***************************************************/


#ifndef __TALK_RECEIVE_THREAD_H__
#define __TALK_RECEIVE_THREAD_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "../include/thread.h"
#include "../include/defGroupTalk.h"
#include "../include/dataProcess.h"
#include "../include/dataManager.h"

class TS_ReceiveThread: public TS_Thread
{
	public:
		TS_ReceiveThread();
		~TS_ReceiveThread();
	private:
		int epfd;			//epoll file descriptor;
		int sockCount;
		bool setNonBlock(int sockfd);
        /*
        	 the sock was close if return -1;
	        */
        int receiveData(int sock);
		TS_dataProcess dataProcess;
		TS_dataManager *dataMng;
	public:
		void run();
		void addSock(int sock);
		void setDataManager(TS_dataManager *pdm);

};


#endif

