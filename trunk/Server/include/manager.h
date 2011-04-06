/***************************************************
Project: grouptalk
author: TaXueWuHen
manager class.
***************************************************/

#ifndef __TALK_MANAGER_H__
#define __TALK_MANAGER_H__

#include "../include/def.h"
#include "../include/mngSock.h"
#include "../include/listenThread.h"
#include "../include/dataManager.h"

class TS_Manager
{
	public:
		TS_Manager();
		~TS_Manager();
		bool init(int argc, char **argv);
		void run();
	private:
		char appPath[D_MAX_PATH];			//self path;
        TS_MngSock mngSock; 
		TS_ListenThread thdListen;
	public:
		TS_dataManager dataMng;
};


#endif

