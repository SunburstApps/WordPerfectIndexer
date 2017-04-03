// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "WordPerfectIndexer_i.h"
#include "dllmain.h"

CWordPerfectIndexerModule _AtlModule;
HINSTANCE _AtlModuleInstance;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	_AtlModuleInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
