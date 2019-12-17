#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <atlcoll.h>
#include <cassert>

class CEventLog
{
private:
	HANDLE hEventLog;

public:
	CEventLog(LPTSTR ProviderName)
	{
		hEventLog = RegisterEventSource(nullptr, ProviderName);
	}

	~CEventLog()
	{
		if (hEventLog != nullptr) DeregisterEventSource(hEventLog);
	}

	BOOL ReportEvent(WORD type, WORD category, DWORD eventId)
	{
		if (hEventLog == nullptr) return false;
		return ::ReportEvent(hEventLog, type, category, eventId, nullptr, 0, 0, nullptr, nullptr);
	}

	template<WORD stringCount>
	BOOL ReportEvent(WORD type, WORD category, DWORD eventId, LPCTSTR (&insertStrings)[stringCount])
	{
		if (hEventLog == nullptr) return false;
		return ::ReportEvent(hEventLog, type, category, eventId, nullptr, stringCount, 0, (LPCTSTR *)insertStrings, nullptr);
	}
};
