// VsSweep.cpp : implementation file
//

#include "stdafx.h"
#include "VsGui.h"
#include "VsSweep.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVsSweep dialog


CVsSweep::CVsSweep(double minWl, double maxWl, VS_HANDLE hnd, CWnd* pParent /*=NULL*/)
	: CDialog(CVsSweep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVsSweep)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_minWl = minWl;
	m_maxWl = maxWl;
	m_minDwell = 20;
	m_maxDwell = 10000;
	m_nomDwell = 100;
	m_vsHnd = hnd;
}


void CVsSweep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVsSweep)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVsSweep, CDialog)
	//{{AFX_MSG_MAP(CVsSweep)
	ON_EN_KILLFOCUS(IDC_SWEEPSTEPNM, OnKillfocusSweepstepnm)
	ON_EN_KILLFOCUS(IDC_SWEEPFROMNM, OnKillfocusSweepfromnm)
	ON_EN_KILLFOCUS(IDC_SWEEPTONM, OnKillfocusSweeptonm)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_SWEEPDWELL, OnKillfocusSweepdwell)
	ON_BN_CLICKED(IDC_CYCLEGO, OnCyclego)
	ON_BN_CLICKED(IDC_CYCLESTOP, OnCyclestop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVsSweep message handlers


void CVsSweep::OnCyclestop() 
{
	m_quit = TRUE;	
}


void CVsSweep::OnCyclego() 
{
	BOOL loop = ((CButton *) GetDlgItem(IDC_SWEEPCYCLE))->GetCheck();
	GetDlgItem(IDC_CYCLEGO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CYCLESTOP)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_SWEEPFROMNM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SWEEPTONM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SWEEPSTEPNM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SWEEPDWELL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SWEEPSLIDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SWEEPCYCLE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	GetDlgItem(IDC_CYCLESTOP)->SetFocus();

	CString str;
	double from, to, step, w;
	int nsteps, i;
	int dwell;

	GetDlgItemText(IDC_SWEEPFROMNM, str);
	sscanf(str, "%lf", &from);

	GetDlgItemText(IDC_SWEEPTONM, str);
	sscanf(str, "%lf", &to);

	GetDlgItemText(IDC_SWEEPSTEPNM, str);
	sscanf(str, "%lf", &step);

	GetDlgItemText(IDC_SWEEPDWELL, str);
	sscanf(str, "%d", &dwell);

	nsteps = (int) (1 + (to - from) / step);
	m_quit = FALSE;
	
	do
	{
		for (i=0; i<nsteps; ++i)
		{
			w = from + step * i;
			if (m_vsHnd)
				VsSetWavelength(m_vsHnd, w, TRUE);

			str.Format("Tuned to %5.1lf nm", w);
			SetDlgItemText(IDC_SWEEPSTATUS, str);
			Sleep(dwell);

			//	wait/force dialog update
			MSG omsg;
			while (PeekMessage(&omsg, NULL, 0, 0, PM_REMOVE))
			{
				::SendMessage(omsg.hwnd,
					omsg.message,
					omsg.wParam,
					omsg.lParam);
			}

			if (m_quit)
			{
				loop = FALSE;
				break;
			}
		}
	}
	while (loop)
		;

	GetDlgItem(IDC_CYCLEGO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CYCLEGO)->SetFocus();
	GetDlgItem(IDC_CYCLESTOP)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_SWEEPFROMNM)->EnableWindow();
	GetDlgItem(IDC_SWEEPTONM)->EnableWindow();
	GetDlgItem(IDC_SWEEPSTEPNM)->EnableWindow();
	GetDlgItem(IDC_SWEEPDWELL)->EnableWindow();
	GetDlgItem(IDC_SWEEPSLIDER)->EnableWindow();
	GetDlgItem(IDC_SWEEPCYCLE)->EnableWindow();
	GetDlgItem(IDOK)->EnableWindow();
	SetDlgItemText(IDC_SWEEPSTATUS, "Idle");

}


void CVsSweep::OnKillfocusSweepstepnm() 
{
	CString stepNmStr;
	double stepNm;
	GetDlgItemText(IDC_SWEEPSTEPNM, stepNmStr);
	if ((sscanf(stepNmStr, "%lf", &stepNm) < 1) ||	
		(fabs(stepNm) > (m_maxWl - m_minWl)))
	{
		CString msg;
		msg.Format("Cannot set wavelength step to %s; check the value and re-enter", stepNmStr);
		MessageBox(msg, "Error");
		stepNmStr.Format("%5.1lf", m_maxWl - m_minWl);
		SetDlgItemText(IDC_SWEEPTONM, stepNmStr);
	}
}


