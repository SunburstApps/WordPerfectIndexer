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

#ifndef WP6ATTRIBUTEGROUP_H
#define WP6ATTRIBUTEGROUP_H

#include "WP6FixedLengthGroup.h"

class WP6AttributeGroup : public WP6FixedLengthGroup
{
public:
	WP6AttributeGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	virtual void parse(WP6Listener *listener) = 0;
	unsigned char getAttribute() const
	{
		return m_attribute;
	}

protected:
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

private:
	unsigned char m_attribute;
};

class WP6AttributeOnGroup : public WP6AttributeGroup
{
public:
	WP6AttributeOnGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP6Listener *listener);
};

class WP6AttributeOffGroup : public WP6AttributeGroup
{
public:
	WP6AttributeOffGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP6Listener *listener);
};

#endif /* WP6ATTRIBUTEGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
