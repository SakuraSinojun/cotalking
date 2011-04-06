
#pragma once

#ifndef __PACK_H__
#define __PACK_H__

void  * create_pack_login(void * pack);
void * create_pack_heart(void * pack);
int create_pack_chat(void ** pack, short id, char * message, int len);

#endif


