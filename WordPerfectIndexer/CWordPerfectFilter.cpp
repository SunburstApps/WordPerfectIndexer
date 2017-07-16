#include "stdafx.h"
#include "CEventLog.h"
#include "CWordPerfectFilter.h"
#include "LogMessages.h"
#include <librevenge/librevenge.h>
#include <librevenge-generators/librevenge-generators.h>
#include <librevenge-stream/librevenge-stream.h>
#include <libwpd/libwpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

namespace /* anonymous */
{
	template<class T>
	class MallocDeleter
	{
	public:
		void operator () (T* ptr) {
			free((void *) ptr);
		}
	};
}

class CWordPerfectFilter::Private
{
public:
	CWordPerfectFilter::Private() :
		Generator(nullptr),
		EventLog(_T("WordPerfect Indexer"))
	{}

	CComPtr<IPropertyStoreCache> Cache;
	librevenge::RVNGTextTextGenerator *Generator;
	CEventLog EventLog;
};

HRESULT CWordPerfectFilter::Initialize(IStream *raw_stream, DWORD)
{
	priv->EventLog.ReportEvent(EVENTLOG_INFORMATION_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_BEGIN_IMPORT);

	HRESULT hr = S_OK;
	CComPtr<IStream> stream(raw_stream);

	librevenge::RVNGString rvngBodyText;
	if (priv->Generator != nullptr) delete priv->Generator;
	priv->Generator = new (std::nothrow) librevenge::RVNGTextTextGenerator(rvngBodyText);
	if (priv->Generator == nullptr) return E_OUTOFMEMORY;

	size_t StreamLength;
	do {
		STATSTG stg;
		ZeroMemory(&stg, sizeof(stg));
		hr = stream->Stat(&stg, STATFLAG_NONAME);
		if (FAILED(hr)) {
			CString str; str.Format(L"0x%08llX", hr);
			LPCWSTR raw_str = str.GetString();
			priv->EventLog.ReportEvent(EVENTLOG_ERROR_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_GETISTREAMLENGTH_FAILURE, &raw_str, 1);
			return E_UNEXPECTED;
		}

		StreamLength = (size_t)stg.cbSize.QuadPart;
	} while (0);

	std::unique_ptr<unsigned char, MallocDeleter<unsigned char>> Buffer((unsigned char *)calloc(StreamLength, sizeof(unsigned char)));
	do {
		hr = stream->Read(Buffer.get(), StreamLength, nullptr);
		if (FAILED(hr)) {
			CString str; str.Format(L"0x%08llX", hr);
			LPCWSTR raw_str = str.GetString();
			priv->EventLog.ReportEvent(EVENTLOG_ERROR_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_STREAM_READ_FAILURE, &raw_str, 1);
			return hr;
		}
	} while (0);

	librevenge::RVNGStringStream rvngStream(Buffer.get(), StreamLength * sizeof(unsigned char));
	libwpd::WPDResult wpdError = libwpd::WPDocument::parse(&rvngStream, priv->Generator, nullptr);

	if (wpdError != libwpd::WPD_OK)
	{
		priv->EventLog.ReportEvent(EVENTLOG_ERROR_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_WPD_DOC_CORRUPT);

		if (wpdError == libwpd::WPD_FILE_ACCESS_ERROR) return STG_E_READFAULT;
		else if (wpdError == libwpd::WPD_PARSE_ERROR || wpdError == libwpd::WPD_OLE_ERROR) return STG_E_DOCFILECORRUPT;
		else if (wpdError == libwpd::WPD_UNSUPPORTED_ENCRYPTION_ERROR) return STG_E_ACCESSDENIED;
		else return E_UNEXPECTED;
	}

	CString bodyText = CA2W(rvngBodyText.cstr(), CP_UTF8);
	PROPVARIANT prop_var;

	PropVariantInit(&prop_var);
	prop_var.vt = VT_LPWSTR;
	prop_var.pwszVal = const_cast<LPWSTR>(bodyText.GetString());
	priv->Cache->SetValueAndState(PKEY_Search_Contents, &prop_var, PSC_NORMAL);

	PropVariantInit(&prop_var);
	prop_var.vt = VT_I4;
	prop_var.intVal = bodyText.GetLength();
	priv->Cache->SetValueAndState(PKEY_Document_CharacterCount, &prop_var, PSC_NORMAL);

	return S_OK;
}

HRESULT CWordPerfectFilter::GetCount(PDWORD pCount)
{
	if (pCount == nullptr) return E_INVALIDARG;

	*pCount = 0;
	if (priv == nullptr || !priv->Cache) return E_UNEXPECTED;
	return priv->Cache->GetCount(pCount);
}

HRESULT CWordPerfectFilter::GetAt(DWORD index, PROPERTYKEY *pkey)
{
	if (pkey == nullptr) return E_INVALIDARG;

	*pkey = PKEY_Null;
	if (priv == nullptr || !priv->Cache) return E_UNEXPECTED;
	return priv->Cache->GetAt(index, pkey);
}

HRESULT CWordPerfectFilter::GetValue(REFPROPERTYKEY key, PROPVARIANT *pvar)
{
	if (pvar == nullptr) return E_INVALIDARG;

	PropVariantInit(pvar);
	if (priv == nullptr || !priv->Cache) return E_UNEXPECTED;
	return priv->Cache->GetValue(key, pvar);
}

HRESULT CWordPerfectFilter::SetValue(REFPROPERTYKEY, REFPROPVARIANT)
{
	// This is a read-only property source.
	return E_ACCESSDENIED;
}

HRESULT CWordPerfectFilter::Commit()
{
	return E_NOTIMPL;
}

HRESULT CWordPerfectFilter::IsPropertyWritable(REFPROPERTYKEY)
{
	// This is a read-only property source.
	return S_FALSE;
}

HRESULT CWordPerfectFilter::FinalConstruct()
{
	if (priv == nullptr) priv = new (std::nothrow) CWordPerfectFilter::Private;
	if (priv == nullptr) return E_OUTOFMEMORY;

	HRESULT hr = PSCreateMemoryPropertyStore(IID_PPV_ARGS(&priv->Cache));
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

void CWordPerfectFilter::FinalRelease()
{
	if (priv != nullptr)
	{
		if (priv->Generator != nullptr) delete priv->Generator;
		delete priv;
	}
}
