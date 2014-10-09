#pragma once

// DlgSaveFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgSaveFile dialog

class DlgSaveFile : public CFileDialog
{
public:
	DlgSaveFile(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

    IMAGE_TYPE     m_ImageType ;
    int      m_nQuality ;
	int      m_nColorIndex ;

private:
// Dialog Data
	//{{AFX_DATA(DlgSaveFile)
	CScrollBar	m_FileQuality;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSelectFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

private:
	TCHAR      m_DefExt[32] ;
    void  UpdateCtrl (int nFilterIndex) ;
	// Generated message map functions
	//{{AFX_MSG(DlgSaveFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnColorBitsChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
