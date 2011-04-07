
#include "../include/userManager.h"

TS_userManager::TS_userManager()
{
}
TS_userManager::~TS_userManager()
{
}


int TS_userManager::getDataMainPosition(int id)
{
	psdmUser pUser = map_find(&m_user, id);
	int intRes = -1;

        // 原程序是pUser = NULL...是故意还是写错?
	if (pUser == NULL)
	{
		pUser = new sdmUser();
		pUser->dataMainPos = -1;
		map_insert(&m_user, id, pUser);
	}
	else
	{
		intRes = pUser->dataMainPos;
	}

	return intRes;
}
void TS_userManager::setDataMainPosition(int id, int index)
{
	psdmUser pUser = map_find(&m_user, id);
	//int intRes = -1;
        //
        // 又一个pUser = NULL...
	if (pUser == NULL)
	{
		pUser = new sdmUser();
		map_insert(&m_user, id, pUser);
	}
	pUser->dataMainPos = index;
}



