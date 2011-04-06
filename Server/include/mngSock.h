/***************************************************
Project: grouptalk
author: TaXueWuHen
sock manager;
***************************************************/

#ifndef __TALK_MANAGERSOCK_H__
#define __TALK_MANAGERSOCK_H__

#include "../include/def.h"
#include "../include/receiveThread.h"
#include "../include/sendThread.h"
#include "../include/dataManager.h"

class TS_MngSock
{
	public:
		TS_MngSock();
		~TS_MngSock();
		bool init();                
        bool stopSock();
        bool allocateSock(int sock);
    private:
        TS_ReceiveThread thdReceive;
		TS_SendThread thdSend;
		TS_dataManager *dataMng;
	public:
		void setDataManager(TS_dataManager *pdm);

};


#endif

