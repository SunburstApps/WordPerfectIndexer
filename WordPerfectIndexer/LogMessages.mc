;// Begin header section.

SeverityNames=(
	Success=0x0:STATUS_SEVERITY_SUCCESS
	Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
	Warning=0x2:STATUS_SEVERITY_WARNING
	Error=0x3:STATUS_SEVERITY_ERROR
)

FacilityNames=(
	Filter=0x0:FACILITY_METADATA_FILTER
)

LanguageNames=(English=0x409:MSG00409)

;// Begin event category section.

MessageIdTypedef=WORD

MessageId=0x1
SymbolicName=TEXT_EXTRACTION_CATEGORY
Language=English
Report metadata to Windows
.

;// Begin event definition section.

MessageIdTypedef=DWORD

MessageId=0x100
Severity=Error
Facility=Filter
SymbolicName=MSG_WPD_DOC_CORRUPT
Language=English
Could not parse WordPerfect document
.

MessageId=0x101
Severity=Informational
Facility=Filter
SymbolicName=MSG_BEGIN_IMPORT
Language=English
Beginning import of WordPerfect document
.

MessageId=0x102
Severity=Informational
SymbolicName=MSG_END_IMPORT
Language=English
WordPerfect document import completed successfully
.
