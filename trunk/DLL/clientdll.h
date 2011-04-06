
/*******************************************************************************
	
        File:   ClientDll.h
        Tab:    8 space.

*******************************************************************************/

#pragma once

#ifndef __CLIENTDLL_H__
#define __CLIENTDLL_H__

#ifdef _WIN32
# ifdef  __cplusplus
#  define DLL_EXPORTS     extern "C" __declspec(dllexport)
# else
#  define DLL_EXPORTS     __declspec(dllexport)
# endif
# define DLL_API         __cdecl
#else
# define DLL_EXPORTS
# define DLL_API
#endif



#include "bool.h"

typedef struct tagUSERSAYSTRUCT
{
        const char *    text;
        bool            bPrivateMessage;
        const char *    strReceiver;
}USERSAY, *PUSERSAY;

typedef enum DATATYPE
{
        DTYPE_CHATMESSAGE,
        DTYPE_SETTINGS,
        //...
}DATATYPE;

typedef int (DLL_API * LPFNCALLBACK)(const char * userid, DATATYPE type, void * data, int len);

DLL_EXPORTS int DLL_API userCreate(const char * userid);
DLL_EXPORTS int DLL_API userLogin(const char * userid, const char * password);
DLL_EXPORTS int DLL_API userSay(const char * userid, PUSERSAY say);
DLL_EXPORTS int DLL_API registCallBack(LPFNCALLBACK func);


#endif






