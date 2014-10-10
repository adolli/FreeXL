// 008Doc.h : interface of the CMy008Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_008DOC_H__EDD4735D_ED38_49D1_AC46_4163DC7184FE__INCLUDED_)
#define AFX_008DOC_H__EDD4735D_ED38_49D1_AC46_4163DC7184FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMy008Doc ;

/// command
class CUndoCommand
{
public:
    virtual void Execute (CMy008Doc* pDoc, FCInterface_PixelProcess* pProcessor, FCObjProgress* pProgress) ;
    virtual void Undo (CMy008Doc* pDoc) ;
    virtual void Redo (CMy008Doc* pDoc) ;
protected:
    FCObjImage   m_Undo ;
    FCObjImage   m_Redo ;
};

class CUndoCommand_Stretch : public CUndoCommand
{
    int                 m_nWidth, m_nHeight ;
    INTERPOLATION_TYPE  m_nType;
public:
    CUndoCommand_Stretch (int nWidth, int nHeight, INTERPOLATION_TYPE nType) : m_nWidth(nWidth), m_nHeight(nHeight), m_nType(nType) {}
    virtual void Execute (CMy008Doc* pDoc, FCInterface_PixelProcess* pProcessor, FCObjProgress* pProgress) ;
};

/// Document
class CMy008Doc : public CDocument,
                  public PCL_Interface_ZoomScale,
                  public PCL_Interface_Undo<CUndoCommand>
{
protected: // create from serialization only
	CMy008Doc();
	DECLARE_DYNCREATE(CMy008Doc)

// Attributes
public:
    FCObjImage   m_img ;

// Operations
public:
    enum {MAX_ZOOM = 16, MIN_ZOOM = -16} ;
    void PerformImgEffect (FCInterface_PixelProcess* pCmd) ;
    void PerformImgEffect (CUndoCommand* pCmd) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy008Doc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMy008Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    virtual void PCL_Implement_Undo (CUndoCommand* pCmd) ;
    virtual void PCL_Implement_Redo (CUndoCommand* pCmd) ;

// Generated message map functions
protected:
	//{{AFX_MSG(CMy008Doc)
    afx_msg void OnImageEffect(UINT nID);
	afx_msg void OnFileSaveAs();
	afx_msg void OnHistogram();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnAppSetting();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomActual();
	afx_msg void OnZoomOut();
	afx_msg void OnFilterSaveAscii();
	afx_msg void OnCountColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_008DOC_H__EDD4735D_ED38_49D1_AC46_4163DC7184FE__INCLUDED_)
