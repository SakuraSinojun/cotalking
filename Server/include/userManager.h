/***************************************************
Project: grouptalk
author: TaXueWuHen
user Manager;
***************************************************/
#ifndef __TALK_USERPMANAGER_H__
#define __TALK_USERPMANAGER_H__

#include <map>
#include "../include/defGroupTalk.h"
#include "../include/publicOpt.h"

using namespace std;

enum TS_Status
{
	Sta_Connected = 0,
	Sta_Logined
};


typedef struct TS_dmUser
{
	int dataMainPos;
	TS_Status userStatus;
} sdmUser, *psdmUser;


class TS_userManager
{
	public:
		TS_userManager();
		~TS_userManager();
	private:
		map<int, psdmUser> m_user;
	public:
		int getDataMainPosition(int id);
		void setDataMainPosition(int id, int index);
};

#endif


