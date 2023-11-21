// VsGuiDlg.cpp : implementation file
//

#ifdef USESTDAFX
#include "stdafx.h"
#else
#include "windows.h"
#endif

#include "vsdrvr.h"
#include "VsGui.h"
#include "VsGuiDlg.h"
#include "VsConfigDlg.h"

#define	VS_WRITE_TIMER_ID		15
#define	VS_WRITE_ACTIVE_PAUSE	300
#define	VS_WAIT_FILTER_TIMER_ID		16
#define	VS_WAIT_FILTER_BUSY_PAUSE		1000
#define	VS_SHORT_PAUSE		20


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HWND hWnd_x = NULL;
UINT hTimer_x = NULL;
BOOL timerActive_x = FALSE;
BOOL updatePending_x = FALSE;
double updateWl_x = 0;
VS_HANDLE vsHnd_x = NULL;
char port_x[32];


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVsGuiDlg dialog

CVsGuiDlg::CVsGuiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVsGuiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVsGuiDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_minWl = 400.;
	m_maxWl = 720.;

	m_serno = 50001;
	m_serPortStr = "COM1";
	m_wl = -1;
	m_version = 1;
	m_fConnect = FALSE;
}

void CVsGuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVsGuiDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CVsGuiDlg::Initialize(LPCSTR port)
{
	m_serPortStr = port;
	return TRUE;
}

BOOL CVsGuiDlg::Retrieve(CString& port)
{
	port = m_serPortStr;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CVsGuiDlg, CDialog)
	//{{AFX_MSG_MAP(CVsGuiDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	ON_EN_KILLFOCUS(IDC_WL, OnKillfocusWl)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SWEEP, OnSweep)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVsGuiDlg message handlers


BOOL CVsGuiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_initializingDialog = true;
	Connect();
	UpdateFilterInfoToGui();
	m_initializingDialog = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CVsGuiDlg::Connect(void)
{
	m_fConnect = FALSE;
	char reply[255];

	//	clear reply, so non-null later is significant
	strcpy(reply, "");

	//	open ser port - can't? post message and return
	Int32 errorCode;
	if (!VsOpen(&vsHnd_x, m_serPortStr, &errorCode))
	{
		CString msg;
		if (errorCode == 105)
			msg.Format("No filter found at port %s; re-configure and try again", m_serPortStr);
		else if (errorCode == 117)
			msg.Format("Filter at port %s is initializing - controls will become active when it is ready", m_serPortStr);
		else
			msg.Format("Unable to connect to port %s; error code %d; re-configure and try again", m_serPortStr, errorCode);
		MessageBox(msg, "Error", MB_OK);

		if (errorCode == 117)
		{
			strncpy(port_x, m_serPortStr, 32);
			timerActive_x = TRUE;
			hWnd_x = GetSafeHwnd();
			hTimer_x = SetTimer(VS_WAIT_FILTER_TIMER_ID, VS_WAIT_FILTER_BUSY_PAUSE, VsWaitFilterBusyProc);
		}
	}

	else if (!VsClearPendingCommands(vsHnd_x))
	{
		MessageBox("Unable to clear filter - power off and try again", "Error");
		VsClose(vsHnd_x);
	}

	//	can read version info? - if not, post message and return
	else if (!VsGetFilterIdentity(vsHnd_x, (Int32 *) &m_version, (Int32 *) &m_serno, &m_minWl, &m_maxWl))
	{

		MessageBox("Unable to read filter identity - power off and try again", "Error");
		VsClose(vsHnd_x);
	}
	else if (!VsGetTemperature(vsHnd_x, &m_filterTemperature))
	{
		MessageBox("Unable to read filter temperature - power off and try again", "Error");
		VsClose(vsHnd_x);
	}
	else
	{
		m_fConnect = 1;
	}

	return m_fConnect;
}


