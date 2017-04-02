#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlcoll.h>
#include "WordPerfectIndexer_i.h"
#include "FilterBase.h"

using namespace ATL;

class ATL_NO_VTABLE CWordPerfectFilter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWordPerfectFilter, &CLSID_WordPerfectFilter>,
	public CFilterBase
{
public:
	CWordPerfectFilter() {}

	DECLARE_REGISTRY_RESOURCEID(IDR_WORDPERFECTINDEXER)
	DECLARE_NOT_AGGREGATABLE(CWordPerfectFilter)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CWordPerfectFilter)
		COM_INTERFACE_ENTRY(IInitializeWithStream)
		COM_INTERFACE_ENTRY(IFilter)
	END_COM_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	IFACEMETHODIMP GetClassID(CLSID* pClassID)
	{
		*pClassID = CLSID_WordPerfectFilter;
		return S_OK;
	}

	virtual HRESULT OnInit();
	virtual HRESULT GetNextChunkValue(CChunkValue &chunkValue);
};

OBJECT_ENTRY_AUTO(__uuidof(WordPerfectFilter), CWordPerfectFilter);
