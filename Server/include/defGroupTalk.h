/***************************************************
Project: grouptalk
author: TaXueWuHen
define of grouptalk;
***************************************************/
#ifndef __TALK_DEFINE_GROUPTALK_H__
#define __TALK_DEFINE_GROUPTALK_H__
#include <time.h>
#include "../include/def.h"


#define TS_DATA_MAX 255

//data format between server and client;
typedef struct ST_DATASC
{
	int version;
	short command;
	short requestID;
	time_t time;				// client time;
	int length;					//data length;
	byte data[TS_DATA_MAX];		//any data;
} scData, *pscData;				// server-client data;

const int C_SCDATA_LENGTH = sizeof(scData);
const int C_SCDATA_BASELEN = C_SCDATA_LENGTH - TS_DATA_MAX;
const int C_SCDATA_VERSION = 0x1;

enum E_SCDataError
{
	E_ERR_VERSION = 0,				// wrong version;
	E_ERR_DATALENGTH,				// wrong data length;

	E_ERR_COUNT
};



#endif