BOOL CVsGuiDlg::UpdateFilterInfoToGui(void)
{
	CString wlStr;
	CString sernoStr;
	CString versionStr;
	CString minWlStr;
	CString maxWlStr;
	CString tempStr;

	if (m_fConnect)
	{
		SetWlSliderLimits(m_minWl, m_maxWl);

		m_wl = 1;
		if (!VsGetWavelength(vsHnd_x, &m_wl))
			MessageBox("Unable to read wavelength from filter", "Error");

		if (m_wl < 0)
		{
			MessageBox("Filter wavelength undefined, or tuning error", "Error");
			wlStr = "not set";
		}
		else
		{
			wlStr.Format("%7.2lf", m_wl);
			SetWlSliderPos(m_wl);
		}

		sernoStr.Format("%5d", m_serno);
		versionStr.Format("%2d", m_version);
		minWlStr.Format("%7.2lf", m_minWl);
		maxWlStr.Format("%7.2lf", m_maxWl);
		tempStr.Format("%5.2lf", m_filterTemperature);
	}
	else
	{
		sernoStr = "<unknown>";
		versionStr = "<unknown>";
		minWlStr = "<min>";
		maxWlStr = "<max>";
		wlStr = "";
		tempStr = "";
	}

	SetDlgItemText(IDC_SERNO, sernoStr);
	SetDlgItemText(IDC_MINWL, minWlStr);
	SetDlgItemText(IDC_MAXWL, maxWlStr);
	SetDlgItemText(IDC_VERSION, versionStr);
	SetDlgItemText(IDC_WL, wlStr);
	SetDlgItemText(IDC_FILTERTEMP, tempStr);

	GetDlgItem(IDC_SERNO)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_MINWL)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_MAXWL)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_VERSION)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_WL)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_FILTERTEMP)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_WLSLIDER)->EnableWindow(m_fConnect);
	GetDlgItem(IDC_SWEEP)->EnableWindow(m_fConnect);
	return TRUE;
}


void CVsGuiDlg::SetWlSliderPos(double wl)
{
	int iwl = (int) (wl + 0.5);
	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_WLSLIDER);
	pccs->SetPos(iwl);
}


void CVsGuiDlg::SetWlSliderLimits(double min, double max)
{
	int iminWl = (int) (min + 0.5);
	int imaxWl = (int) (max + 0.5);
	int iwlRange = (imaxWl - iminWl);
	int iwlStep;

	if (iwlRange < 20)
		iwlStep = 2;
	else if (iwlRange < 100)
		iwlStep = 10;
	else if (iwlRange < 500)
		iwlStep = 20;
	else 
		iwlStep = 50;

	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_WLSLIDER);
	pccs->SetRange(iminWl, imaxWl);
	pccs->SetPageSize(iwlStep);
}


void CVsGuiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVsGuiDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CVsGuiDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CVsGuiDlg::OnConfigure() 
{
	if (timerActive_x)
		::KillTimer(hWnd_x, hTimer_x);

	VsDisconnect(vsHnd_x);

	CVsConfigDlg cvcDlg;
	cvcDlg.Initialize(m_serPortStr);
	if (cvcDlg.DoModal() == IDOK)
		cvcDlg.Retrieve(m_serPortStr);

	Connect();
	UpdateFilterInfoToGui();
}


void CVsGuiDlg::OnKillfocusWl() 
{
	UpdateWavelengthFromTextBox();
}

void CVsGuiDlg::UpdateWavelengthFromTextBox(void)
{
	if (m_initializingDialog)
		return;

	double wl;
	CString wlText;
	GetDlgItemText(IDC_WL, wlText);
	if (sscanf(wlText, "%lf", &wl) == 1)
	{
		if (wl != m_wl)
		{
			m_wl = wl;
			if (!RequestWavelengthUpdate(m_wl))
			{
				CString msg;
				msg.Format("Unable to set wavelength to %7.2lf nm", m_wl);
				MessageBox(msg, "Error");
			}

			SetWlSliderPos(m_wl);

			//	wait/force dialog update
			MSG omsg;
			while (PeekMessage(&omsg, NULL, 0, 0, PM_REMOVE))
			{
				::SendMessage(omsg.hwnd,
					omsg.message,
					omsg.wParam,
					omsg.lParam);
			}
		}
	}
	else
	{
		CString msg;
		msg.Format("Cannot interpret %s as a wavelength - please check and re-enter", wlText);
		MessageBox(msg, "Error");
	}
}

void CVsGuiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_WLSLIDER);
	double wl = pccs->GetPos();

	if (wl == m_wl)
		return;

	m_wl = wl;
	if (!RequestWavelengthUpdate(m_wl))
	{
		CString msg;
		msg.Format("Unable to set wavelength to %7.2lf nm", m_wl);
		MessageBox(msg, "Error");

	}

	CString wlStr;
	wlStr.Format("%7.2lf", m_wl);
	SetDlgItemText(IDC_WL, wlStr);

	//	wait/force dialog update
	MSG omsg;
	while (PeekMessage(&omsg, NULL, 0, 0, PM_REMOVE))
	{
		::SendMessage(omsg.hwnd,
			omsg.message,
			omsg.wParam,
			omsg.lParam);
	}
}


