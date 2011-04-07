
#include "bool.h"
#include "net.h"
#include "pack.h"
#include "protocal.h"
#include "clientdll.h"

#include <assert.h>
#include <pthread.h>


static SOCKET                   sock;
extern LPFNCALLBACK             lpfnCallBack;

typedef struct data_link
{
        void *                  data;
        int                     len;
        struct data_link *      prev;
        struct data_link *      next;
}data_link;

static data_link *      head = NULL;
static data_link *      tail = NULL;


static bool __push_data(void * data, int len);


bool send_pack(void * data, int len)
{
        return __push_data(data, len); 

}

static bool __push_data(void * data, int len)
{
        data_link *     temp;

        temp = (data_link *)malloc(sizeof(data_link));
        if(temp == NULL)
        {
                return false;
        }
        
        temp->data = malloc(len);
        if(temp->data == NULL)
        {
                return false;
        }

        memcpy(temp->data, data, len);
        temp->len = len;
        temp->prev = NULL;
        temp->next = NULL;

        if(head == NULL)
        {
                head = temp;
        }else{
                temp->prev = tail;
                tail->next = temp;
        }
        tail = temp;

        return true;
}

static int __pop_data(void * data)
{
        int             len;
        data_link *     temp;

        if(data == NULL)
        {
                if(head == NULL)
                {
                        return 0;
                }else{
                        return head->len;
                }
        }

        if(head == NULL)
        {
                return 0;
        }

        temp = head->next;
        if(tail == head)
        {
                tail = temp;
        }

        len = head->len;
        memcpy(data, head->data, head->len);
        free(head->data);
        free(head);
        
        head = temp;
        
        return len;

}


static unsigned short int _get_server_port()
{
        return 9500;
}

static const char * _get_server_address()
{
        return "127.0.0.1";
}

static void _recv_poll(fd_set * fdsr)
{
        char            buffer[1024];
        int             bytes_read;
        PDATAHEAD       datahead = NULL;

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
                        datahead = (PDATAHEAD)buffer;
                        if(datahead->version == CO_VERSION)
                        {
                                if(datahead->command == CO_CHAT)
                                {
                                        lpfnCallBack(NULL, DTYPE_CHATMESSAGE, buffer + sizeof(datahead), datahead->length);
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
        DATAPACK *      data = NULL;
        int             len;

        printf("into _send_poll()\n");

        if(FD_ISSET(sock, fdsw))
        {
               if( (len = __pop_data(NULL)) > 0 )
               {
                       data = (DATAPACK *)malloc(len);
                       len = __pop_data((void *)data);
                       send(sock, (void *)data, len, 0);
                       free(data);
               } 
        }

        printf("leave _send_poll()\n");
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








