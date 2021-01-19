// Mirag_Vvod.h : main header file for the MIRAG_VVOD application
//

#if !defined(AFX_MIRAG_VVOD_H__AD02CE09_F68D_11D4_AB44_00A0CCD50301__INCLUDED_)
#define AFX_MIRAG_VVOD_H__AD02CE09_F68D_11D4_AB44_00A0CCD50301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMirag_VvodApp:
// See Mirag_Vvod.cpp for the implementation of this class
//

class CMirag_VvodApp : public CWinApp
{
public:
	CMirag_VvodApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMirag_VvodApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMirag_VvodApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIRAG_VVOD_H__AD02CE09_F68D_11D4_AB44_00A0CCD50301__INCLUDED_)
