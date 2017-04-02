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
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
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

#include "WP42Part.h"
#include "WP42FileStructure.h"
#include "WP42MultiByteFunctionGroup.h"
#include "libwpd_internal.h"

// constructPart: constructs a parseable low-level representation of part of the document
// returns the part if it successfully creates the part, returns 0 if it can't
// throws an exception if there is an error
// precondition: readVal us between 0xC0 and 0xFF
// TODO: check the precondition :D
WP42Part *WP42Part::constructPart(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char readVal)
{
	WPD_DEBUG_MSG(("WordPerfect: Offset: %i, ConstructPart(readVal: 0x%2x)\n", (unsigned int)input->tell(), readVal));

	if (((unsigned char)0xC0 > readVal) || ((unsigned char)0xFE < readVal))
	{
		WPD_DEBUG_MSG(("WordPerfect: Returning 0 from constructPart\n"));
		return 0;
	}

	WPD_DEBUG_MSG(("WordPerfect: constructMultiByteFunctionGroup(input, val)\n"));
	return WP42MultiByteFunctionGroup::constructMultiByteFunctionGroup(input, encryption, readVal);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
