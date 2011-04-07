
#include "../DLL/clientdll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int dll_callback(const char * userid, DATATYPE type, void * data, int len)
{

        switch(type)
        {
        case DTYPE_CHATMESSAGE:
                printf("DTYPE_CHATMESSAGE\n");
                //printf("%s\n", (char *)data);
                break;
        case DTYPE_DISCONNECTED:
                printf("disconnected\n");
                exit(0);
                break;
        default:
                break;
        }

        return 0;
}

int main()
{

        char            temp[1024];
        char            urid[1024];
        char            pswd[1024];
        USERSAY         usersay;


        printf("Input User Name:");
        if(fgets(urid, 1024, stdin) == NULL)
        {
                perror("fgets:");
                return -1;
        }

        printf("Input Password:");
        if(fgets(pswd, 1024, stdin) == NULL)
        {
                perror("fgets:");
                return -1;
        }
        
        urid[strlen(urid)-1] = '\0';
        pswd[strlen(pswd)-1] = '\0';

        printf("before regist\n");
        registCallBack(dll_callback);

        printf("before create\n");
        userCreate(urid);
        
        printf("before login\n");
        userLogin(urid, pswd);

        while ( fgets(temp, 1024, stdin) != NULL )
        {
                temp[strlen(temp)-1] = '\0';
                if(strcmp(temp, "exit") == 0 ||
                        strcmp(temp, "quit") == 0 ||
                        strcmp(temp, "logout") == 0)
                {
                        break;
                }
                usersay.text = temp;
                usersay.bPrivateMessage = false;
                usersay.strReceiver = NULL;
                userSay(urid, &usersay); 
        }

        return 0;

}



