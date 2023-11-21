#if !defined(AFX_VSCONFIGDLG_H__9E4FDC8D_ED0D_11D6_BBB5_444553540001__INCLUDED_)
#define AFX_VSCONFIGDLG_H__9E4FDC8D_ED0D_11D6_BBB5_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VsConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVsConfigDlg dialog

class CVsConfigDlg : public CDialog
{

	// Construction
public:
	CVsConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVsConfigDlg)
	enum { IDD = IDD_CONFIGDIALOG };
	//}}AFX_DATA
	BOOL	Initialize(LPCSTR serPort);
	BOOL	Retrieve(CString& serPort);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVsConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_serPortStr;
	void	SetFilterStatus(enum filterState fs);

	// Generated message map functions
	//{{AFX_MSG(CVsConfigDlg)
	afx_msg void OnSelchangeSerport();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAutodetect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSCONFIGDLG_H__9E4FDC8D_ED0D_11D6_BBB5_444553540001__INCLUDED_)
