#include "stdafx.h"
#include "CWordPerfectFilter.h"
#include <librevenge/librevenge.h>
#include <librevenge-generators/librevenge-generators.h>
#include <librevenge-stream/librevenge-stream.h>
#include <libwpd/libwpd.h>
#include <stdio.h>

#define strequal(a, b) (strcmp(a,b) == 0)


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
	if (wpdError != libwpd::WPD_OK)
	{
		DeleteFileA(priv->TempFilePath);

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
	return E_NOTIMPL;
}
