// 008.h : main header file for the 008 application
//

#if !defined(AFX_008_H__49A58106_A468_46F4_A4BD_A622A787B631__INCLUDED_)
#define AFX_008_H__49A58106_A468_46F4_A4BD_A622A787B631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy008App:
// See 008.cpp for the implementation of this class
//

class CMy008App : public CWinApp
{
public:
	CMy008App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy008App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMy008App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_008_H__49A58106_A468_46F4_A4BD_A622A787B631__INCLUDED_)
