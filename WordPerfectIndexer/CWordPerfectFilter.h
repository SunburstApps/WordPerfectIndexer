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
	public IInitializeWithStream,
	public IPropertyStore,
	public IPropertyStoreCapabilities,
	public IFilter
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

	// IFilter
	IFACEMETHODIMP Init(ULONG grfFlags, ULONG cAttributes, const FULLPROPSPEC *aAttributes, ULONG *pFlags);
	IFACEMETHODIMP GetChunk(STAT_CHUNK *pStat);
	IFACEMETHODIMP GetText(ULONG *pcwcBuffer, WCHAR *awcBuffer);
	IFACEMETHODIMP GetValue(PROPVARIANT **ppPropValue);
	IFACEMETHODIMP BindRegion(FILTERREGION, REFIID, void **)
	{
		return E_NOTIMPL;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(WordPerfectFilter), CWordPerfectFilter);
