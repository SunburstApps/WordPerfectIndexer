/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#ifndef WP1HEADERFOOTERGROUP_H
#define WP1HEADERFOOTERGROUP_H

#include "WP1VariableLengthGroup.h"

class WP1SubDocument;

class WP1HeaderFooterGroup : public WP1VariableLengthGroup
{
public:
	WP1HeaderFooterGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char group);
	~WP1HeaderFooterGroup();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP1Listener *listener);

private:
	WP1HeaderFooterGroup(const WP1HeaderFooterGroup &);
	WP1HeaderFooterGroup &operator=(const WP1HeaderFooterGroup &);
	unsigned char m_definition;
	WP1SubDocument *m_subDocument;
};

#endif /* WP1HEADERFOOTERGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
