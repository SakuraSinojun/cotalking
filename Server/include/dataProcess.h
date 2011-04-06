/***************************************************
Project: grouptalk
author: TaXueWuHen
data Process;
***************************************************/
#ifndef __TALK_DATAPROCESS_H__
#define __TALK_DATAPROCESS_H__

#include <stdio.h>
#include <map>
#include "../include/def.h"
#include "../include/defGroupTalk.h"
#include "../include/publicOpt.h"
#include "../include/dataManager.h"

using namespace std;

// data cache
typedef struct st_scDataCache
{
	int dataLength;
	char *data[C_SCDATA_LENGTH * 2];
} scDataCache, *pscDataCache;

class TS_dataProcess
{
	private:
		int cachePoint;
		map<int, pscDataCache> m_dataCache;
		TS_dataManager *dataMng;
	public:
		TS_dataProcess();
		~TS_dataProcess();
	public:
		void* getAddr(int key);
		bool dataAnalyse(int key, int size);
		void setDataManager(TS_dataManager *pdm);
};

#endif

