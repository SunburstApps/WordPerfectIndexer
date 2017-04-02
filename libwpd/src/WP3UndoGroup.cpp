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

#include "WP3UndoGroup.h"
#include "libwpd_internal.h"
#include "WP3Listener.h"

WP3UndoGroup::WP3UndoGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID) :
	WP3FixedLengthGroup(groupID),
	m_undoType(0),
	m_undoLevel(0)
{
	_read(input, encryption);
}

void WP3UndoGroup::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	m_undoType = readU8(input, encryption);
	m_undoLevel = readU16(input, encryption, true);
}

void WP3UndoGroup::parse(WP3Listener *listener)
{
	listener->undoChange(m_undoType, m_undoLevel);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
