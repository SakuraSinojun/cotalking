/***************************************************
Project: grouptalk
author: TaXueWuHen
sock send thread
***************************************************/


#ifndef __TALK_SEND_THREAD_H__
#define __TALK_SEND_THREAD_H__

#include <stdio.h>
#include <list>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include "../include/thread.h"
#include "../include/dataManager.h"

using namespace std;

class TS_SendThread: public TS_Thread
{
	public:
		TS_SendThread();
		~TS_SendThread();
	private:
		list<int> listSock;
		pthread_mutex_t mutex_sock;
		int intTemp;
		TS_dataManager *dataMng;
	private:
        /*da
        	 the sock was close if return -1;
	        */
		int sendData(int sock, void *data, int datalen);
	public:
		void run();
		void addSock(int sock);
		void setDataManager(TS_dataManager *pdm);


};


#endif


