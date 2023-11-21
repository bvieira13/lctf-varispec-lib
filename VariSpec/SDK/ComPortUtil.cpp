
#include "stdlib.h"
#include "stdafx.h"


int GetComPortList(CString nameList[], DWORD maxNames)
{
	//	number of ports
	DWORD nKeyValues = 0;

	//	get active COMx ports from registry key
	HKEY hKey;
	LONG openResult = RegOpenKeyEx(
						HKEY_LOCAL_MACHINE, 
						"HARDWARE\\DEVICEMAP\\SERIALCOMM", 
						NULL,
						KEY_READ,
						&hKey);

	//	failure? (should never happen)
	if (openResult != ERROR_SUCCESS) 
		return nKeyValues;

	//	loop through names and values, adding latter to combo box
	TCHAR name[MAX_PATH];
	TCHAR value[MAX_PATH];
	DWORD nameLen;
	DWORD valueLen;
	DWORD nSubKeys;
	DWORD maxSubKeyNameLen;
	DWORD maxNameLen;
	DWORD maxValLen;
	LONG infResult = RegQueryInfoKey(hKey, NULL, NULL, NULL, 
					&nSubKeys, &maxSubKeyNameLen, NULL, &nKeyValues, 
					&maxNameLen, &maxValLen, NULL, NULL);
	LONG revResult;
	DWORD valIndex = 0;
	nKeyValues = __min(maxNames, nKeyValues);
	for (valIndex=0; valIndex<nKeyValues; ++valIndex)
	{
		nameLen = MAX_PATH;
		valueLen = MAX_PATH;
		revResult = RegEnumValue(hKey, valIndex, name, &nameLen, NULL, NULL, (LPBYTE) value, &valueLen);
#ifdef _DEBUG
		if (revResult != ERROR_SUCCESS)
		{
			char buffer[MAX_PATH];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,  revResult, 0, buffer, MAX_PATH-1, NULL);
		}
#endif
		nameList[valIndex] = value;
	}

	//	return the number of keys populated
	return nKeyValues;
}

bool FillComComboBox(CComboBox *pcBox, LPCSTR targetString)
{
	//	returns true/false depending whether targetString was found

	//	quit if pcBox undefined
	if (!pcBox)
		return false;

	//	save old selection index then clear contents
	const DWORD listLen = 32;
	CString comList[listLen];
	DWORD numPorts = GetComPortList(comList, listLen);

	pcBox->ResetContent();
	if (numPorts < 1)
	{
		pcBox->AddString("COM1");
		pcBox->SetCurSel(0);
		return false;
	}

	//	loop through names, adding to combo box
	DWORD i;
	for (i=0; i<numPorts; ++i)
		pcBox->AddString(comList[i]);

	//	select the selection that was in place before - if possible
	int foundIndex = pcBox->FindString(0, targetString);
	int setIndex = __max(foundIndex, 0);
	pcBox->SetCurSel(setIndex);
	return (foundIndex >= 0);
}
