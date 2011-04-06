
#include "bool.h"
#include "net.h"
#include "pack.h"
#include "protocal.h"
#include "stack.h"
#include "clientdll.h"

#include <assert.h>
#include <pthread.h>


static SOCKET                   sock;
static stack_val                stack = NULL;

extern LPFNCALLBACK             lpfnCallBack;

static stack_ret __push_data(void * data, int len);

bool send_pack(void * data, int len)
{
        return (__push_data(data, len) == STACK_SUCCESS);

}

static stack_ret __push_data(void * data, int len)
{
        void *  tmp;

        if(stack == NULL)
        {
                stack = stack_create(sizeof(void *));
        }

        tmp = malloc(len);
        memcpy(tmp, data, len);

        return stack_push(stack, tmp);

}

static stack_ret __pop_data(void * data)
{
        return stack_pop_front(stack, data);
}


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
        char            buffer[1024];
        int             bytes_read;
        PDATAHEAD       head;

        if(FD_ISSET(sock, fdsr))
        {
                bytes_read = recv(sock, buffer, 1024, 0);
                if(bytes_read <= 0)
                {
                        // 网络中断、服务器离线等。
                        // 此处需要回调通知UI。
                        lpfnCallBack(NULL, DTYPE_DISCONNECTED, NULL, 0); 
                        pthread_exit(NULL);
                }else{
                        // 接收到数据，回调通知UI。
                        head = (PDATAHEAD)buffer;
                        if(head->version == CO_VERSION)
                        {
                                if(head->command == CO_CHAT)
                                {
                                        lpfnCallBack(NULL, DTYPE_CHATMESSAGE, buffer + sizeof(head), head->length);
                                }
                                else
                                {
                                }
                        } 
                }
        }
}

static void _send_poll(fd_set * fdsw)
{
        DATAPACK *      data;

        if(FD_ISSET(sock, fdsw))
        {
               if(__pop_data((void *)data) != STACK_EMPTY)
               {
                       send(sock, data, sizeof(DATAHEAD) + data->head.length, 0);
               } 
        }

}


static void * _start_network(void * arg)
{
        struct sockaddr_in      addr;
        fd_set                  fdsr;
        fd_set                  fdsw;
        int                     ret;
        struct timeval          tv;
        DATAPACK *              pack;


        if(!net_init())
        {
                // 回调通知UI出错。
                pthread_exit(NULL);
        }

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == INVALID_SOCKET)
        {
                pnerror("socket");
                pthread_exit(NULL);
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(_get_server_address());
        addr.sin_port = htons(_get_server_port());

        if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        {
                pnerror("connect");
                pthread_exit(NULL);
        }

        while(true)
        {
                FD_ZERO(&fdsr);
                FD_ZERO(&fdsw);
                FD_SET(sock, &fdsr);
                FD_SET(sock, &fdsw);

                tv.tv_sec = 10;
                tv.tv_usec = 0;

                ret = select((int)sock + 1, &fdsr, &fdsw, NULL, &tv);
                if(ret < 0)
                {
                        pnerror("select");
                        closesocket(sock);
                        pthread_exit(NULL);
                }
                else if(ret == 0)
                {
                        pack = create_pack_heart(NULL);
                        send_pack(pack, sizeof(DATAPACK));
                }
                else
                {
                        _recv_poll(&fdsr);
                        _send_poll(&fdsw);
                }
        }

        return NULL;
}

bool start_thread()
{
        pthread_t       pid;

        if(pthread_create(&pid, NULL, _start_network, NULL) != 0)
        {
                return false;
        }

        return true;
}








