/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
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

#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include <librevenge-stream/librevenge-stream.h>

class WPXMemoryInputStream : public librevenge::RVNGInputStream
{
public:
	WPXMemoryInputStream(unsigned char *data, unsigned long size);
	~WPXMemoryInputStream();
	bool isStructured()
	{
		return false;
	}
	unsigned subStreamCount()
	{
		return 0;
	}
	const char *subStreamName(unsigned)
	{
		return 0;
	}
	bool existsSubStream(const char *)
	{
		return false;
	}
	librevenge::RVNGInputStream *getSubStreamByName(const char *)
	{
		return 0;
	}
	librevenge::RVNGInputStream *getSubStreamById(unsigned)
	{
		return 0;
	}
	const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	int seek(long offset, librevenge::RVNG_SEEK_TYPE seekType);
	long tell();
	bool isEnd();
	unsigned long getSize() const
	{
		return m_size;
	}

private:
	long m_offset;
	unsigned long m_size;
	unsigned char *m_data;
	WPXMemoryInputStream(const WPXMemoryInputStream &);
	WPXMemoryInputStream &operator=(const WPXMemoryInputStream &);
};

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
