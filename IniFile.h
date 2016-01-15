#pragma once

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#define HAVE_STDINT_H

using namespace std;

#define PINI_MAX_OPENED_FILES 128

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

class CIniFile {
public:
	CIniFile(char *szFileName, int iReadWrite, int &iErrorCode);
	~CIniFile();

	int GetIniKeysCount();
	int GetIniKeyExists(char *szKey);

	int GetIniInt(char *szKey, int &iValue);
	int GetIniFloat(char *szKey, float &fValue);
	int GetIniString(char *szKey, const char* &szValue);

	int SetIniInt(char *szKey, int iValue);
	int SetIniFloat(char *szKey, float fValue);
	int SetIniString(char *szKey, char* szValue);

	int RemoveIniKey(char *szKey);

private:
	fstream fileHandle;
	int giReadWrite;
	int iIsError;
	unordered_map< string, string > iniFile;
};