
#pragma once
#ifndef __LOCAL_H__
#define __LOCAL_H__


typedef enum 
{
        LA_ZERO         = 0,
        LA_LOGACK,
        LA_RIGACK,
        
        LA_CHAT,
        
        // ˽��
        LA_USERLIST,
        LA_FRIENDLIST,
        LA_ADDREQ,
        LA_ADDACK,
        LA_DELACK,
        LA_USERINFO,
        
        LA_USERLOGOFF,
        
        //Ⱥ��
        LA_CREATEGROUPACK,
        LA_JOINGROUPACK,
        LA_GROUPLIST,
        LA_GROUPINFO,
        LA_GROUPUSERLIST,
        
}LOCALACTION;


#pragma pack(push)
#pragma pack(1)

typedef struct
{
        int     ack_state;
        int     msglen;
        char    message[1];     //ֻ��char����ʾ���ȣ������wchar�ͣ�ֻҪǿ��ת��һ�¡�
}COMMONACK;

typedef struct
{
        unsigned int    uid;    //��Դuser������Դgroup��ֻ��id..��ϸ��Ϣ
        unsigned int    gid;
}CHATMESSAGE;


typedef struct
{
        unsigned int    pid;
        LOCALACTION     action;
        unsigned int    data_size;
        union
        {
                COMMONACK       ack;
                CHATMESSAGE     chat;
        };
}LMESSAGE;

#pragma pack(pop)


#define ACK_SUCCESS     0
#define ACK_FAILURE     1



#endif

