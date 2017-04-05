#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cassert>

class CEventLog
{
private:
	HANDLE hEventLog;

public:
	CEventLog(LPTSTR ProviderName)
	{
		hEventLog = RegisterEventSource(nullptr, ProviderName);
		assert(hEventLog != nullptr && "RegisterEventSource failed");
	}

	~CEventLog()
	{
		DeregisterEventSource(hEventLog);
	}

	BOOL ReportEvent(WORD type, WORD category, DWORD eventId)
	{
		return this->ReportEvent(type, category, eventId, nullptr, 0);
	}

	BOOL ReportEvent(WORD type, WORD category, DWORD eventId, LPCTSTR insertStrings[], WORD stringCount)
	{
		return ::ReportEvent(hEventLog, type, category, eventId, nullptr, stringCount, 0, insertStrings, nullptr);
	}

	template<WORD Count>
	BOOL ReportEvent(WORD type, WORD category, DWORD eventId, LPCTSTR insertStrings[Count])
	{
		return this->ReportEvent(type, category, eventId, insertStrings, Count);
		wcscpy_s
	}
};
