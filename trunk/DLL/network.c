
#include "bool.h"
#include "net.h"

static SOCKET   sock;

static data_link * __add_data(void * data, int len);


bool send(void * data, int len)
{
        return (__add_data(data, len) != NULL );

}


struct data_link
{
        void *                  data;
        int                     len;

        struct data_link *      prev;
        struct data_link *      next;
};

static data_link * head = NULL;
static data_link * tail = NULL;

static data_link * __add_data(void * data, int len)
{
        data_link *     temp;

        assert(data != NULL);
        if(data == NULL)
        {
                return NULL;
        }

        temp = malloc(sizeof(data_link));
        temp->data = malloc(len);
        temp->prev = NULL;
        temp->next = NULL;
        memcpy(temp->data, data, len);

        
        if(head == NULL)
        {
                head = temp;
        }else{
                tail->next = temp;
                temp->prev = tail;
        }
        tail = temp;
        
        return temp;

}

static bool __remove_data(data_link * link)
{
        data_link *     prev;
        data_link *     next;

        assert(link != NULL);
        if(link == NULL)
        {
                return false;
        }

        prev = link->prev;
        next = link->next;

        if(prev == NULL)
        {
                head = next; 
        }else{
                prev->next = next;
        }

        if(next == NULL)
        {
                tail = prev;
        }else{
                next->prev = prev;
        }
        
        free(link->data);
        free(link);
        
        return true;
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
        
        if(!net_init())
        {
                // 回调通知UI出错。
                pthread_exit(NULL);
        }
                
