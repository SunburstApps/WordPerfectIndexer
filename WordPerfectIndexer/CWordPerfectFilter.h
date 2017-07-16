#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <propsys.h>
#include <propkey.h>
#include <propvarutil.h>
#include "WordPerfectIndexer_i.h"

using namespace ATL;

class ATL_NO_VTABLE CWordPerfectFilter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWordPerfectFilter, &CLSID_WordPerfectFilter>,
	public IInitializeWithStream,
	public IPropertyStore,
	public IPropertyStoreCapabilities
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
	END_COM_MAP()

	HRESULT FinalConstruct();
	void FinalRelease();

	IFACEMETHODIMP GetClassID(CLSID* pClassID)
	{
		*pClassID = CLSID_WordPerfectFilter;
		return S_OK;
	}

	// IInitializeWithStream
	IFACEMETHODIMP Initialize(IStream *stream, DWORD);

	// IPropertyStore
	IFACEMETHODIMP GetCount(PDWORD);
	IFACEMETHODIMP GetAt(DWORD, PROPERTYKEY *);
	IFACEMETHODIMP GetValue(REFPROPERTYKEY, PROPVARIANT *);
	IFACEMETHODIMP SetValue(REFPROPERTYKEY, REFPROPVARIANT);
	IFACEMETHODIMP Commit();

	// IPropertyStoreCapabilities
	IFACEMETHODIMP IsPropertyWritable(REFPROPERTYKEY);
};

OBJECT_ENTRY_AUTO(__uuidof(WordPerfectFilter), CWordPerfectFilter);
