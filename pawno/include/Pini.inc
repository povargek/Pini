// Pini INI File Reader/Writer by povargek

#define PINI_MAX_OPENED_FILES 128

#define PINI_INVALID_FILE PINI:0xFF

#define PINI_READ 0
#define PINI_READWRITE 1

#define PINI_NO_ERROR 0

#define PINI_UNKNOWN_ERROR 9

#define PINI_FILE_NO_EXIST 10
#define PINI_OPEN_ERROR 11
#define PINI_PARSE_ERROR 12
#define PINI_EMPTY_FILE 13

#define PINI_KEY_NOT_FOUND 20

#define PINI_INVALID_HANDLE 21
#define PINI_READONLY 22
#define PINI_OPEN_LIMIT 23

native OpenIniFile(&PINI:Handle, filename[], readwrite = PINI_READWRITE);
native GetIniKeysCount(PINI:Handle);
native GetIniInt(PINI:Handle, const key[], &value);
native GetIniFloat(PINI:Handle, const key[], &Float:value);
native GetIniString(PINI:Handle, const key[], value[], destsize = sizeof(value));
native SetIniInt(PINI:Handle, const key[], value);
native SetIniFloat(PINI:Handle, const key[], Float:value);
native SetIniString(PINI:Handle, const key[], value[]);
native CloseIniFile(PINI:Handle);