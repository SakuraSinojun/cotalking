
#include <malloc.h>
#include <memory.h>
#include "../include/dataProcess.h"
#include "../include/manager.h"


TS_dataProcess::TS_dataProcess()
{
	cachePoint = 0;
}
TS_dataProcess::~TS_dataProcess()
{
}

void* TS_dataProcess::getAddr(int key)
{
	pscDataCache pCache = map_find(&m_dataCache, key);
	if (pCache == NULL)
	{
		pCache = (pscDataCache)malloc(sizeof(scDataCache));
		memset(pCache, 0, sizeof(scDataCache));
		map_insert(&m_dataCache, key, pCache);
	}
	return pCache->data + pCache->dataLength;
}
bool TS_dataProcess::dataAnalyse(int key, int size)
{
	if (size < 1)
		return false;

	pscDataCache pCache = map_find(&m_dataCache, key);
	if (pCache == NULL)
		return false;		//no way;
	pCache->dataLength += size;

	if (pCache->dataLength < C_SCDATA_BASELEN)
		return false;

	pscData pData = (pscData)pCache->data;
	if (pData->version != C_SCDATA_VERSION)  //wrong version;
	{
		dataMng->appendError(key, E_ERR_VERSION);
		return false;
	};
	if (pData->length < 0 || pData->length > TS_DATA_MAX) //wrong data length;
	{
		pCache->dataLength = 0;
		return false;
	}
	int intPackSize = C_SCDATA_VERSION + pData->length;
	if (intPackSize > pCache->dataLength)  //wait else data;
		return false;

	dataMng->appendData(key, pCache->data, intPackSize);

	//move data to top, and calculate size;
	memcpy(pCache->data, pCache->data + intPackSize, pCache->dataLength - intPackSize);
	pCache->dataLength = pCache->dataLength - intPackSize;

	return true;
}
void TS_dataProcess::setDataManager(TS_dataManager *pdm)
{
  dataMng = pdm;
}
		




