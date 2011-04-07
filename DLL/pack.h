
#pragma once

#ifndef __PACK_H__
#define __PACK_H__

#include "protocal.h"

void  * create_pack_login(void * pack);
void * create_pack_heart(void * pack);
//int create_pack_chat(DATAPACK ** pack, short id, char * message, int len);
void * create_pack_chat(short id, void * message, int len, void * pack);

#endif


