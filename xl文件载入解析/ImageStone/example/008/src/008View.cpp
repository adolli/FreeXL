// 008View.cpp : implementation of the CMy008View class
//

#include "stdafx.h"
#include "008.h"

#include "008Doc.h"
#include "008View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy008View

IMPLEMENT_DYNCREATE(CMy008View, CScrollView)

BEGIN_MESSAGE_MAP(CMy008View, CScrollView)
	//{{AFX_MSG_MAP(CMy008View)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy008View construction/destruction

CMy008View* GetCurrentView()
{
    CMy008View  	* pArtView = NULL ;
    CMDIFrameWnd	* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd ;
    if (pFrame)
    {
        // Get the active MDI child window.
        CMDIChildWnd	* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame() ;
        if (pChild)
            pArtView = (CMy008View*)pChild->GetActiveView() ; // Get the active view attached to the active MDI child window.
    }
    return pArtView ;
}

CMy008View::CMy008View()
{
	// TODO: add construction code here

}

CMy008View::~CMy008View()
{
}

BOOL CMy008View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMy008View drawing

void CMy008View::OnDraw(CDC* pDC)
{
    CMy008Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    RECT   rc = {0,0,pDoc->m_img.Width(),pDoc->m_img.Height()} ;
    pDoc->Actual_to_Scaled (rc) ;
    FCWin32::DrawImage (pDoc->m_img, pDC->m_hDC, rc) ;
}

void CMy008View::OnInitialUpdate()
{
    CFrameWnd	* pFrame = GetParentFrame() ;
    if (pFrame)
    {
        // get max display region
        CRect		rcMax ;
        ::GetClientRect (((CMainFrame*)AfxGetMainWnd())->m_hWndMDIClient, rcMax) ;

        // test scale param
        int			n = -16 ;
        SIZE		szImg = {GetDocument()->m_img.Width(),GetDocument()->m_img.Height()} ;
        for ( ; n <= -1 ; n++)
        {
            if ((szImg.cx/-n > rcMax.Width()) || (szImg.cy/-n > rcMax.Height()))
                break ;
        }
        GetDocument()->SetZoomScale (--n) ;

        int   nNewW = szImg.cx/-n+2*(GetSystemMetrics(SM_CXFRAME)+GetSystemMetrics(SM_CXEDGE)),
              nNewH = szImg.cy/-n+2*(GetSystemMetrics(SM_CYFRAME)+GetSystemMetrics(SM_CYEDGE))+GetSystemMetrics(SM_CYCAPTION) ;
        pFrame->SetWindowPos (NULL, 0, 0, nNewW, nNewH, SWP_NOZORDER|SWP_NOMOVE) ;
    }
	CScrollView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CMy008View printing

BOOL CMy008View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy008View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy008View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMy008View diagnostics

#ifdef _DEBUG
void CMy008View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMy008View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMy008Doc* CMy008View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy008Doc)));
	return (CMy008Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy008View message handlers

void CMy008View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    // TODO: calculate the total size of this view
    CMy008Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    POINT   pt = {pDoc->m_img.Width(),pDoc->m_img.Height()} ;
    pDoc->Actual_to_Scaled (pt) ;
    SetScrollSizes(MM_TEXT, CSize(pt)) ;
    CScrollView::OnUpdate (pSender, lHint, pHint) ;
}

void CMy008View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonDown(nFlags, point);
}
