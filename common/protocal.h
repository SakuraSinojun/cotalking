


/*****************************************************************
 * file:        protocal.h
 * project:     grouptalk
 * author:      Naarcissus
 * desc:        protocal.
 *****************************************************************/

#pragma once

#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__


#include <time.h>


#define CO_VERSION              0x1
#define TS_DATA_MAX             255

typedef struct DATAHEAD
{
        int     version;
        short   command;
        short   requestID;
        time_t  time;
        int     length;
}DATAHEAD, *PDATAHEAD;


typedef struct DATAPACK
{
        DATAHEAD        head;
        char            data[TS_DATA_MAX];
}DATAPACK, *PDATAPACK;


enum 
{
        CO_HEART        =       0,
        CO_CHAT, 
        CO_LOGIN,
};


#endif










