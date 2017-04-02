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

#ifndef WP5ATTRIBUTEGROUP_H
#define WP5ATTRIBUTEGROUP_H

#include "WP5Listener.h"
#include "WP5FixedLengthGroup.h"

class WP5AttributeGroup : public WP5FixedLengthGroup
{
public:
	WP5AttributeGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	virtual void parse(WP5Listener *listener) = 0;
	unsigned char getAttribute() const
	{
		return m_attribute;
	}

protected:
	virtual void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

private:
	unsigned char m_attribute;
};

class WP5AttributeOnGroup : public WP5AttributeGroup
{
public:
	WP5AttributeOnGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP5Listener *listener);
};

class WP5AttributeOffGroup : public WP5AttributeGroup
{
public:
	WP5AttributeOffGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID);
	void parse(WP5Listener *listener);
};

#endif /* WP5ATTRIBUTEGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
