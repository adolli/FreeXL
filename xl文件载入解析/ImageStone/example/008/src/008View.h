// 008View.h : interface of the CMy008View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_008VIEW_H__E1582BC2_5C84_4E47_B977_4F23F8F981DA__INCLUDED_)
#define AFX_008VIEW_H__E1582BC2_5C84_4E47_B977_4F23F8F981DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMy008View : public CScrollView
{
protected: // create from serialization only
	CMy008View();
	DECLARE_DYNCREATE(CMy008View)

// Attributes
public:
	CMy008Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy008View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMy008View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy008View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 008View.cpp
inline CMy008Doc* CMy008View::GetDocument()
   { return (CMy008Doc*)m_pDocument; }
#endif

CMy008View* GetCurrentView() ;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_008VIEW_H__E1582BC2_5C84_4E47_B977_4F23F8F981DA__INCLUDED_)
