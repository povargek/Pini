#include "IniFile.h"

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

typedef void(*logprintf_t)(char* format, ...);

logprintf_t logprintf;

void **ppPluginData;
extern void *pAMXFunctions;

CIniFile *pIniFiles[PINI_MAX_OPENED_FILES];
int iIniSlots[PINI_MAX_OPENED_FILES];

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()  {
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max) {
	cell* dest = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
	cell* start = dest;
	while (max--&&*source)
		*dest++ = (cell)*source++;
	*dest = 0;
	return dest - start;
}



// native OpenIniFile(&PINI:Handle, filename[], readwrite = PINI_READWRITE);
static cell AMX_NATIVE_CALL n_OpenIniFile(AMX* amx, cell* params) {
	char *szFileName;
	amx_StrParam(amx, params[2], szFileName);
	int rw = params[3];

	for (int i = 0; i < PINI_MAX_OPENED_FILES; i++) {
		if (iIniSlots[i] == 0) {
			int err = PINI_NO_ERROR;
			pIniFiles[i] = new CIniFile(szFileName, rw, err);

			if (err != PINI_NO_ERROR) {
				cell* cptr;
				amx_GetAddr(amx, params[1], &cptr);
				*cptr = (cell)(0xFF);

				iIniSlots[i] = 0;
				delete pIniFiles[i];
			}
			else {
				cell* cptr;
				amx_GetAddr(amx, params[1], &cptr);
				*cptr = (cell)(i);

				iIniSlots[i] = 1;
			}

			return err;
		}
	}

	return PINI_OPEN_LIMIT;
}

// native GetIniKeysCount(PINI:Handle);

static cell AMX_NATIVE_CALL n_GetIniKeysCount(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return -1;

	if (pIniFiles[handle] == nullptr) return -1;

	if (iIniSlots[handle] == 0) return -1;

	return pIniFiles[handle]->GetIniKeysCount();
}

// native GetIniKeyExists(PINI:Handle, const key[]);

static cell AMX_NATIVE_CALL n_GetIniKeyExists(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return -1;

	if (pIniFiles[handle] == nullptr) return -1;

	if (iIniSlots[handle] == 0) return -1;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	return pIniFiles[handle]->GetIniKeyExists(szKey);
}

// native GetIniInt(PINI:Handle, const key[], &value);

static cell AMX_NATIVE_CALL n_GetIniInt(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	int iRetVal;
	
	int ret = pIniFiles[handle]->GetIniInt(szKey, iRetVal);

	if (ret != PINI_NO_ERROR) return ret;

	cell* cptr;
	amx_GetAddr(amx, params[3], &cptr);
	*cptr = (cell)(iRetVal);

	return ret;
}

// native GetIniFloat(PINI:Handle, const key[], &Float:value);

static cell AMX_NATIVE_CALL n_GetIniFloat(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	float fRetVal;

	int ret = pIniFiles[handle]->GetIniFloat(szKey, fRetVal);

	if (ret != PINI_NO_ERROR) return ret;

	cell* cptr;

	amx_GetAddr(amx, params[3], &cptr);
	*cptr = amx_ftoc(fRetVal);

	return ret;
}

// native GetIniString(PINI:Handle, const key[], value[], destsize = sizeof(value));

static cell AMX_NATIVE_CALL n_GetIniString(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	const char *szValue;

	int ret = pIniFiles[handle]->GetIniString(szKey, szValue);

	if (ret != PINI_NO_ERROR) return ret;

	set_amxstring(amx, params[3], szValue, params[4]);

	return PINI_NO_ERROR;
}

// native SetIniInt(PINI:Handle, const key[], value);

static cell AMX_NATIVE_CALL n_SetIniInt(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	int ret = pIniFiles[handle]->SetIniInt(szKey, params[3]);

	return ret;
}

// native SetIniFloat(PINI:Handle, const key[], Float:value);

static cell AMX_NATIVE_CALL n_SetIniFloat(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	int ret = pIniFiles[handle]->SetIniFloat(szKey, amx_ctof(params[3]));

	return ret;
}

// native SetIniString(PINI:Handle, const key[], value[]);

static cell AMX_NATIVE_CALL n_SetIniString(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	char *szValue;
	amx_StrParam(amx, params[2], szKey);
	amx_StrParam(amx, params[3], szValue);

	int ret = pIniFiles[handle]->SetIniString(szKey, szValue);

	return ret;
}

// native CloseIniFile(PINI:Handle);

static cell AMX_NATIVE_CALL n_CloseIniFile(AMX* amx, cell* params) {
	int handle = params[1];
	
	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	delete pIniFiles[handle];

	iIniSlots[handle] = 0;

	return PINI_NO_ERROR;
}

// native RemoveIniKey(PINI:Handle, const key[]);

static cell AMX_NATIVE_CALL n_RemoveIniKey(AMX* amx, cell* params) {
	int handle = params[1];

	if (handle < 0 || handle >= PINI_MAX_OPENED_FILES) return PINI_INVALID_HANDLE;

	if (pIniFiles[handle] == nullptr) return PINI_INVALID_HANDLE;

	if (iIniSlots[handle] == 0) return PINI_INVALID_HANDLE;

	char *szKey;
	amx_StrParam(amx, params[2], szKey);

	return pIniFiles[handle]->RemoveIniKey(szKey);
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) {
	memset(iIniSlots, 0, sizeof(iIniSlots));
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf(" Pini plugin by povargek loaded! (http://vk.com/povargek)\n");
	return true;
}
PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	logprintf(" Pini plugin by povargek unloaded! (http://vk.com/povargek)\n");
}

AMX_NATIVE_INFO pluginNatives[] = {
	{ "OpenIniFile", n_OpenIniFile },
	{ "GetIniKeysCount", n_GetIniKeysCount },
	{ "GetIniKeyExists", n_GetIniKeyExists },
	{ "GetIniInt", n_GetIniInt },
	{ "GetIniFloat", n_GetIniFloat },
	{ "GetIniString", n_GetIniString },
	{ "SetIniInt", n_SetIniInt },
	{ "SetIniFloat", n_SetIniFloat },
	{ "SetIniString", n_SetIniString },
	{ "CloseIniFile", n_CloseIniFile },
	{ "RemoveIniKey", n_RemoveIniKey },
	{ 0,		0 }
};


PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {	
	return amx_Register( amx, pluginNatives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
}
