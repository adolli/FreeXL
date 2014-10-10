#include "stdafx.h"
#include "DlgLayerSmoothEdge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(DlgLayerSmoothEdge, CDialog)
    //{{AFX_MSG_MAP(DlgLayerSmoothEdge)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//================================================================================
void DlgLayerSmoothEdge::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
    UpdateData (TRUE) ;
    UpdateData (FALSE) ;
}
//================================================================================
