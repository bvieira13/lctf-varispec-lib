// RegistryAccess.cpp: implementation of the CRegistryAccess class.
//
//////////////////////////////////////////////////////////////////////

#ifdef USESTDAFX
#include "stdafx.h"
#else
#include "windows.h"
#endif

#include "RegistryAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryAccess::CRegistryAccess()
{
	m_hKey = NULL;
}

CRegistryAccess::~CRegistryAccess()
{
	if (m_hKey)
		RegCloseKey(m_hKey);
}


BOOL CRegistryAccess::OpenRegKey(LPCSTR keyName, BOOL &createdKey, BOOL &keyInLocalMachine)
{
	LONG	openResult;
	DWORD	keyCreateDisposition;
	char	msg[MAX_PATH];
	HKEY	hKey;


	openResult = RegCreateKeyEx(HKEY_CURRENT_USER, keyName, 
		0,
		REG_NONE,			//	lpClass (whatever that is) is REG_NONE
		REG_OPTION_NON_VOLATILE,
		KEY_READ | KEY_WRITE,
		NULL,
		&hKey,
		&keyCreateDisposition);

	if (openResult != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, (DWORD) openResult, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		msg, MAX_PATH-1, NULL); 
		return FALSE;
	}

	createdKey = (keyCreateDisposition == REG_CREATED_NEW_KEY);
	keyInLocalMachine = FALSE;
	m_hKey = hKey;
	return TRUE;
}


BOOL	CRegistryAccess::CloseRegKey(void)
{
	if (m_hKey)
		RegCloseKey(m_hKey);
	m_hKey = NULL;
	return TRUE;
}


CRegistryAccess::DeleteRegKey(LPCSTR keyName)
{
	if (m_hKey)
		RegCloseKey(m_hKey);

	m_hKey = NULL;
	LONG stat = RegDeleteKey(HKEY_CURRENT_USER,  keyName);
	return (stat == ERROR_SUCCESS);
}


CRegistryAccess::DeleteRegEntry(LPCSTR entryName)
{
	if (!m_hKey)
		return FALSE;

	LONG stat = RegDeleteValue(m_hKey, entryName);
	return (stat == ERROR_SUCCESS);
}



BOOL CRegistryAccess::GetRegCString(LPCSTR entry, CString &csval)
{
	char	val[MAX_PATH];
	if (GetRegString(entry, val))
	{
		csval = val;
		return TRUE;
	}
	else
	{
		csval.Empty();
		return FALSE;
	}
}

BOOL CRegistryAccess::GetRegString(LPCSTR entry, char val[], int stringLen)
{
	if (!m_hKey)
		return FALSE;

	DWORD regType = REG_SZ;
	DWORD regDatSize = MAX_PATH - 1;
	LONG stat = RegQueryValueEx(m_hKey, entry, NULL, &regType, (LPBYTE) val, &regDatSize);
	return (stat == ERROR_SUCCESS);
}


BOOL CRegistryAccess::SetRegString(LPCSTR entry, LPCSTR val)
{
	if (!m_hKey)
		return FALSE;

	LONG stat = RegSetValueEx(m_hKey, entry, NULL, REG_SZ, (CONST BYTE *) val, (DWORD) strlen(val));
	return (stat == ERROR_SUCCESS);
}

BOOL CRegistryAccess::GetRegDouble(LPCSTR entry, double& val)
{
	val = 0.;
	char	buf[MAX_PATH];
	LONG stat = GetRegString(entry, buf);
	return ((stat == ERROR_SUCCESS) && 
			(sscanf(buf, "%lf", &val) > 0));
}

BOOL CRegistryAccess::SetRegDouble(LPCSTR entry, const double val)
{
	char buf[MAX_PATH];
	sprintf(buf, "%lf", val);
	return SetRegString(entry, buf);
}


BOOL CRegistryAccess::GetRegInt(LPCSTR entry, int& val)
{
	DWORD	regType = REG_DWORD;
	DWORD	regSize = sizeof(DWORD);
	LONG stat = RegQueryValueEx(m_hKey, entry, NULL, &regType, (BYTE *) (&val), &regSize);
	return (stat == ERROR_SUCCESS);
}


BOOL CRegistryAccess::SetRegInt(LPCSTR entry, const int val)
{
	if (!m_hKey)
		return FALSE;

	LONG stat = RegSetValueEx(m_hKey, entry, NULL, REG_DWORD, (CONST BYTE *) (&val), sizeof(DWORD));
	return (stat == ERROR_SUCCESS);
}

