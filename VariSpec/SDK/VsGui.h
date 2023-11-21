// VsGui.h : main header file for the VSGUI application
//

#if !defined(AFX_VSGUI_H__9E4FDC83_ED0D_11D6_BBB5_444553540001__INCLUDED_)
#define AFX_VSGUI_H__9E4FDC83_ED0D_11D6_BBB5_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "RegistryAccess.h"


/////////////////////////////////////////////////////////////////////////////
// CVsGuiApp:
// See VsGui.cpp for the implementation of this class
//

class CVsGuiApp : public CWinApp, CRegistryAccess
{
public:
	CVsGuiApp();

protected:
	BOOL	GetRegVars(void);
	BOOL	SaveRegVars(void);

	CString m_serPort;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVsGuiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVsGuiApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSGUI_H__9E4FDC83_ED0D_11D6_BBB5_444553540001__INCLUDED_)
