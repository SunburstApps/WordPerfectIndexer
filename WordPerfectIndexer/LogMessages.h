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
// MessageId: MSG_GETTEMPPATH_FAILED
//
// MessageText:
//
// GetTempPath() call failed (error %1)
//
#define MSG_GETTEMPPATH_FAILED           ((DWORD)0xC0000100L)

//
// MessageId: MSG_GETTEMPFILENAME_FAILED
//
// MessageText:
//
// GetTempFileName() call failed (error %1)
//
#define MSG_GETTEMPFILENAME_FAILED       ((DWORD)0xC0000101L)

//
// MessageId: MSG_FOPEN_FAILED
//
// MessageText:
//
// Could not open file %1!s! (errno %2)
//
#define MSG_FOPEN_FAILED                 ((DWORD)0xC0000102L)

//
// MessageId: MSG_STREAM_READ_ERROR
//
// MessageText:
//
// Could not read from stream (HRESULT %1)
//
#define MSG_STREAM_READ_ERROR            ((DWORD)0xC0000103L)

//
// MessageId: MSG_WPD_DOC_CORRUPT
//
// MessageText:
//
// Could not parse WordPerfect document at path %1!s!
// 
// The file has not been deleted.
//
#define MSG_WPD_DOC_CORRUPT              ((DWORD)0xC0000104L)

//
// MessageId: MSG_BEGIN_IMPORT
//
// MessageText:
//
// Beginning import of WordPerfect document
//
#define MSG_BEGIN_IMPORT                 ((DWORD)0x40000105L)

//
// MessageId: MSG_END_IMPORT
//
// MessageText:
//
// WordPerfect document import completed successfully
//
#define MSG_END_IMPORT                   ((DWORD)0x40000106L)

//
// MessageId: MSG_DELETE_TEMP_FILE
//
// MessageText:
//
// Deleted temporary file %1!s!
//
#define MSG_DELETE_TEMP_FILE             ((DWORD)0x40000107L)

