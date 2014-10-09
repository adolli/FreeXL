#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgHistogram dialog

class DlgHistogram : public CDialog
{
// Construction
public:
	DlgHistogram(const FCObjImage & img, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgHistogram)
	CStatic	m_Range;
	CStatic	m_Pixel;
	CStatic	m_Percent;
	CStatic	m_Mid;
	CStatic	m_Count;
	CStatic	m_Ave;
	CStatic	m_staChannel;
	CComboBox	m_cmbChannel;
	CStatic	m_staShow;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgHistogram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public :
	void  UpdateText() ;
	void  InvertSelection() ;

protected:
	FCHistogram		m_Histogram ;
	FCObjImage		m_imgHisGray, m_imgHisRed, m_imgHisGreen, m_imgHisBlue ;
	FCObjImage		m_imgCurr ;
	CBitmap			m_hGray, m_hRed, m_hGreen, m_hBlue ;
	CPoint			m_ptAnchor ;

	// Generated message map functions
	//{{AFX_MSG(DlgHistogram)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeHistogramChannel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
