/***************************************************
Project: grouptalk
author: TaXueWuHen
public
Notice: you must definition in .h if you use the template and compile with g++
***************************************************/

#ifndef __TALK_PUBLICOPT_H__
#define __TALK_PUBLICOPT_H__

#include <stdio.h>
#include <map>

using namespace std;

template <typename T1, typename T2>
T2 map_find(map<T1, T2> *pMap, T1 key)
{
	if (!pMap)
		return NULL;
	typename map<T1, T2>::iterator it;		//compile with c++ must have "typename"; :(
	it = (*pMap).find(key);
	if (it == (*pMap).end())
		return NULL;

	return it->second;
}

template <typename T1, typename T2>
void map_insert(map<T1, T2> *pMap, T1 key, T2 value)
{
	if (!pMap)
		return;
	(*pMap).insert(make_pair(key, value));
}





#endif

