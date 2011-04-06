#pragma once
#include "stdafx.h"
#include <atltime.h>	// CTime class
#include <vector>

class CChatRecord
{
public:
	int ID;
	CString NickName;
	CTime TimeStamp;
	CString Message;

public:
	CString GetTitleText()
	{
		CString title;
		title.Format(_T("%s(%9d) %2d:%2d:%2d"),NickName,ID,
			TimeStamp.GetHour(),TimeStamp.GetMinute(),TimeStamp.GetSecond());
		return title;
	}
};

typedef std::vector<CChatRecord> CChatRecordList;