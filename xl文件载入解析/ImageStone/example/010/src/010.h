// 010.h : main header file for the 010 application
//

#if !defined(AFX_010_H__387C2007_1F56_41F3_A8E2_E68B3C3E39A4__INCLUDED_)
#define AFX_010_H__387C2007_1F56_41F3_A8E2_E68B3C3E39A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy010App:
// See 010.cpp for the implementation of this class
//

class CMy010App : public CWinApp
{
public:
	CMy010App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy010App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy010App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_010_H__387C2007_1F56_41F3_A8E2_E68B3C3E39A4__INCLUDED_)
