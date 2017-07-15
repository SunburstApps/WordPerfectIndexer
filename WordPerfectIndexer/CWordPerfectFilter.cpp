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

namespace /* anonymous */
{
	PWSTR StrDupA2W(PCSTR ascii)
	{
		size_t asciilen = strlen(ascii);
		size_t widelen = MultiByteToWideChar(CP_ACP, 0, ascii, asciilen, nullptr, 0);

		PWSTR wide = (PWSTR)malloc(widelen * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, ascii, asciilen, wide, widelen);
		return wide;
	}

	bool ParseGUID(GUID& guid, LPCWSTR string)
	{
		const int expected_fields = 11;
#pragma warning(suppress: 4996) // Don't warn about CRT security deprecations
		int num_fields_parsed = swscanf(string, L"%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
			&guid.Data1, &guid.Data2, &guid.Data3,
			&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
			&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

		return num_fields_parsed == expected_fields;
	}
}

class CWordPerfectFilter::Private
{
public:
	librevenge::RVNGTextTextGenerator *Generator;
	bool CanParse = true;
	CEventLog EventLog = CEventLog(_T("WordPerfect Indexer"));
	size_t LastOffset = 0;
	CString BodyText;
};

HRESULT CWordPerfectFilter::OnInit()
{
	priv->EventLog.ReportEvent(EVENTLOG_INFORMATION_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_BEGIN_IMPORT);

	librevenge::RVNGString BodyTextRVNG;
	if (priv == nullptr) priv = new CWordPerfectFilter::Private;
	if (priv->Generator != nullptr) delete priv->Generator;
	priv->Generator = new librevenge::RVNGTextTextGenerator(BodyTextRVNG);

	STATSTG statStg;
	ZeroMemory(&statStg, sizeof(statStg));
	HRESULT hr = m_pStream->Stat(&statStg, STATFLAG_NONAME);
	if (hr != S_OK)
	{
		CString str; str.Format(L"0x%08llX", hr);
		CAtlArray<CString> args; args.Add(str);

		priv->EventLog.ReportEvent(EVENTLOG_ERROR_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_GETISTREAMLENGTH_FAILURE, args);
		return E_UNEXPECTED;
	}

	size_t StreamLength = statStg.cbSize.QuadPart;
	unsigned char *Buffer = (unsigned char *)malloc(StreamLength * sizeof(unsigned char));
	hr = m_pStream->Read(Buffer, StreamLength * sizeof(unsigned char), nullptr);

	librevenge::RVNGStringStream RevengeStream(Buffer, StreamLength * sizeof(unsigned char));
	libwpd::WPDResult wpdError = libwpd::WPDocument::parse(&RevengeStream, priv->Generator, nullptr);

	if (wpdError != libwpd::WPD_OK)
	{
		priv->EventLog.ReportEvent(EVENTLOG_ERROR_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_WPD_DOC_CORRUPT);

		switch (wpdError)
		{
		case libwpd::WPD_FILE_ACCESS_ERROR:
			return STG_E_READFAULT;
		case libwpd::WPD_PARSE_ERROR:
		case libwpd::WPD_OLE_ERROR:
			return STG_E_DOCFILECORRUPT;
		case libwpd::WPD_UNSUPPORTED_ENCRYPTION_ERROR:
			return STG_E_ACCESSDENIED;
		default:
			return E_UNEXPECTED;
		}
	}

	priv->BodyText = CA2W(BodyTextRVNG.cstr(), CP_UTF8);
	priv->LastOffset = 0;
	free(Buffer);

	return S_OK;
}

HRESULT CWordPerfectFilter::GetNextChunkValue(CChunkValue& chunkValue)
{
	constexpr size_t DefaultChunkLength = 10240;
	size_t ChunkLength = DefaultChunkLength;
	bool isLastChunk = false;

	if ((priv->LastOffset + ChunkLength) > priv->BodyText.GetLength())
	{
		ChunkLength = priv->BodyText.GetLength() - priv->LastOffset;
		isLastChunk = true;
	}

	CString Chunk = priv->BodyText.Mid(priv->LastOffset, ChunkLength);
	chunkValue.SetTextValue(PKEY_Search_Contents, Chunk, CHUNK_TEXT);
	priv->LastOffset += ChunkLength;

	if (isLastChunk)
	{
		priv->EventLog.ReportEvent(EVENTLOG_INFORMATION_TYPE, TEXT_EXTRACTION_CATEGORY, MSG_END_IMPORT);
		return FILTER_E_END_OF_CHUNKS;
	}
	else
	{
		return S_OK;
	}
}

void CWordPerfectFilter::FinalRelease()
{
	if (priv != nullptr)
	{
		delete priv->Generator;
		delete priv;
	}
}
