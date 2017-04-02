/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */
#include <string.h>
#include <limits>

#include "WP6ExtendedDocumentSummaryPacket.h"
#include "libwpd_internal.h"

WP6ExtendedDocumentSummaryPacket::WP6ExtendedDocumentSummaryPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int /* id */, unsigned dataOffset, unsigned dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_dataSize(dataSize),
	m_streamData(0),
	m_stream(0)
{
	if (dataSize > 0)
		_read(input, encryption, dataOffset, dataSize);
}

WP6ExtendedDocumentSummaryPacket::~WP6ExtendedDocumentSummaryPacket()
{
	if (m_stream)
		DELETEP(m_stream);
	if (m_streamData)
		delete [] m_streamData;
}

void WP6ExtendedDocumentSummaryPacket::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	if (!m_dataSize)
		return;
	if (m_dataSize > ((std::numeric_limits<unsigned>::max)() / 2))
		m_dataSize = ((std::numeric_limits<unsigned>::max)() / 2);
	m_streamData = new unsigned char[m_dataSize];
	for (unsigned i=0; i<(unsigned)m_dataSize; i++)
		m_streamData[i] = readU8(input, encryption);

	m_stream = new WPXMemoryInputStream(m_streamData, (unsigned long)m_dataSize);
}

void WP6ExtendedDocumentSummaryPacket::parse(WP6Listener *listener) const
{
	if (!m_stream)
		return;
	unsigned short groupLength = 0;

	for (unsigned i=0; i < (unsigned)m_dataSize && !m_stream->isEnd(); i+=groupLength)
	{
		try
		{
			groupLength = readU16(m_stream, 0);
		}
		catch (FileException)
		{
			return;
		}
		if ((groupLength == 0) || m_stream->isEnd())
			return;
		unsigned short tagID = readU16(m_stream, 0);
		if (m_stream->isEnd())
			return;
		if (m_stream->seek(2, librevenge::RVNG_SEEK_CUR))
			return;

		librevenge::RVNGString name;
		unsigned short wpChar = 0;
		if (!m_stream->isEnd())
			wpChar = readU16(m_stream, 0);
		for (; wpChar != 0 && !m_stream->isEnd(); wpChar = readU16(m_stream, 0))
		{
			unsigned char character = (unsigned char)(wpChar & 0x00FF);
			unsigned char characterSet = (unsigned char)((wpChar >> 8) & 0x00FF);
			const unsigned *chars;
			int len = extendedCharacterWP6ToUCS4(character,
			                                     characterSet, &chars);
			for (int j = 0; j < len; j++)
				appendUCS4(name, chars[j]);
		}

		if (tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_CREATION_DATE ||
		        tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DATE_COMPLETED ||
		        tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_RECORDED_DATE ||
		        tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_REVISION_DATE ||
		        tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_VERSION_DATE)
		{
			try
			{
				unsigned short year = readU16(m_stream, 0);
				unsigned char month = readU8(m_stream, 0);
				unsigned char day = readU8(m_stream, 0);
				unsigned char hour = readU8(m_stream, 0);
				unsigned char minute = readU8(m_stream, 0);
				unsigned char second = readU8(m_stream, 0);
				unsigned char dayOfWeek = readU8(m_stream, 0);
				unsigned char timeZone = readU8(m_stream, 0);
				unsigned char unused = readU8(m_stream, 0);
				if (month > 0 && day > 0 && year >= 1900)
					listener->setDate(tagID, year, month, day, hour, minute, second, dayOfWeek, timeZone, unused);
			}
			catch (FileException)
			{
				return;
			}
		}
		else
		{
			librevenge::RVNGString data;
			if (!m_stream->isEnd())
				wpChar = readU16(m_stream, 0);
			for (; wpChar != 0 && !m_stream->isEnd(); wpChar = readU16(m_stream, 0))
			{
				unsigned char character = (unsigned char)(wpChar & 0x00FF);
				unsigned char characterSet = (unsigned char)((wpChar >> 8) & 0x00FF);
				const unsigned *chars;
				int len = extendedCharacterWP6ToUCS4(character,
				                                     characterSet, &chars);
				for (int j = 0; j < len; j++)
					appendUCS4(data, chars[j]);
			}
			if (data.len())
				listener->setExtendedInformation(tagID, data);
		}
		m_stream->seek((i+groupLength), librevenge::RVNG_SEEK_SET);
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
