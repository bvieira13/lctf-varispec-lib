#if !defined(AFX_VSSWEEP_H__F4832D6D_70D8_4414_B3CE_44A37F4C88AD__INCLUDED_)
#define AFX_VSSWEEP_H__F4832D6D_70D8_4414_B3CE_44A37F4C88AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VsSweep.h : header file
//

#include "vsdrvr.h"

/////////////////////////////////////////////////////////////////////////////
// CVsSweep dialog

class CVsSweep : public CDialog
{
// Construction
public:
	CVsSweep(double minWl, double maxWl, VS_HANDLE hnd, CWnd* pParent = NULL);   // standard constructor
	void SetDwellSliderLimits(int min, int max);
	void SetDwellSliderPosition(int pos);
	double m_sliderSf;
	double m_sliderOff;
	double m_minWl;
	double m_maxWl;
	VS_HANDLE m_vsHnd;
	BOOL m_quit;


	int	m_minDwell;
	int m_maxDwell;
	int m_nomDwell;

// Dialog Data
	//{{AFX_DATA(CVsSweep)
	enum { IDD = IDD_SWEEPDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVsSweep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVsSweep)
	afx_msg void OnCyclegostop();
	afx_msg void OnKillfocusSweepstepnm();
	afx_msg void OnKillfocusSweepfromnm();
	afx_msg void OnKillfocusSweeptonm();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusSweepdwell();
	afx_msg void OnCyclego();
	afx_msg void OnCyclestop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSSWEEP_H__F4832D6D_70D8_4414_B3CE_44A37F4C88AD__INCLUDED_)
