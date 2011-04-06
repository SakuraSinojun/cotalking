
#include "../DLL/clientdll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int dll_callback(const char * userid, DATATYPE type, void * data, int len)
{

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

        userCreate(urid);
        userLogin(urid, pswd);

        registCallBack(dll_callback);
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



