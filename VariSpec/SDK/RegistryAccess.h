// RegistryAccess.h: interface for the CRegistryAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYACCESS_H__71F2A463_B432_11D6_BBB5_444553540001__INCLUDED_)
#define AFX_REGISTRYACCESS_H__71F2A463_B432_11D6_BBB5_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegistryAccess  
{
public:
	CRegistryAccess();
	virtual ~CRegistryAccess();

	BOOL	OpenRegKey(LPCSTR keyName, BOOL& created, BOOL& inLocalMachine);
	BOOL	CloseRegKey(void);
	BOOL	DeleteRegKey(LPCSTR keyName);
	BOOL	DeleteRegEntry(LPCSTR subKey);

	BOOL	GetRegCString(LPCSTR name, CString& val);
	BOOL	GetRegString(LPCSTR name, char val[], int maxLen=MAX_PATH);
	BOOL	GetRegInt(LPCSTR name, int& val);
	BOOL	GetRegDouble(LPCSTR name, double& val);
	BOOL	SetRegString(LPCSTR name, LPCSTR val);
	BOOL	SetRegInt(LPCSTR name, const int val);
	BOOL	SetRegDouble(LPCSTR name, const double val);

private:
	HKEY	m_hKey;
};

#endif // !defined(AFX_REGISTRYACCESS_H__71F2A463_B432_11D6_BBB5_444553540001__INCLUDED_)
