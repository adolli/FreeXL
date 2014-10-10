#if !defined(AFX_DLGAPPSETTING_H__17FECF8B_86F4_4690_A59F_3BE0C4B690A1__INCLUDED_)
#define AFX_DLGAPPSETTING_H__17FECF8B_86F4_4690_A59F_3BE0C4B690A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAppSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgAppSetting dialog

class DlgAppSetting : public CDialog
{
// Construction
public:
	DlgAppSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgAppSetting)
	enum { IDD = IDD_APP_SETTING };
	int		m_nUndoLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgAppSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgAppSetting)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAPPSETTING_H__17FECF8B_86F4_4690_A59F_3BE0C4B690A1__INCLUDED_)
