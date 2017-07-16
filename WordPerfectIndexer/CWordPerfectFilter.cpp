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
		EventLog(_T("WordPerfect Indexer")),
		CurrentChunkId(0)
	{}

	CComPtr<IPropertyStoreCache> Cache;
	librevenge::RVNGTextTextGenerator *Generator;
	CEventLog EventLog;

	DWORD CurrentChunkId;
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
	InitPropVariantFromString(bodyText.GetString(), &prop_var);
	priv->Cache->SetValue(PKEY_Search_Contents, prop_var);
	PropVariantClear(&prop_var);

	PropVariantInit(&prop_var);
	InitPropVariantFromInt32(bodyText.GetLength(), &prop_var);
	priv->Cache->SetValue(PKEY_Document_CharacterCount, prop_var);
	PropVariantClear(&prop_var);

	priv->CurrentChunkId = 0;

	priv->EventLog.ReportEvent(EVENTLOG_INFORMATION_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_END_IMPORT);

	return S_OK;
}

HRESULT CWordPerfectFilter::GetCount(PDWORD pCount)
{
	*pCount = 0;
	return priv->Cache->GetCount(pCount);
}

HRESULT CWordPerfectFilter::GetAt(DWORD index, PROPERTYKEY *pkey)
{
	*pkey = PKEY_Null;
	return priv->Cache->GetAt(index, pkey);
}

HRESULT CWordPerfectFilter::GetValue(REFPROPERTYKEY key, PROPVARIANT *pvar)
{
	PropVariantInit(pvar);
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

HRESULT CWordPerfectFilter::GetNextChunkValue(CChunkValue& value)
{
	constexpr DWORD ChunkId_Content = 0;
	constexpr DWORD ChunkId_CharCount = 1;

	CString num; num.Format(L"%d", priv->CurrentChunkId);
	LPCWSTR numStr = num.GetString();
	priv->EventLog.ReportEvent(EVENTLOG_INFORMATION_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_TRACE_FILTER, &numStr, 1);

	HRESULT hr = S_OK;
	if (priv->CurrentChunkId == ChunkId_Content)
	{
		PROPVARIANT pVar;
		PropVariantInit(&pVar);
		HRESULT hr = this->GetValue(PKEY_Search_Contents, &pVar);
		if (FAILED(hr)) return E_UNEXPECTED;

		hr = value.SetTextValue(PKEY_Search_Contents, pVar.pwszVal);
		PropVariantClear(&pVar);

		priv->CurrentChunkId++;
		return hr;
	}
	else if (priv->CurrentChunkId == ChunkId_CharCount)
	{
		PROPVARIANT pVar;
		PropVariantInit(&pVar);
		HRESULT hr = this->GetValue(PKEY_Document_CharacterCount, &pVar);
		if (FAILED(hr)) return E_UNEXPECTED;

		hr = value.SetIntValue(PKEY_Document_CharacterCount, pVar.intVal);
		PropVariantClear(&pVar);

		priv->CurrentChunkId++;
		return hr;
	}
	else
	{
		return FILTER_E_END_OF_CHUNKS;
	}
}

HRESULT CWordPerfectFilter::FinalConstruct()
{
	priv = new (std::nothrow) CWordPerfectFilter::Private;
	if (priv == nullptr) return E_OUTOFMEMORY;

	IPropertyStoreCache *cache;
	HRESULT hr = PSCreateMemoryPropertyStore(IID_IPropertyStoreCache, (void **) &cache);
	if (FAILED(hr)) return E_FAIL;

	priv->Cache.Attach(cache);
	return S_OK;
}

void CWordPerfectFilter::FinalRelease()
{
	if (priv->Generator != nullptr) delete priv->Generator;
	delete priv;
}
