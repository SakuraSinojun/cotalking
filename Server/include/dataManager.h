/***************************************************
Project: grouptalk
author: TaXueWuHen
data Manager;
***************************************************/
#ifndef __TALK_DATAPMANAGER_H__
#define __TALK_DATAPMANAGER_H__

#include <map>
#include "../include/def.h"
#include "../include/defGroupTalk.h"
#include "../include/userManager.h"

#define C_MAX_MAINDATA 1000

using namespace std;

//data main struct;
typedef struct TS_dmMain
{
	int sock;
	int length;
	scData data;
} sdmMain, *psdmMain;

const int C_MAX_SENDDATA = 10;

class TS_dataManager
{
	public:
		TS_dataManager();
		~TS_dataManager();
	private:
		sdmMain dataMain[C_MAX_MAINDATA];
		int dataPos;	//pointer on dataMain;
		TS_userManager m_userMng;
		byte dataSend[C_SCDATA_LENGTH * C_MAX_SENDDATA];
	public:
		//append data;
		//if the data of received is wrong, then len = 0;
		bool appendData(int id, void* data, int len);
		bool appendError(int id, int errid);
		void *getSendData(int id, int &len);


};

#endif