void CVsSweep::OnKillfocusSweepfromnm() 
{
	CString fromNmStr;
	double fromNm;
	GetDlgItemText(IDC_SWEEPFROMNM, fromNmStr);
	if ((sscanf(fromNmStr, "%lf", &fromNm) < 1) ||	
		(fromNm < m_minWl) ||
		(fromNm > m_maxWl))
	{
		CString msg;
		msg.Format("Cannot set min wavelength to %s; check the value and re-enter", fromNmStr);
		MessageBox(msg, "Error");
		fromNmStr.Format("%5.1lf", m_maxWl);
		SetDlgItemText(IDC_SWEEPTONM, fromNmStr);
	}
}


void CVsSweep::OnKillfocusSweeptonm() 
{
	CString toNmStr;
	double toNm;
	GetDlgItemText(IDC_SWEEPTONM, toNmStr);
	if ((sscanf(toNmStr, "%lf", &toNm) < 1) ||	
		(toNm < m_minWl) ||
		(toNm > m_maxWl))
	{
		CString msg;
		msg.Format("Cannot set min wavelength to %s; check the value and re-enter", toNmStr);
		MessageBox(msg, "Error");
		toNmStr.Format("%5.1lf", m_minWl);
		SetDlgItemText(IDC_SWEEPTONM, toNmStr);
	}
}


void CVsSweep::OnKillfocusSweepdwell() 
{
	CString dwellStr;
	int dwell;
	GetDlgItemText(IDC_SWEEPDWELL, dwellStr);
	if ((sscanf(dwellStr, "%d", &dwell) < 1) ||	
		(dwell < m_minDwell) ||
		(dwell > m_maxDwell))
	{
		CString msg;
		msg.Format("Cannot set dwell to %s; check the value and re-enter", dwellStr);
		MessageBox(msg, "Error");
		dwellStr.Format("%d", m_nomDwell);
		dwell = m_nomDwell;
		SetDlgItemText(IDC_SWEEPTONM, dwellStr);
	}
	SetDwellSliderPosition(dwell);
}


void CVsSweep::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//	do base class default first ...	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//	get slider position
	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_SWEEPSLIDER);
	int pos = pccs->GetPos();
	int dwellMs = (int) (pow(10., (m_sliderOff + (double) pos) / m_sliderSf) + 0.5);

	CString dwellStr;
	dwellStr.Format("%d", dwellMs);
	SetDlgItemText(IDC_SWEEPDWELL, dwellStr);

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


BOOL CVsSweep::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//	establish lower / upper dwell times, in ms
	SetDwellSliderLimits(m_minDwell, m_maxDwell);
	SetDwellSliderPosition(m_nomDwell);
	CString str;
	str.Format("%d", m_nomDwell);
	SetDlgItemText(IDC_SWEEPDWELL, str);

	str.Format("%5.1lf", m_minWl);
	SetDlgItemText(IDC_SWEEPFROMNM, str);

	str.Format("%5.1lf", m_maxWl);
	SetDlgItemText(IDC_SWEEPTONM, str);

	str.Format("%5.1lf", (m_maxWl - m_minWl) / 10);
	SetDlgItemText(IDC_SWEEPSTEPNM, str);

	GetDlgItem(IDC_CYCLEGO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CYCLESTOP)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CVsSweep::SetDwellSliderLimits(int min, int max)
{
	double logMin, logMax;
	logMin = log10((double) min);
	logMax = log10((double) max);
	m_sliderSf = 100. / (logMax - logMin);

	int iscaleMin = (int) (logMin * m_sliderSf + 0.5);
	int iscaleMax = (int) (logMax * m_sliderSf + 0.5);
	int iscaleStep = 5;
	m_sliderOff = logMin * m_sliderSf - iscaleMin;

	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_SWEEPSLIDER);
	pccs->SetRange(iscaleMin, iscaleMax);
	pccs->SetPageSize(iscaleStep);
}


void CVsSweep::SetDwellSliderPosition(int dwell)
{
	double logPos = log10((double) dwell);
	int iscalePos = (int) (logPos * m_sliderSf + 0.5);

	CSliderCtrl *pccs = (CSliderCtrl *) GetDlgItem(IDC_SWEEPSLIDER);
	pccs->SetPos(iscalePos);
}

