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

#ifndef WP6VARIABLELENGTHGROUP_H
#define WP6VARIABLELENGTHGROUP_H

#include "WP6Part.h"

class WP6VariableLengthGroup_SubGroup
{
public:
	virtual ~WP6VariableLengthGroup_SubGroup() {}
	virtual void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const = 0;
};

class WP6VariableLengthGroup : public WP6Part
{
public:
	WP6VariableLengthGroup(); // WP6VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP6VariableLengthGroup();

	static WP6VariableLengthGroup *constructVariableLengthGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char groupID);

	static bool isGroupConsistent(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char groupID);

protected:
	void _read(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	virtual void _readContents(librevenge::RVNGInputStream * /* input */, WPXEncryption * /* encryption */) {} // we don't always need more information than that provided generically

	unsigned char getSubGroup() const
	{
		return m_subGroup;
	}
	unsigned short getSize() const
	{
		return m_size;
	}
	unsigned char getFlags() const
	{
		return m_flags;
	}
	unsigned char getNumPrefixIDs() const
	{
		return m_numPrefixIDs;
	}
	const unsigned short *getPrefixIDs() const
	{
		return m_prefixIDs;
	}
	unsigned short getSizeNonDeletable() const
	{
		return m_sizeNonDeletable;
	}
	unsigned short getSizeDeletable() const
	{
		return m_sizeDeletable;
	}

private:
	WP6VariableLengthGroup(const WP6VariableLengthGroup &);
	WP6VariableLengthGroup &operator=(const WP6VariableLengthGroup &);
	unsigned char m_subGroup;
	unsigned short m_size;
	unsigned char m_flags;
	unsigned char m_numPrefixIDs;
	unsigned short *m_prefixIDs;
	unsigned short m_sizeNonDeletable;
	unsigned short m_sizeDeletable;
};

#endif /* WP6VARIABLELENGTHGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
