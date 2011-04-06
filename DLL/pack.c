

#include "protocal.h"

#include <stdlib.h>
#include <string.h>

static DATAPACK         datapack;


void  * create_pack_login(void * pack)
{
        if(pack == NULL)
        {
               pack = &datapack;
        }

        memset(pack, 0, sizeof(DATAPACK));

        pack->version = CO_VERSION;
        pack->command = CO_LOGIN;
        pack->requestID = 0;
        pack->time = (time_t)0;
        pack->length = 0;

        return pack;
}


void * create_pack_heart(void * pack)
{
        if(pack == NULL)
        {
               pack = &datapack;
        }

        memset(pack, 0, sizeof(DATAPACK));

        pack->version = CO_VERSION;
        pack->command = CO_HEART;
        pack->requestID = 0;
        pack->time = (time_t)0;
        pack->length = 0;

        return pack;

}

int create_pack_chat(void ** pack, short id, char * message, int len)
{
        assert(pack != NULL);
        if(pack == NULL)
        {
                return NULL;
        }

        *pack = malloc(sizeof(DATAPACK) + len - 1);

        if(*pack == NULL)
        {
                return 0;
        }

        *pack->version = CO_VERSION;
        *pack->command = CO_CHAT;
        *pack->requestID = id;
        *pack->time = (time_t)0;
        *pack->length = len;
        memcpy(*pack->data, message, len);

        return sizeof(DATAPACK) + len - 1;

}









