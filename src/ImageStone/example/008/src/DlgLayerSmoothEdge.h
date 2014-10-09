#pragma once
#include "resource.h"

//================================================================================
class DlgLayerSmoothEdge : public CDialog
{
public:
	DlgLayerSmoothEdge(UINT nIDD, int nMin, int nMax, int nInit, CWnd* pParent = NULL)
        : CDialog(nIDD, pParent)
    {
        m_nMin = nMin ;
        m_nMax = nMax ;
        m_nInit = nInit ;
        m_nScroll_1 = FClamp(nInit, nMin, nMax) ;
    }

	int		m_nScroll_1 ;

protected:
	int		m_nMin, m_nMax, m_nInit ;

    //{{AFX_MSG(DlgPhoXoBase)
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CSliderCtrl   * pSlide = (CSliderCtrl*)GetDlgItem(IDC_SCROLL_PARAM) ;
        if (pSlide)
            pSlide->SetRange (m_nMin, m_nMax) ;

        CDialog::DoDataExchange(pDX);
        DDX_Text(pDX, IDC_SHOWPARAM, m_nScroll_1);
        DDX_Slider(pDX, IDC_SCROLL_PARAM, m_nScroll_1);
    }
};
