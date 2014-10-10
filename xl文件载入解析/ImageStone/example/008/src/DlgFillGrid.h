#pragma once
#include "resource.h"

//================================================================================
class DlgFillGrid : public CDialog
{
public:
    DlgFillGrid(CWnd* pParent=NULL) : CDialog(IDD_EFF_FILLGRID, pParent) {}

    static RGBQUAD     m_crFill1 ;
    static RGBQUAD     m_crFill2 ;
    static int         m_nGridWidth ;

protected:
    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        DDX_Text(pDX, IDC_FILLGRID_WIDTH, DlgFillGrid::m_nGridWidth);
    }

protected:
	//{{AFX_MSG(DlgFillGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
