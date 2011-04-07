

#include "protocal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static DATAPACK         datapack;


void  * create_pack_login(void * pack)
{
        DATAPACK *      p;

        if(pack == NULL)
        {
               pack = &datapack;
        }

        p = (DATAPACK *)pack;

        memset(pack, 0, sizeof(DATAPACK));

        p->head.version = CO_VERSION;
        p->head.command = CO_LOGIN;
        p->head.requestID = 0;
        p->head.time = (time_t)0;
        p->head.length = 0;
        return pack;
}


void * create_pack_heart(void * pack)
{
        DATAPACK *      p;

        if(pack == NULL)
        {
               pack = &datapack;
        }

        p = (DATAPACK *)pack;

        memset(pack, 0, sizeof(DATAPACK));

        p->head.version = CO_VERSION;
        p->head.command = CO_HEART;
        p->head.requestID = 0;
        p->head.time = (time_t)0;
        p->head.length = 0;

        return pack;

}

void * create_pack_chat(short id, void * message, int len, void * pack)
{
        DATAPACK *      p;

        assert(message != NULL);
        if(message == NULL)
        {
                return NULL;
        }

        if(pack == NULL)
        {
                pack = &datapack;
        }
        p = (DATAPACK *)pack;

        memset(p, 0, sizeof(DATAPACK));
        p->head.version = CO_VERSION;
        p->head.command = CO_HEART;
        p->head.requestID = 0;
        p->head.time = (time_t)0;
        p->head.length = (len<TS_DATA_MAX)?len:TS_DATA_MAX;

        memcpy(p->data, message, p->head.length);

        return p;

}


/*
int create_pack_chat(DATAPACK ** pack, short id, char * message, int len)
{
        assert(pack != NULL);
        if(pack == NULL)
        {
                return -1;
        }

        *pack = malloc(sizeof(DATAPACK) + len - 1);

        if(*pack == NULL)
        {
                return 0;
        }

        (*pack)->head.version = CO_VERSION;
        (*pack)->head.command = CO_CHAT;
        (*pack)->head.requestID = id;
        (*pack)->head.time = (time_t)0;
        (*pack)->head.length = len;
        //memcpy((*pack)->data, message, len);

        memcpy((*pack)->data, message, (len<TS_DATA_MAX)?len:TS_DATA_MAX);
        
        return sizeof(DATAPACK);

        //return sizeof(DATAPACK) + len - 1;

}

*/





