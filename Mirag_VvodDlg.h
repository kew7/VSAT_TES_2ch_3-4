// Mirag_VvodDlg.h : header file
//

#if !defined(AFX_MIRAG_VVODDLG_H__AD02CE0B_F68D_11D4_AB44_00A0CCD50301__INCLUDED_)
#define AFX_MIRAG_VVODDLG_H__AD02CE0B_F68D_11D4_AB44_00A0CCD50301__INCLUDED_

#include "MDevice.h"
#include "Abonent.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CMirag_VvodDlg dialog

class CAbonentDlg : public CDialog
{
// Construction
public:

    CAbonent Ocean;
	long BytesDone1,BytesDone2;
	BOOL flWork;
	CMDevice Mirag;
	CAbonentDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMirag_VvodDlg)
	enum { IDD = IDD_MIRAG_VVOD_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMirag_VvodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMirag_VvodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBPusk();
	afx_msg void OnMesCount(int def, LPARAM Block );
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIRAG_VVODDLG_H__AD02CE0B_F68D_11D4_AB44_00A0CCD50301__INCLUDED_)
