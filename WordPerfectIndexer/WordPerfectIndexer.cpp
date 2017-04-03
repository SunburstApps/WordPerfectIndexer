#include "stdafx.h"
#include "resource.h"
#include "WordPerfectIndexer_i.h"
#include "dllmain.h"
#include <propsys.h>
#include <atlpath.h>

using namespace ATL;
extern HINSTANCE _AtlModuleInstance;

STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

_Check_return_ STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
	return _AtlModule.DllRegisterServer();
}

STDAPI DllUnregisterServer(void)
{
	return _AtlModule.DllUnregisterServer();
}

STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;

#if IS_MASTER_INDEXER_LIBRARY
	CPath PropdescPath;
	WCHAR PathBuffer[MAX_PATH + 1];
	ZeroMemory(PathBuffer, (MAX_PATH + 1) * sizeof(WCHAR));
	GetModuleFileName(_AtlModuleInstance, PathBuffer, MAX_PATH + 1);
	PropdescPath = PathBuffer;

	PropdescPath.RemoveFileSpec();
	PropdescPath.AddBackslash();
	PropdescPath.Append(L"WordPerfectIndexer.propdesc");
#endif

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}

#ifdef IS_MASTER_INDEXER_LIBRARY
		if (SUCCEEDED(hr)) ::PSRegisterPropertySchema(PropdescPath);
#endif
	}
	else
	{
		hr = DllUnregisterServer();

#if IS_MASTER_INDEXER_LIBRARY
		if (SUCCEEDED(hr)) ::PSUnregisterPropertySchema(PropdescPath);
#endif
	}

	return hr;
}
