#pragma once

//=============================================================================
// image effect dialog
//=============================================================================
class DlgEffect : public CDialog
{
public:
	DlgEffect (const FCObjImage& img, UINT nIDTemplate, CWnd* pParent = NULL) ;
    ~DlgEffect () ;

	int		m_nScroll_1 ;
	int		m_nScroll_2 ;
	int		m_nScroll_3 ;

	int		m_nRadio_1 ;

	BOOL	m_bCheck_1 ;

    int     m_nCombo_1 ;

    int     m_nEdit1 ;
    int     m_nEdit2 ;

    std::deque<FCObjImage*>   m_listTexture ; // �����洢����Ի����е�����

	RGBQUAD m_crTone ;
    static RGBQUAD m_crBlind ;

	IMAGE_CHANNEL  GetColorChannel() ;

    // convolute
	int		m_nElement[25] ;
	int		m_nFilterScale ;
	int		m_nFilterOffset ;

    CPoint  m_pt ;

protected :
// Dialog Data
	//{{AFX_DATA(DlgEffect)
	CStatic		m_Preview;
    CSliderCtrl m_Slide_1 ;
    CSliderCtrl m_Slide_2 ;
    CSliderCtrl m_Slide_3 ;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEffect)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected :
    FCObjImage      m_Thumb ;
    FCObjImage      m_Curr ;

	double			m_fScale ; // ����ͼ��ԭͼ�ı���

    UINT            m_nDlgID ; // Ч��dialog ID

    RECT            m_rcThumb ; // thumbλͼ�ڴ����е�λ��

    bool LoadPhotoshopACF (LPCTSTR szFileName, std::deque<int>& listElem) ;
    bool SavePhotoshopACF (LPCTSTR szFileName, const std::deque<int> & listElem) ;

protected:
    virtual void OnOK();
	// Generated message map functions
	//{{AFX_MSG(DlgEffect)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnPaint();
    afx_msg void OnDataChanged();
    afx_msg void OnClickButton1();
    afx_msg void OnClickButton2();
    afx_msg void OnClickButton3();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} ;
//=============================================================================
