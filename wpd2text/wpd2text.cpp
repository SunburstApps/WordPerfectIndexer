/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
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

#include <stdio.h>
#include <string.h>
#include <librevenge-stream/librevenge-stream.h>
#include <librevenge-generators/librevenge-generators.h>
#include <libwpd/libwpd.h>

using namespace libwpd;

namespace /* anonymous */
{
	int ShowUsage(void)
	{
		fprintf(stderr, "wpd2text - Extract textual contents and metadata from WordPerfect files\n\n");
		fprintf(stderr, "usage: wpd2text [/info] [/password <password>] <file>\n");
		fprintf(stderr, "\t/metadata            - Display metadata instead of body text\n");
		fprintf(stderr, "\t/password <password> - Attempt to decrypt a password-protected file\n");
		return -1;
	}
}

int main(int argc, const char * argv[])
{
	if (argc < 2) return ShowUsage();

	const char *inputFile = nullptr;
	bool showMetadata = false;
	const char *password = nullptr;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "/password") == 0)
		{
			if (i < argc + 1) password = argv[++i];
		}
		else if (strcmp(argv[i], "/metadata") == 0)
		{
			showMetadata = true;
		}
		else if (strcmp(argv[i], "/?") == 0)
		{
			return ShowUsage();
		}
		else if (strncmp(argv[i], "/", 1) == 0)
		{
			fprintf(stderr, "Unknown option: %s\n", argv[i]);
			return ShowUsage();
		}
		else if (inputFile == nullptr)
		{
			inputFile = argv[i];
		}
		else
		{
			fprintf(stderr, "Cannot specify more than one file\n");
			return ShowUsage();
		}
	}

	if (inputFile == nullptr) return ShowUsage();

	librevenge::RVNGFileStream input(inputFile);

	WPDConfidence confidence = WPDocument::isFileFormatSupported(&input);
	if (confidence != WPD_CONFIDENCE_EXCELLENT && confidence != WPD_CONFIDENCE_SUPPORTED_ENCRYPTION)
	{
		fprintf(stderr, "%s: unsupported file format\n", inputFile);
		return 1;
	}

	if (confidence == WPD_CONFIDENCE_SUPPORTED_ENCRYPTION && password == nullptr)
	{
		fprintf(stderr, "%s: file is encrypted, but password is not specified\n", inputFile);
		fprintf(stderr, "Try specifying one with the /password flag\n");
		return 1;
	}

	if (confidence == WPD_CONFIDENCE_SUPPORTED_ENCRYPTION && (WPDocument::verifyPassword(&input, password) != WPD_PASSWORD_MATCH_OK))
	{
		fprintf(stderr, "%s: cannot decrypt: incorrect password\n", inputFile);
		fprintf(stderr, "This could also happen if the document is not encrypted in the first place.\n");
		return 1;
	}

	librevenge::RVNGString document;
	librevenge::RVNGTextTextGenerator documentGenerator(document, showMetadata);
	WPDResult error = WPDocument::parse(&input, &documentGenerator, password);

	if (error == WPD_FILE_ACCESS_ERROR) fprintf(stderr, "%s: cannot access document\n", inputFile);
	else if (error == WPD_PARSE_ERROR) fprintf(stderr, "%s: parsing error (invalid document)\n", inputFile);
	else if (error == WPD_UNSUPPORTED_ENCRYPTION_ERROR) fprintf(stderr, "%s: cannot read: unsupported encryption\n", inputFile);
	else if (error == WPD_OLE_ERROR) fprintf(stderr, "%s: error: invalid document (could not find WordPerfect stream in OLE format document)\n", inputFile);
	else if (error != WPD_OK) fprintf(stderr, "%s: cannot read: unknown error\n", inputFile);

	if (error != WPD_OK) return 1;

	printf("%s\n", document.cstr());
	return 0;
}
