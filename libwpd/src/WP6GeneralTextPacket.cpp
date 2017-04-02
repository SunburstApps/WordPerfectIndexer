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
#include <vector>

#include "WP6GeneralTextPacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6GeneralTextPacket::WP6GeneralTextPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int /* id */, unsigned dataOffset, unsigned dataSize):
	WP6PrefixDataPacket(input, encryption),
	m_subDocument(0),
	m_streamData(0)
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6GeneralTextPacket::~WP6GeneralTextPacket()
{
	if (m_subDocument)
		delete m_subDocument;
	if (m_streamData)
		delete [] m_streamData;
}

void WP6GeneralTextPacket::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();
	unsigned short numTextBlocks = readU16(input, encryption);
	input->seek(4, librevenge::RVNG_SEEK_CUR);

	if (numTextBlocks < 1)
	{
		WPD_DEBUG_MSG(("WordPerfect: Number of text blocks is %i\n", numTextBlocks));
		return; // m_subDocument will be 0
	}

	std::vector<unsigned> blockSizes(numTextBlocks);
	unsigned totalSize = 0;
	unsigned i;

	for (i=0; i<numTextBlocks; i++)
	{
		if ((input->tell() - startPosition + 4) < 0)
			throw FileException();
		if ((unsigned long)(input->tell() - startPosition + 4) > (unsigned long)getDataSize() || input->isEnd())
			throw FileException();
		blockSizes[i] = readU32(input, encryption);
		unsigned int newTotalSize = totalSize + blockSizes[i];
		if (newTotalSize < totalSize)
			throw FileException();
		totalSize = newTotalSize;
	}

	if (!totalSize)
	{
		WPD_DEBUG_MSG(("WordPerfect: The total size of the text is %ui\n", totalSize));
		return; // m_subDocument will be 0
	}
	m_streamData = new unsigned char[totalSize];
	unsigned streamPos = 0;
	for (i=0; i<numTextBlocks; i++)
	{
		if ((input->tell() - startPosition + blockSizes[i]) > getDataSize() || input->isEnd())
			throw FileException();
		for (unsigned int j=0; j<blockSizes[i]; j++)
		{
			m_streamData[streamPos] = readU8(input, encryption);
			streamPos++;
		}
	}

	if (totalSize)
		m_subDocument = new WP6SubDocument(m_streamData, totalSize);
}

void WP6GeneralTextPacket::parse(WP6Listener *listener) const
{
	if (m_subDocument)
		m_subDocument->parse(listener);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
