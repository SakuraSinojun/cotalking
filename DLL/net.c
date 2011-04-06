

#include "network.h"

const char * net_error()
{
        char * buffer;
        
#ifndef _WIN32
        buffer = strerror(errno);
#else
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
                      NULL, 
                      WSAGetLastError(), 
                      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
                      (PSTR)&buffer, 0, NULL);
#endif
        return buffer;
    
}

int net_init()
{
#ifdef _WIN32

        WSADATA wsaData;
        WORD    version = MAKEWORD(2, 0);
        int     ret = WSAStartup(version, &wsaData);
        if(ret)
        {
                net_error("WSAStartup");
                return 0;
        }
#endif
        return 1;
}

void pnerror(char * str)
{
        printf("%s:%s\n", str, net_error());
}







