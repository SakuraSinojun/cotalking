
#include "bool.h"
#include "clientdll.h"
#include "net.h"
#include "protocal.h"
#include "network.h"
#include "pack.h"


#include <stdlib.h>
#include <pthread.h>



LPFNCALLBACK	lpfnCallBack = NULL;
static short    uid = 0;



int userCreate(const char * userid)
{
        start_thread();
	return 0;
}

int userLogin(const char * userid, const char * password)
{
        DATAPACK        *       pack;

        pack = create_pack_login(NULL);

        return (send_pack(pack, sizeof(DATAPACK)))?1:0;

}

int userSay(const char * userid, PUSERSAY say)
{
        DATAPACK        *       pack;
        int                     len;

        len = create_pack_chat(&pack, uid, say->text, strlen(say->text));
        
        return (send_pack(pack, len))?1:0;

}

int registCallBack(LPFNCALLBACK func)
{
	if(lpfnCallBack != NULL)
	{
		return -1;
	}
	
	lpfnCallBack = func;
	
	return 0;
}




