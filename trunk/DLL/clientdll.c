
#include "bool.h"
#include "clientdll.h"
#include "network.h"
#include "../../common/defGroupTalk.h"

#include <stdlib.h>
#include <pthread.h>



LPFNCALLBACK	lpfnCallBack = NULL;

int userCreate(const char * userid)
{
	return 0;
}

int userLogin(const char * userid, const char * password)
{
	return 0;
}

int userSay(const char * userid, PUSERSAY say)
{
	return 0;
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                      
//                 也许这些应该拿到另外的文件里去？
//

static SOCKET   sock;

static unsigned short int _get_server_port()
{
        return 10234;
}

static const char * _get_server_address()
{
        return "127.0.0.1";
}

static void _recv_poll(fd_set * fdsr)
{
        char    buffer[1024];
        int     bytes_read;
        
        if(FD_ISSET(sock, fdsr))
        {
                bytes_read = recv(sock, buffer, 1024, 0);
                if(bytes_read <= 0)
                {
                        // 网络中断、服务器离线等。
                        // 此处需要回调通知UI。
                }else{
                        // 接收到数据，回调通知UI。
                }
        }
}

static void _send_poll(fd_set * fdsw)
{
}


static void * _start_network(void * arg)
{
        struct sockaddr_in      addr;
        fd_set                  fdsr;
        fd_set                  fdsw;
        int                     ret;
        int                     maxfd;
        struct timeval          tv;
        
        if(!network_init())
        {
                // 回调通知UI出错。
                pthread_exit(NULL);
        }
                
        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == INVALID_SOCKET);
        {
                // 回调通知UI出错。
                pthread_exit(NULL);               
        }
        
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_get_server_port());
        addr.sin_addr.s_addr = inet_addr(_get_server_address());
        
        
        if( connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0 )
        {
                // 回调通知UI出错。
                pthread_exit(NULL);
         }
        
        maxfd = sock;
        
        
        // select模型预留给文件传输、视频、语音等功能。
        while(1)
        {
                FD_ZERO(&fdsr);
                FD_ZERO(&fdsw);
                FD_SET(sock, &fdsr);
                FD_SET(sock, &fdsw);
                
                tv.tv_sec = 5;
                tv.tv_usec = 0;
                
                ret = select(maxfd, &fdsr, &fdsw, NULL, &tv);
                switch(ret)
                {
                case 0:
                        //timeout:
                        break;
                case -1:
                        //error:
                        break;
                default:
                        _recv_poll(&fdsr);
                        _send_poll(&fdsw);                      
                        break;
                }
        }
        
}


static bool _start_thread()
{
        pthread_t       tid;
        
        pthread_create(&tid, NULL, _start_network, NULL);
        
}













