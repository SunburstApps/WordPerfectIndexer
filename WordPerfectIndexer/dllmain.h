// dllmain.h : Declaration of module class.

class CWordPerfectIndexerModule : public ATL::CAtlDllModuleT< CWordPerfectIndexerModule >
{
public :
	DECLARE_LIBID(LIBID_WordPerfectIndexerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WORDPERFECTINDEXER, "{FD5BB05E-F376-40DC-A13A-5C0A03F5D729}")
};

extern class CWordPerfectIndexerModule _AtlModule;
