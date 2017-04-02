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

#ifndef WP6CHARACTERGROUP_H
#define WP6CHARACTERGROUP_H

#include "WP6VariableLengthGroup.h"
#include "WP6FontDescriptorPacket.h"

class WP6CharacterGroup_SetAlignmentCharacterSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetAlignmentCharacterSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_character;
	unsigned char m_characterSet;
};

class WP6CharacterGroup_ColorSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ColorSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_red;
	unsigned char m_green;
	unsigned char m_blue;
};

class WP6CharacterGroup_CharacterShadingChangeSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CharacterShadingChangeSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_shading;
};

class WP6CharacterGroup_FontFaceChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontFaceChangeSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned short sizeDeletable);
	~WP6CharacterGroup_FontFaceChangeSubGroup();
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned short m_oldMatchedPointSize;
	unsigned short m_hash;
	unsigned short m_matchedFontIndex;
	unsigned short m_matchedFontPointSize;
	WP6FontDescriptorPacket *m_packet;
	// Unimplemented to prevent compiler from creating crasher ones
	WP6CharacterGroup_FontFaceChangeSubGroup(const WP6CharacterGroup_FontFaceChangeSubGroup &);
	WP6CharacterGroup_FontFaceChangeSubGroup &operator=(const WP6CharacterGroup_FontFaceChangeSubGroup &);

};

class WP6CharacterGroup_FontSizeChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontSizeChangeSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned short m_desiredFontPointSize;
};

class WP6CharacterGroup_SetDotLeaderCharactersSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetDotLeaderCharactersSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_character;
	unsigned char m_characterSet;
	unsigned char m_numberOfSpaces;
};

class WP6CharacterGroup_ParagraphNumberOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ParagraphNumberOnSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned short m_outlineHash;
	unsigned char m_level;
	unsigned char m_flag;
};

class WP6CharacterGroup_TableDefinitionOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOnSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_flags;
	unsigned char m_position;
	unsigned short m_leftOffset;
};

class WP6CharacterGroup_TableDefinitionOffSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOffSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:

};

class WP6CharacterGroup_TableColumnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableColumnSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:
	unsigned char m_flags;
	unsigned short m_width;
	unsigned short m_leftGutter;
	unsigned short m_rightGutter;
	unsigned m_attributes;
	unsigned char m_alignment;
	unsigned short m_absPosFromRight;
	unsigned short m_numberType;
	unsigned char m_currencyIndex;
};

class WP6CharacterGroup_CommentSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CommentSubGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const unsigned char numPrefixIDs, unsigned short const *prefixIDs) const;

private:

};

class WP6CharacterGroup : public WP6VariableLengthGroup
{
public:
	WP6CharacterGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	~WP6CharacterGroup();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	WP6CharacterGroup(const WP6CharacterGroup &);
	WP6CharacterGroup &operator=(const WP6CharacterGroup &);
	WP6VariableLengthGroup_SubGroup *m_subGroupData;

};

#endif /* WP6CHARACTERGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
