#pragma once

//================================================================================
class DlgSelectFile : public CFileDialog
{
public:
	DlgSelectFile(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL) ;

private:
    FCObjImage   m_imgPreview ;

    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	// Generated message map functions
	//{{AFX_MSG(DlgSelectFile)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