BOOL CVsGuiDlg::RequestWavelengthUpdate(double wl)
{
	if ((wl < m_minWl) ||
		(wl > m_maxWl))
		return FALSE;

	if (!VsIsConnected(vsHnd_x))
		return FALSE;

	//	this is very subtle (believe it or not)
	//	you would think we could just write to the VS if ReadyForVsWrite() is true
	//	but consider effect of timer interrupt immediately after if() is evaluated FALSE
	//	... the chain would be broken. This approach is free of that problem.

	//	actual write happens in timer service routine
	updateWl_x = wl;
	updatePending_x = TRUE;

	//	if necessary, start timer - actual update is done in VsWriteTimerProc
	if (!timerActive_x)
	{
		timerActive_x = TRUE;
		hWnd_x = GetSafeHwnd();
		hTimer_x = SetTimer(VS_WRITE_TIMER_ID, VS_SHORT_PAUSE, VsWriteTimerProc);
		return (hTimer_x != NULL);
	}
	else
	{
		//	it will be picked up when the timer completes
		return TRUE;
	}

}


enum filterState GetFilterState(LPCSTR port)
{
	VS_HANDLE hVs;
	Int32 err;
	int stat = VsOpen(&hVs, port, &err);
	VsClose(hVs);
	if (hVs > 0)
		return NotOpen;
	if (err == 117)
		return NotReady;
	else
		return NotFound;
}


VOID CALLBACK VsWaitFilterBusyProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//	this shares window and timerID with the other proc, since they are NEVER active at same time
	if ((hWnd == hWnd_x) && (idEvent == hTimer_x))
	{
		KillTimer(hWnd_x, hTimer_x);
		timerActive_x = FALSE;

		//	if filter is not ready, wait and try again
		enum filterState fs = GetFilterState(port_x);
		if (fs == NotReady)
		{
			timerActive_x = TRUE;		//	should already be so
			hTimer_x = SetTimer(hWnd_x, VS_WAIT_FILTER_TIMER_ID, VS_WAIT_FILTER_BUSY_PAUSE, VsWaitFilterBusyProc);
			return;
		}
		else if (fs == NotFound)
		{
			//	post message box and call off the search
			::MessageBox(hWnd_x, "Filter that was initialized has been disconnected", "Filter disconnected", MB_OK);
		}
		else
		{
			CVsGuiDlg *pDlg = (CVsGuiDlg *) CWnd::FromHandle(hWnd_x);
			pDlg->Connect();
			pDlg->UpdateFilterInfoToGui();
		}
	}
}


VOID CALLBACK VsWriteTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if ((hWnd == hWnd_x) && (idEvent == hTimer_x))
	{
		KillTimer(hWnd_x, hTimer_x);
		timerActive_x = FALSE;

		//	if filter is not ready for wavelength update, delay until we are
		if (updatePending_x && !VsIsReady(vsHnd_x))
		{
			timerActive_x = TRUE;		//	should already be so
			hTimer_x = SetTimer(hWnd_x, VS_WRITE_TIMER_ID, VS_WRITE_ACTIVE_PAUSE, VsWriteTimerProc);
			return;
		}

		//	now we can update the filter
		if (updatePending_x)
		{
			if (!VsSetWavelength(vsHnd_x, updateWl_x, FALSE))
			{
				char msg[64];
				sprintf(msg, "Unable to set savelength to %8.3lf nm", updateWl_x);
				MessageBox(hWnd_x, msg, "Error",MB_OK);
			}
			TRACE("Writing wavelength of %lf to VariSpec\n", updateWl_x);

			//	update is no longer pending, now that write is done
			updatePending_x = FALSE;

			//	start another timer for this most recent wavelength write
			timerActive_x = TRUE;
			hTimer_x = SetTimer(hWnd_x, VS_WRITE_TIMER_ID, VS_WRITE_ACTIVE_PAUSE, VsWriteTimerProc);
		}
	}
}

void CVsGuiDlg::OnSweep() 
{
	CVsSweep *pdlg = new CVsSweep(m_minWl, m_maxWl, vsHnd_x);
	pdlg->DoModal();

}

void CVsGuiDlg::OnOK() 
{
	HWND hFocusWnd = ::GetFocus();
	HWND hWlWnd = GetDlgItem(IDC_WL)->m_hWnd;

	if (hFocusWnd == hWlWnd)
		UpdateWavelengthFromTextBox();
	else
		CDialog::OnOK();
}
