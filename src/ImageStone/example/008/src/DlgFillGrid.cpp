#include "stdafx.h"
#include "DlgFillGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// static member initialize
RGBQUAD DlgFillGrid::m_crFill1 = FCColor::crBlack() ;
RGBQUAD DlgFillGrid::m_crFill2 = FCColor::crWhite() ;
int DlgFillGrid::m_nGridWidth = 5 ;


BEGIN_MESSAGE_MAP(DlgFillGrid, CDialog)
	//{{AFX_MSG_MAP(DlgFillGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//=============================================================================
void DlgFillGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
    UINT        nCtrl[2] = {IDC_FILLGRID_COLOR_1, IDC_FILLGRID_COLOR_2} ;
    COLORREF    crFill[2] = {RGBQUADtoRGB(DlgFillGrid::m_crFill1),
                             RGBQUADtoRGB(DlgFillGrid::m_crFill2)} ;
    for (int i=0 ; i < 2 ; i++)
    {
        CWnd		* pWnd = GetDlgItem (nCtrl[i]) ;
        CRect		rcWnd ;
        pWnd->GetClientRect (rcWnd) ;
        pWnd->ClientToScreen (rcWnd) ;
        ScreenToClient (rcWnd) ;
        if (rcWnd.PtInRect(point))
        {
            CColorDialog	dlg (crFill[i], CC_FULLOPEN) ;
            if (dlg.DoModal() == IDOK)
            {
                if (i == 0)
                    DlgFillGrid::m_crFill1 = RGBtoRGBQUAD(dlg.GetColor()) ;
                else if (i == 1)
                    DlgFillGrid::m_crFill2 = RGBtoRGBQUAD(dlg.GetColor()) ;
                this->Invalidate(FALSE) ;
            }
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}
//=============================================================================
void DlgFillGrid::OnPaint() 
{
    CPaintDC	dc (this) ;
    CBrush      blackBr(RGB(0,0,0)) ;
    UINT        nCtrl[2] = {IDC_FILLGRID_COLOR_1, IDC_FILLGRID_COLOR_2} ;
    COLORREF    crFill[2] = {RGBQUADtoRGB(DlgFillGrid::m_crFill1),
                             RGBQUADtoRGB(DlgFillGrid::m_crFill2)} ;
    for (int i=0 ; i < 2 ; i++)
    {
        CWnd		* pWnd = GetDlgItem (nCtrl[i]) ;
        CRect		rcWnd ;
        pWnd->GetClientRect (rcWnd) ;
        pWnd->ClientToScreen (rcWnd) ;
        ScreenToClient (rcWnd) ;
        dc.FillRect (rcWnd, &CBrush(crFill[i])) ;
        dc.FrameRect (rcWnd, &blackBr) ;
    }
}
//=============================================================================
