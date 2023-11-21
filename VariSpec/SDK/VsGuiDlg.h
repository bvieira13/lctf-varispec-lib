// VsGuiDlg.h : header file
//

#if !defined(AFX_VSGUIDLG_H__9E4FDC85_ED0D_11D6_BBB5_444553540001__INCLUDED_)
#define AFX_VSGUIDLG_H__9E4FDC85_ED0D_11D6_BBB5_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VsSweep.h"

/////////////////////////////////////////////////////////////////////////////
// CVsGuiDlg dialog

enum filterState {NotFound, NotReady, NotOpen, Open};


class CVsGuiDlg : public CDialog
{
// Construction
public:
	CVsGuiDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL	Initialize(LPCSTR serPort);
	BOOL	Retrieve(CString& serPort);
	BOOL	Connect();
	BOOL	UpdateFilterInfoToGui(void);
	BOOL	Disconnect();

// Dialog Data
	//{{AFX_DATA(CVsGuiDlg)
	enum { IDD = IDD_VSGUI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVsGuiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool FindFilter(CString comList, int& portNo);
	void UpdateWavelengthFromTextBox(void);
	void SetWlSliderPos(double wl);
	void SetWlSliderLimits(double min, double max);
	BOOL RequestWavelengthUpdate(double wl);
	HICON m_hIcon;

	bool m_initializingDialog;
	CString	m_serPortStr;
	double m_wl;
	double m_minWl;
	double m_maxWl;
	int	m_serno;
	int m_version;
	double m_filterTemperature;
	BOOL m_fConnect;


	// Generated message map functions
	//{{AFX_MSG(CVsGuiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConfigure();
	afx_msg void OnKillfocusWl();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSweep();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


enum filterState GetFilterState(LPCSTR port);
VOID CALLBACK VsWaitFilterBusyProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
VOID CALLBACK VsWriteTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSGUIDLG_H__9E4FDC85_ED0D_11D6_BBB5_444553540001__INCLUDED_)
