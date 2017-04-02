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
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6HIGHLIGHTGROUP_H
#define WP6HIGHLIGHTGROUP_H

#include "WP6FixedLengthGroup.h"

class WP6HighlightGroup : public WP6FixedLengthGroup
{
public:
	WP6HighlightGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	virtual void parse(WP6Listener *listener) = 0;
	const RGBSColor getColor() const
	{
		return m_color;
	}

protected:
	virtual void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

private:
	RGBSColor m_color;
};

class WP6HighlightOnGroup : public WP6HighlightGroup
{
public:
	WP6HighlightOnGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP6Listener *listener);
};

class WP6HighlightOffGroup : public WP6HighlightGroup
{
public:
	WP6HighlightOffGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP6Listener *listener);
};

#endif /* WP6HIGHLIGHTGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
