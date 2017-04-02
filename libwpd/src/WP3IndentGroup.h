/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3INDENTGROUP_H
#define WP3INDENTGROUP_H

#include "WP3FixedLengthGroup.h"
#include "WP3FileStructure.h"

class WP3IndentGroup : public WP3FixedLengthGroup
{
public:
	WP3IndentGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP3Listener *listener);

protected:
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

private:
	unsigned char m_subGroup;
	double m_offset;
};

#endif /* WP3INDENTGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
