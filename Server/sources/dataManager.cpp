
#include <memory.h>
#include "../include/dataManager.h"
#include "../include/publicOpt.h"

TS_dataManager::TS_dataManager()
{
	dataPos = 0;
}
TS_dataManager::~TS_dataManager()
{
}

bool TS_dataManager::appendData(int id, void* data, int len)
{
	if (len > C_SCDATA_LENGTH)
		return false;
	if (dataPos >= C_MAX_MAINDATA)
		dataPos = 0;

        printf("dataPos=%d\n", dataPos);

	dataMain[dataPos].sock = id;
	dataMain[dataPos].length = len;
	memcpy(&(dataMain[dataPos].data), data, len);

	return true;
}
bool TS_dataManager::appendError(int id,int errid)
{

	return true;
}

void *TS_dataManager::getSendData(int id, int &len)
{
	int intPos = m_userMng.getDataMainPosition(id);
	if (intPos < 0)
	{
		len = 0;
		m_userMng.setDataMainPosition(id, dataPos);
		return NULL;
	}
	if (intPos < dataPos - C_MAX_SENDDATA)
		intPos = dataPos - C_MAX_SENDDATA;
	if (intPos > dataPos)
	{
		if (dataPos >= C_MAX_SENDDATA)
			intPos = dataPos - C_MAX_SENDDATA;
		else
		{
			intPos = C_MAX_MAINDATA - dataPos + 1;
		}
			
	}

	len = 0;
	while (len < C_MAX_SENDDATA)
	{
		intPos ++;
		if (intPos >= C_MAX_MAINDATA)
			intPos = 0;
		memcpy(dataSend + len, &(dataMain[intPos].data), dataMain[intPos].length);
		len += dataMain[intPos].length;
		if (intPos == dataPos)
			break;
	}
	m_userMng.setDataMainPosition(id, intPos);
	return dataSend;
	
}





