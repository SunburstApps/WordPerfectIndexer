/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#include "WP42Heuristics.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

using namespace libwpd;

WPDPasswordMatch WP42Heuristics::verifyPassword(librevenge::RVNGInputStream *input, const char *password)
{
	if (!password)
		return WPD_PASSWORD_MATCH_DONTKNOW;

	input->seek(0, librevenge::RVNG_SEEK_SET);
	WPXEncryption *encryption = 0;
	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
		        readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			encryption = new WPXEncryption(password, 6);
			if (readU16(input, 0) == encryption->getCheckSum())
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_OK;
			}
			else
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_NONE;
			}
		}
		else
			return WPD_PASSWORD_MATCH_DONTKNOW;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_PASSWORD_MATCH_DONTKNOW;
	}
}

WPDConfidence WP42Heuristics::isWP42FileFormat(librevenge::RVNGInputStream *input, const char *password)
{
	input->seek(0, librevenge::RVNG_SEEK_SET);
	WPXEncryption *encryption = 0;
	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
		        readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			if (password)
			{
				encryption = new WPXEncryption(password, 6);
				if (readU16(input, 0) != encryption->getCheckSum())
				{
					delete encryption;
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
				}
			}
			else
			{
				if (readU16(input,0) != 0x0000)
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
			}
		}

		input->seek(0, librevenge::RVNG_SEEK_SET);
		if (password && encryption)
			input->seek(6, librevenge::RVNG_SEEK_SET);

		int functionGroupCount = 0;

		WPD_DEBUG_MSG(("WP42Heuristics::isWP42FileFormat()\n"));

		while (!input->isEnd())
		{
			unsigned char readVal = readU8(input, encryption);

			WPD_DEBUG_MSG(("WP42Heuristics, Offset 0x%.8x, value 0x%.2x\n", (unsigned int)(input->tell() - 1), readVal));

			if (readVal < (unsigned char)0x20)
			{
				// line breaks et al, skip
			}
			else if (readVal >= (unsigned char)0x20 && readVal <= (unsigned char)0x7F)
			{
				// normal ASCII characters, skip
			}
			else if (readVal >= (unsigned char)0x80 && readVal <= (unsigned char)0xBF)
			{
				// single character function codes, skip
				functionGroupCount++;
			}
			else if (readVal >= (unsigned char)0xFF)
			{
				// special codes that should not be found as separate functions
				if (encryption)
					delete encryption;
				return WPD_CONFIDENCE_NONE;
			}
			else
			{
				// multi character function group
				// check that the size constrains are valid, and that every group_member
				// is properly closed at the right place

				if (WP42_FUNCTION_GROUP_SIZE[readVal-0xC0] == -1)
				{
					// variable length function group

					// skip over all the bytes in the group, and scan for the closing gate
					unsigned char readNextVal = 0;
					while (!input->isEnd())
					{
						readNextVal = readU8(input, encryption);
						if (readNextVal == readVal)
							break;
					}

					// when passed the complete file, we don't allow for open groups when we've reached EOF
					if ((readNextVal == 0) || (input->isEnd() && (readNextVal != readVal)))
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					functionGroupCount++;
				}
				else
				{
					// fixed length function group

					// seek to the position where the closing gate should be
					int res = input->seek(WP42_FUNCTION_GROUP_SIZE[readVal-0xC0]-2, librevenge::RVNG_SEEK_CUR);
					// when passed the complete file, we should be able to do that
					if (res)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					// read the closing gate
					unsigned char readNextVal = readU8(input, encryption);
					if (readNextVal != readVal)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					functionGroupCount++;
				}
			}
		}

		/* When we get here, the document is in a format that we *could* import properly.
		However, if we didn't entcounter a single WP4.2 function group) we need to be more carefull:
		this would be the case when passed a plaintext file for example, which libwpd is not
		supposed to handle. */
		if (!functionGroupCount)
		{
			if (encryption)
			{
				delete encryption;
				return WPD_CONFIDENCE_EXCELLENT;
			}
			return WPD_CONFIDENCE_NONE;
		}
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_EXCELLENT;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_NONE;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
