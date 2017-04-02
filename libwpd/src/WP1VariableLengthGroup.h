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
 * Copyright (c) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP1VARIABLELENGTHGROUP_H
#define WP1VARIABLELENGTHGROUP_H

#include "WP1Part.h"

class WP1VariableLengthGroup : public WP1Part
{
public:
	WP1VariableLengthGroup(unsigned char group); // WP1VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP1VariableLengthGroup() {}

	static WP1VariableLengthGroup *constructVariableLengthGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char group);

	static bool isGroupConsistent(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char group);

protected:
	void _read(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	virtual void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption) = 0;

	unsigned char getGroup() const
	{
		return m_group;
	}
	unsigned getSize() const
	{
		return m_size;
	}

private:
	unsigned char m_group;
	unsigned m_size;
};

#endif /* WP1VARIABLELENGTHGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
