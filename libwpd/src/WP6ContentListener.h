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
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6CONTENTLISTENER_H
#define WP6CONTENTLISTENER_H

#include <librevenge/librevenge.h>
#include "WP6Listener.h"
#include "WPXContentListener.h"
#include "WP6FileStructure.h"

#include <stack>
#include <map>
#include <vector>

class WPXTable;

enum WP6StyleState { NORMAL, DOCUMENT_NOTE, DOCUMENT_NOTE_GLOBAL,
                     BEGIN_BEFORE_NUMBERING,
                     BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING,
                     DISPLAY_REFERENCING,
                     BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING,
                     BEGIN_AFTER_NUMBERING, STYLE_BODY, STYLE_END
                   };

enum WP6ListType { ORDERED, UNORDERED };

const int STATE_MEMORY = 3;
class WP6StyleStateSequence
{
public:
	WP6StyleStateSequence() : m_stateSequence()
	{
		clear();
	}
	void setCurrentState(WP6StyleState state)
	{
		for (unsigned i=(STATE_MEMORY-1); i>0; i--) m_stateSequence[i] = m_stateSequence[i-1];
		m_stateSequence[0]=state;
	}
	WP6StyleState getCurrentState() const
	{
		return m_stateSequence[0];
	}
	WP6StyleState getPreviousState() const
	{
		return m_stateSequence[1];
	}
	void clear()
	{
		m_stateSequence.clear();
		for (int i=0; i<STATE_MEMORY; i++) m_stateSequence.push_back(NORMAL);
	}

private:
	std::vector<WP6StyleState> m_stateSequence;
};

struct WP6ContentParsingState
{
	WP6ContentParsingState(WPXTableList tableList, unsigned nextTableIndice = 0);
	~WP6ContentParsingState();
	librevenge::RVNGString m_bodyText;
	librevenge::RVNGString m_textBeforeNumber;
	librevenge::RVNGString m_textBeforeDisplayReference;
	librevenge::RVNGString m_numberText;
	librevenge::RVNGString m_textAfterDisplayReference;
	librevenge::RVNGString m_textAfterNumber;

	double m_paragraphMarginBottomRelative;
	double m_paragraphMarginBottomAbsolute;

	unsigned m_numRemovedParagraphBreaks;

	unsigned m_numListExtraTabs;
	bool m_isListReference;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	unsigned m_nextTableIndice;

	std::stack<unsigned> m_listLevelStack;
	std::stack<WP6ListType> m_listTypeStack;
	unsigned short m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	unsigned char m_oldListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	unsigned m_noteTextPID;
	unsigned m_numNestedNotes;

	bool m_isFrameOpened;

	bool m_isLinkOpened;

	unsigned m_leaderCharacter;
	unsigned char m_leaderNumSpaces;
	std::vector<WPXTabStop> m_tempTabStops;
	std::vector<bool> m_tempUsePreWP9LeaderMethod;

	WPXNumberingType m_currentPageNumberingType;

private:
	WP6ContentParsingState(const WP6ContentParsingState &);
	WP6ContentParsingState &operator=(const WP6ContentParsingState &);
};

struct WP6ListLevel
{
	int m_level;
	bool m_isListElementOpenAtLevel;
};

class WP6OutlineDefinition
{
public:
	WP6OutlineDefinition();
	WP6OutlineDefinition(const unsigned char *numberingMethods, const unsigned char tabBehaviourFlag);
	void update(const unsigned char *numberingMethods, const unsigned char tabBehaviourFlag);

	WPXNumberingType getListType(int level)
	{
		if (level >= 0 && unsigned(level) < WPD_NUM_ELEMENTS(m_listTypes))
			return m_listTypes[level];
		return WPXNumberingType();
	}

protected:
	void _updateNumberingMethods(const unsigned char *numberingMethods);

private:
	WPXNumberingType m_listTypes[WP6_NUM_LIST_LEVELS];
};

