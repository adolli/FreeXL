#pragma once

//================================================================================
class DlgImgSize : public CDialog
{
public:
	DlgImgSize(int nOldWidth, int nOldHeight, CWnd* pParent = NULL);

	int		m_nWidth ;
	int		m_nHeight ;
    int     m_nStretchType ;

protected :
    int      m_nPerWidth ;
    int      m_nPerHeight ;
    BOOL     m_bRatio;

    int      m_nOldW ;
    int      m_nOldH ;

protected:
    virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	//{{AFX_MSG(DlgImgSize)
    afx_msg void OnChangeWidth();
    afx_msg void OnChangeHeight();
    afx_msg void OnChangeWidthScale();
    afx_msg void OnChangeHeightScale();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
