#include "IniFile.h"


CIniFile::CIniFile(char *szFileName, int iReadWrite, int &iErrorCode) {
	iniFile.clear();

	if (iReadWrite == PINI_READ) {

		fileHandle.open(szFileName, ios_base::in);

		if (!fileHandle.is_open())
		{
			iErrorCode = PINI_FILE_NO_EXIST;
			iIsError = PINI_FILE_NO_EXIST;
			return;
		}
	}
	else {
		fileHandle.open(szFileName, std::fstream::in | std::fstream::out | std::fstream::app);

		if (!fileHandle) {
			iErrorCode = PINI_OPEN_ERROR;
			iIsError = PINI_OPEN_ERROR;
			return;
		}
	}

	for (string line; getline(fileHandle, line);) {
		size_t found = line.find('=');
		if (found != std::string::npos) {
			string key;
			string value;
			size_t keyend = found;
			if(line.at(found - 1) == ' ') keyend = found - 1;
			key.append(line, 0, keyend);

			size_t valuebegin = found+1;
			if (line.at(found + 1) == ' ') valuebegin = found+2;

			value.append(line, valuebegin, line.length());

			iniFile.insert(std::pair<string, string>(key, value));
		}
		else {
			iErrorCode = PINI_PARSE_ERROR;
			iIsError = PINI_PARSE_ERROR;
			return;
		}
	}

	if (iReadWrite == PINI_READWRITE) {
		fileHandle.close();
		fileHandle.open(szFileName, std::fstream::out);
	}

	if (iniFile.size() <= 0 && iReadWrite != PINI_READWRITE) {
		iErrorCode = PINI_EMPTY_FILE;
		iIsError = PINI_EMPTY_FILE;
		return;
	}

	iIsError = PINI_NO_ERROR;

	giReadWrite = iReadWrite;

}


CIniFile::~CIniFile() {
	if (iIsError == PINI_NO_ERROR) {
		if (giReadWrite == PINI_READWRITE) {

			unordered_map <string, string>::iterator cur;
			for (cur = iniFile.begin(); cur != iniFile.end(); cur++)
			{
				string key = cur->first;
				string value = cur->second;

				fileHandle << key << " = " << value << endl;
			}
		}
		
		fileHandle.close();
	}
}

int CIniFile::GetIniKeysCount() {
	return iniFile.size();
}

int CIniFile::GetIniKeyExists(char * szKey) {
	std::unordered_map<string, string>::iterator it;

	it = iniFile.find(szKey);

	if (it == iniFile.end()) return 0;

	return 1;
}

int CIniFile::GetIniInt(char * szKey, int & iValue) {
	if (iIsError != PINI_NO_ERROR) return iIsError;
		
	std::unordered_map<string, string>::iterator it;

	it = iniFile.find(szKey);

	if (it == iniFile.end()) return PINI_KEY_NOT_FOUND;

	iValue = std::stoi(it->second);

	return PINI_NO_ERROR;
}

int CIniFile::GetIniFloat(char * szKey, float & fValue) {
	if (iIsError != PINI_NO_ERROR) return iIsError;

	std::unordered_map<string, string>::iterator it;

	it = iniFile.find(szKey);

	if (it == iniFile.end()) return PINI_KEY_NOT_FOUND;

	fValue = std::stof(it->second);

	return PINI_NO_ERROR;
}

int CIniFile::GetIniString(char * szKey, const char *& szValue) {
	if (iIsError != PINI_NO_ERROR) return iIsError;

	std::unordered_map<string, string>::iterator it;

	it = iniFile.find(szKey);

	if (it == iniFile.end()) return PINI_KEY_NOT_FOUND;

	szValue = it->second.c_str();

	return PINI_NO_ERROR;
}

int CIniFile::SetIniInt(char * szKey, int iValue) {
	if (iIsError != PINI_NO_ERROR) return iIsError;

	if (giReadWrite == PINI_READ) return PINI_READONLY;

	std::unordered_map<string, string>::iterator it;
	it = iniFile.find(szKey);

	if (it == iniFile.end()) {
		iniFile.insert(std::pair<string, string>(szKey, to_string(iValue)));
	}
	else {
		iniFile.at(szKey) = to_string(iValue);
	}

	return PINI_NO_ERROR;
}

int CIniFile::SetIniFloat(char * szKey, float fValue) {
	if (iIsError != PINI_NO_ERROR) return iIsError;

	if (giReadWrite == PINI_READ) return PINI_READONLY;

	std::unordered_map<string, string>::iterator it;
	it = iniFile.find(szKey);

	if (it == iniFile.end()) {
		iniFile.insert(std::pair<string, string>(szKey, to_string(fValue)));
	}
	else {
		iniFile.at(szKey) = to_string(fValue);
	}

	return PINI_NO_ERROR;
}

int CIniFile::SetIniString(char * szKey, char * szValue)
{
	if (iIsError != PINI_NO_ERROR) return iIsError;

	if (giReadWrite == PINI_READ) return PINI_READONLY;

	std::unordered_map<string, string>::iterator it;
	it = iniFile.find(szKey);

	if (it == iniFile.end()) {
		iniFile.insert(std::pair<string, string>(szKey, szValue));
	}
	else {
		iniFile.at(szKey) = szValue;
	}

	return PINI_NO_ERROR;
}

int CIniFile::RemoveIniKey(char * szKey) {
	if (iIsError != PINI_NO_ERROR) return iIsError;

	if (giReadWrite == PINI_READ) return PINI_READONLY;

	iniFile.erase(szKey);

	return PINI_NO_ERROR;
}