class WP6ContentListener : public WP6Listener, protected WPXContentListener
{
public:
	WP6ContentListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, librevenge::RVNGTextInterface *documentInterface);
	~WP6ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void setDate(const unsigned short type, const unsigned short year,
	             const unsigned char month, const unsigned char day,
	             const unsigned char hour, const unsigned char minute,
	             const unsigned char second, const unsigned char dayOfWeek,
	             const unsigned char timeZone, const unsigned char unused);
	void setExtendedInformation(const unsigned short type, const librevenge::RVNGString &data);
	void setAlignmentCharacter(const unsigned character);
	void setLeaderCharacter(const unsigned character, const unsigned char numSpaces);
	void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops,
	                    const std::vector<bool> &usePreWP9LeaderMethods);
	void insertCharacter(unsigned character);
	void insertTab(const unsigned char tabType, double tabPosition);
	void handleLineBreak();
	void insertEOL();
	void insertBreak(const unsigned char breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void lineSpacingChange(const double lineSpacing)
	{
		WPXContentListener::lineSpacingChange(lineSpacing);
	}
	void justificationChange(const unsigned char justification)
	{
		WPXContentListener::justificationChange(justification);
	}
	void characterColorChange(const unsigned char red, const unsigned char green, const unsigned char blue);
	void characterShadingChange(const unsigned char shading);
	void highlightChange(const bool isOn, const RGBSColor &color);
	void fontChange(const unsigned short matchedFontPointSize, const unsigned short fontPID, const librevenge::RVNGString &fontName);
	void attributeChange(const bool isOn, const unsigned char attribute);
	void spacingAfterParagraphChange(const double spacingRelative, const double spacingAbsolute);
	void pageNumberingChange(const WPXPageNumberPosition /* page numbering position */, const unsigned short /* matchedFontPointSize */, const unsigned short /* fontPID */) {}
	void pageMarginChange(const unsigned char /* side */, const unsigned short /* margin */) {}
	void pageFormChange(const unsigned short /* length */, const unsigned short /* width */, const WPXFormOrientation /* orientation */) {}
	void marginChange(const unsigned char side, const unsigned short margin);
	void paragraphMarginChange(const unsigned char side, const signed short margin);
	void indentFirstLineChange(const signed short offset);
	void columnChange(const WPXTextColumnType columnType, const unsigned char numColumns, const std::vector<double> &columnWidth,
	                  const std::vector<bool> &isFixedWidth);
	void updateOutlineDefinition(const unsigned short outlineHash, const unsigned char *numberingMethods, const unsigned char tabBehaviourFlag);

	void paragraphNumberOn(const unsigned short outlineHash, const unsigned char level, const unsigned char flag);
	void paragraphNumberOff();
	void displayNumberReferenceGroupOn(const unsigned char subGroup, const unsigned char level);
	void displayNumberReferenceGroupOff(const unsigned char subGroup);
	void styleGroupOn(const unsigned char subGroup);
	void styleGroupOff(const unsigned char subGroup);
	void globalOn(const unsigned char systemStyle);
	void globalOff();
	void noteOn(const unsigned short textPID);
	void noteOff(const WPXNoteType noteType);
	void headerFooterGroup(const unsigned char /* headerFooterType */, const unsigned char /* occurrenceBits */, const unsigned short /* textPID */) {}
	void suppressPageCharacteristics(const unsigned char /* suppressCode */) {}
	void setPageNumber(const unsigned short /* pageNumber */) {}
	void setPageNumberingType(const WPXNumberingType pageNumberingType);
	void endDocument()
	{
		WPXContentListener::endDocument();
	}
	void endSubDocument()
	{
		WPXContentListener::endSubDocument();
	}

	void defineTable(const unsigned char position, const unsigned short leftOffset);
	void addTableColumnDefinition(const unsigned width, const unsigned leftGutter, const unsigned rightGutter,
	                              const unsigned attributes, const unsigned char alignment);
	void startTable();
	void insertRow(const unsigned short rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
	void insertCell(const unsigned char colSpan, const unsigned char rowSpan, const unsigned char borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment,
	                const bool useCellAttributes, const unsigned cellAttributes);
	void endTable();
	void boxOn(const unsigned char anchoringType, const unsigned char generalPositioningFlags, const unsigned char horizontalPositioningFlags,
	           const signed short horizontalOffset, const unsigned char leftColumn, const unsigned char rightColumn,
	           const unsigned char verticalPositioningFlags, const signed short verticalOffset, const unsigned char widthFlags, const unsigned short width,
	           const unsigned char heightFlags, const unsigned short height, const unsigned char boxContentType, const unsigned short nativeWidth,
	           const unsigned short nativeHeight,
	           const librevenge::RVNGString &linkTarget);
	void boxOff();
	void insertGraphicsData(const unsigned short packetId);
	void insertTextBox(const WP6SubDocument *subDocument);
	void commentAnnotation(const unsigned short textPID);

	void undoChange(const unsigned char undoType, const unsigned short undoLevel);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);

	//void _handleLineBreakElementBegin();
	void _paragraphNumberOn(const unsigned short outlineHash, const unsigned char level);
	void _flushText();
	void _handleListChange(const unsigned short outlineHash);

	void _changeList();

private:
	WP6ContentListener(const WP6ContentListener &);
	WP6ContentListener &operator=(const WP6ContentListener &);
	WP6ContentParsingState *m_parseState;

	std::map<unsigned short,WP6OutlineDefinition> m_outlineDefineHash;
	std::map<unsigned, librevenge::RVNGPropertyList> m_listDefinitions;
};

#endif /* WP6CONTENTLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
