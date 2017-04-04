#include "stdafx.h"
#include "CWordPerfectFilter.h"
#include <librevenge/librevenge.h>
#include <librevenge-generators/librevenge-generators.h>
#include <librevenge-stream/librevenge-stream.h>
#include <libwpd/libwpd.h>
#include <stdio.h>
#include <stdlib.h>

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
}

class CWordPerfectFilter::Private
{
public:
	char TempFilePath[MAX_PATH + 1];
	librevenge::RVNGTextTextGenerator *Generator;
	librevenge::RVNGString BodyText;
	bool CanParse = true;
};

HRESULT CWordPerfectFilter::OnInit()
{
	priv = new CWordPerfectFilter::Private;
	priv->Generator = new librevenge::RVNGTextTextGenerator(priv->BodyText);

	char TempPath[MAX_PATH + 1];
	if (GetTempPathA(MAX_PATH + 1, TempPath) == 0)
		return E_UNEXPECTED;

	GetTempFileNameA(TempPath, "PFF", 0, priv->TempFilePath);

	FILE *TempFile = fopen(TempPath, "w");
	if (TempFile == nullptr) return E_UNEXPECTED;

	const ULONG BufferSize = 4096;
	ULONG BytesRead = BufferSize;
	BYTE ReadBuffer[BufferSize];

	HRESULT hr;
	do
	{
		hr = m_pStream->Read(ReadBuffer, BufferSize, &BytesRead);
		fwrite(ReadBuffer, 1, BytesRead, TempFile);
	} while (hr == S_OK);

	fclose(TempFile);
	if (hr != S_FALSE)
	{
		// S_FALSE is returned on end-of-stream.
		// S_OK is returned when there is more data.
		// Any other HRESULT is an error.

		DeleteFileA(priv->TempFilePath);
		return hr;
	}

	librevenge::RVNGFileStream RevengeStream(priv->TempFilePath);
	libwpd::WPDResult wpdError = libwpd::WPDocument::parse(&RevengeStream, priv->Generator, nullptr);
	DeleteFileA(priv->TempFilePath);

	if (wpdError != libwpd::WPD_OK)
	{
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

	return S_OK;
}

HRESULT CWordPerfectFilter::GetNextChunkValue(CChunkValue& chunkValue)
{
	const GUID WordPerfectFilterGuid = { 0x64F0A51B, 0xF686, 0x4EC2, 0xBE, 0xBC, 0x65, 0x4C, 0x17, 0x4E, 0x6E, 0x73 };
	PROPERTYKEY BodyTextPropKey = { WordPerfectFilterGuid, 100 };

	PWSTR BodyText = StrDupA2W(priv->BodyText.cstr());

	const size_t MaxColumnLength = 1048576;
	WCHAR TruncatedBodyText[MaxColumnLength / sizeof(WCHAR)];
	if (wcslen(BodyText) > MaxColumnLength)
	{
		wcsncpy_s(TruncatedBodyText, BodyText, MaxColumnLength - 3);
		wcsncat_s(TruncatedBodyText, L"...", 3);
	}

	chunkValue.SetTextValue(BodyTextPropKey, TruncatedBodyText);
	free((void *)BodyText);

	return FILTER_E_END_OF_CHUNKS;
}

CWordPerfectFilter::~CWordPerfectFilter()
{
	if (priv != nullptr)
	{
		delete priv->Generator;
		delete priv;
	}
}
