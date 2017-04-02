// dllmain.h : Declaration of module class.

class CWordPerfectIndexerNativeModule : public ATL::CAtlDllModuleT< CWordPerfectIndexerNativeModule >
{
public :
	DECLARE_LIBID(LIBID_WordPerfectIndexerNativeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WORDPERFECTINDEXERNATIVE, "{91865800-C5AC-43A7-9A26-F60059C37D13}")
};

extern class CWordPerfectIndexerNativeModule _AtlModule;
