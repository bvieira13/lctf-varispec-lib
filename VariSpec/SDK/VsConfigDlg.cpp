// VsConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VsGui.h"
#include "VsConfigDlg.h"
#include "vsdrvr.h"
#include "comportUtil.h"
#include "VsGuiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVsConfigDlg dialog


CVsConfigDlg::CVsConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVsConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVsConfigDlg)
	//}}AFX_DATA_INIT
}


void CVsConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVsConfigDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVsConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CVsConfigDlg)
	ON_CBN_SELCHANGE(IDC_SERPORT, OnSelchangeSerport)
	ON_BN_CLICKED(IDC_AUTODETECT, OnAutodetect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL	CVsConfigDlg::Initialize(LPCSTR port)
{
	m_serPortStr = port;
	return TRUE;
}


BOOL	CVsConfigDlg::Retrieve(CString& serPortStr)
{
	serPortStr = m_serPortStr;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CVsConfigDlg message handlers

void CVsConfigDlg::OnSelchangeSerport() 
{
	CString port;
	GetDlgItemText(IDC_SERPORT, port);
	enum filterState fs = GetFilterState(port);
	SetFilterStatus(fs);
}

void CVsConfigDlg::OnOK() 
{
	//	get combo box info to member var
	CString serPortStr;
	GetDlgItemText(IDC_SERPORT, serPortStr);
	m_serPortStr = serPortStr;
	CDialog::OnOK();
}


BOOL CVsConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CComboBox *pcBox = (CComboBox *) GetDlgItem(IDC_SERPORT);
	FillComComboBox(pcBox, m_serPortStr);
	CString port;
	GetDlgItemText(IDC_SERPORT, port);
	enum filterState fs = GetFilterState(port);
	SetFilterStatus(fs);
	return TRUE;
}


void CVsConfigDlg::OnAutodetect() 
{
	const DWORD maxPorts = 32;
	CString	portNameStr[maxPorts];
	DWORD nPorts = GetComPortList(portNameStr, maxPorts);
	DWORD i=0;
	enum filterState fs;
	while (i<nPorts)
	{
		fs = GetFilterState(portNameStr[i]);
		if ((fs == NotOpen) || (fs == NotReady))
		{
			m_serPortStr = portNameStr[i];
			SetFilterStatus(fs);
			CComboBox *pcBox = (CComboBox *) GetDlgItem(IDC_SERPORT);
			int index = pcBox->FindStringExact(0, portNameStr[i]);
			if (index >= 0)
				pcBox->SetCurSel(index);
			return;
		}
		++i;
	}
	MessageBox("No filter detected - confirm it is connected", "Detect failed", MB_OK);
}

void CVsConfigDlg::SetFilterStatus(filterState fs)
{
	CString text = "Not Found";
	if (fs == NotOpen)
		text = "Ready";
	else if (fs == NotReady)
		text = "Initializing";
	else
		text = "Not Found";
	SetDlgItemText(IDC_FILTERSTATE, text);
}
