#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <propvarutil.h>
#include "WordPerfectIndexer_i.h"
#include "FilterBase.h"

using namespace ATL;

class ATL_NO_VTABLE CWordPerfectFilter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWordPerfectFilter, &CLSID_WordPerfectFilter>,
	public IPropertyStore,
	public IPropertyStoreCapabilities,
	public CFilterBase
{
private:
	class Private;
	Private *priv;

public:
	DECLARE_REGISTRY_RESOURCEID(IDR_WORDPERFECTINDEXER)
	DECLARE_NOT_AGGREGATABLE(CWordPerfectFilter)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CWordPerfectFilter)
		COM_INTERFACE_ENTRY(IInitializeWithStream)
		COM_INTERFACE_ENTRY(IPropertyStore)
		COM_INTERFACE_ENTRY(IPropertyStoreCapabilities)
		COM_INTERFACE_ENTRY(IFilter)
	END_COM_MAP()

	HRESULT FinalConstruct();
	void FinalRelease();

	// IPropertyStore
	IFACEMETHODIMP GetCount(PDWORD);
	IFACEMETHODIMP GetAt(DWORD, PROPERTYKEY *);
	IFACEMETHODIMP GetValue(REFPROPERTYKEY, PROPVARIANT *);
	IFACEMETHODIMP SetValue(REFPROPERTYKEY, REFPROPVARIANT);
	IFACEMETHODIMP Commit();

	// IPropertyStoreCapabilities
	IFACEMETHODIMP IsPropertyWritable(REFPROPERTYKEY);

	// CFilterBase
	HRESULT GetNextChunkValue(CChunkValue&);
	HRESULT OnInit(void);
};

OBJECT_ENTRY_AUTO(CLSID_WordPerfectFilter, CWordPerfectFilter);
