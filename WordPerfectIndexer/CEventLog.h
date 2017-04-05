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

	BOOL ReportEvent(WORD type, WORD category, DWORD eventId, const ATL::CAtlArray<ATL::CString>& insertStrings)
	{
		ATL::CAtlArray<LPCTSTR> StringLiterals;
		const size_t limit = insertStrings.GetCount();
		for (size_t i = 0; i < limit; i++) StringLiterals.Add(insertStrings[i].GetString());

		return this->ReportEvent(type, category, eventId, StringLiterals.GetData(), (WORD)StringLiterals.GetCount());
	}
};
