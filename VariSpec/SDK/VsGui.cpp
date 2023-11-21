// VsGui.cpp : Defines the class behaviors for the application.
//

#ifdef USESTDAFX
#include "stdafx.h"
#else
#include "windows.h"
#endif

#include "vsdrvr.h"
#include "VsGui.h"
#include "VsGuiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVsGuiApp

BEGIN_MESSAGE_MAP(CVsGuiApp, CWinApp)
	//{{AFX_MSG_MAP(CVsGuiApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVsGuiApp construction

CVsGuiApp::CVsGuiApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVsGuiApp object

CVsGuiApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVsGuiApp initialization

BOOL CVsGuiApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	GetRegVars();

	CVsGuiDlg dlg;
	m_pMainWnd = &dlg;

	dlg.Initialize(m_serPort);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		dlg.Retrieve(m_serPort);
		SaveRegVars();
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


BOOL CVsGuiApp::GetRegVars()
{
	BOOL	created, inLocal;
	OpenRegKey("Software\\CRI\\VariSpec", created, inLocal);
	if (created)
	{
		SetRegString("SerialPort", "COM1");
		SaveRegVars();
	}
	else
	{
		GetRegCString("SerialPort", m_serPort);
	}

	CloseRegKey();
	if (m_serPort.GetLength() > 5)
		m_serPort = "COM1";

	return TRUE;
}


BOOL CVsGuiApp::SaveRegVars(void)
{
	BOOL	created, inLocal;

	OpenRegKey("Software\\CRI\\VariSpec", created, inLocal);
	SetRegString("SerialPort", m_serPort);

	CloseRegKey();
	return TRUE;
}
