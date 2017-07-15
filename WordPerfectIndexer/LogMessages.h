// Begin header section.
// Begin event category section.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_METADATA_FILTER         0x0


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: TEXT_EXTRACTION_CATEGORY
//
// MessageText:
//
// Report metadata to Windows
//
#define TEXT_EXTRACTION_CATEGORY         ((WORD)0x00000001L)

// Begin event definition section.
//
// MessageId: MSG_WPD_DOC_CORRUPT
//
// MessageText:
//
// Could not parse WordPerfect document
//
#define MSG_WPD_DOC_CORRUPT              ((DWORD)0xC0000100L)

//
// MessageId: MSG_BEGIN_IMPORT
//
// MessageText:
//
// Beginning import of WordPerfect document
//
#define MSG_BEGIN_IMPORT                 ((DWORD)0x40000101L)

//
// MessageId: MSG_END_IMPORT
//
// MessageText:
//
// WordPerfect document import completed successfully
//
#define MSG_END_IMPORT                   ((DWORD)0x40000102L)

//
// MessageId: MSG_GETISTREAMLENGTH_FAILURE
//
// MessageText:
//
// Could not get the length of the IStream (error %1)
//
#define MSG_GETISTREAMLENGTH_FAILURE     ((DWORD)0xC0000103L)

