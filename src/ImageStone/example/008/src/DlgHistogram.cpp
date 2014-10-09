// DlgHistogram.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgHistogram dialog


DlgHistogram::DlgHistogram(const FCObjImage & img, CWnd* pParent /*=NULL*/) : CDialog(IDD_HISTOGRAM, pParent), m_Histogram(img)
{
	//{{AFX_DATA_INIT(DlgHistogram)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    FCObjImage   imgHis ;
    imgHis.Create (256, 10, 24) ;
    imgHis.SinglePixelProcessProc (FCPixelGradientLine(CPoint(0,0), CPoint(255,0), FCColor::crBlack(), FCColor::crWhite())) ;
    m_hGray.Attach (FCWin32::CreateDDBHandle(imgHis)) ;

    imgHis.SinglePixelProcessProc (FCPixelGradientLine(CPoint(0,0), CPoint(255,0), FCColor::crBlack(), PCL_RGBA(255,0,0))) ;
    m_hRed.Attach (FCWin32::CreateDDBHandle(imgHis)) ;

    imgHis.SinglePixelProcessProc (FCPixelGradientLine(CPoint(0,0), CPoint(255,0), FCColor::crBlack(), PCL_RGBA(0,255,0))) ;
    m_hGreen.Attach (FCWin32::CreateDDBHandle(imgHis)) ;

    imgHis.SinglePixelProcessProc (FCPixelGradientLine(CPoint(0,0), CPoint(255,0), FCColor::crBlack(), PCL_RGBA(0,0,255))) ;
    m_hBlue.Attach (FCWin32::CreateDDBHandle(imgHis)) ;
}

void DlgHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgHistogram)
	DDX_Control(pDX, IDC_HISTO_RANGE, m_Range);
	DDX_Control(pDX, IDC_HISTO_PIXEL, m_Pixel);
	DDX_Control(pDX, IDC_HISTO_PERCENT, m_Percent);
	DDX_Control(pDX, IDC_HISTO_MID, m_Mid);
	DDX_Control(pDX, IDC_HISTO_COUNT, m_Count);
	DDX_Control(pDX, IDC_HISTO_AVE, m_Ave);
	DDX_Control(pDX, IDS_HISTOGRAM_CHANNEL, m_staChannel);
	DDX_Control(pDX, IDC_HISTOGRAM_CHANNEL, m_cmbChannel);
	DDX_Control(pDX, IDS_HISTOGRAM_SHOW, m_staShow);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(DlgHistogram, CDialog)
	//{{AFX_MSG_MAP(DlgHistogram)
	ON_CBN_SELCHANGE(IDC_HISTOGRAM_CHANNEL, OnChangeHistogramChannel)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgHistogram message handlers

BOOL DlgHistogram::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect        rcHis ;
	m_staShow.GetClientRect(&rcHis) ;
	m_Histogram.GetHistogramImage (rcHis.Height(), &m_imgHisGray, CHANNEL_GRAY) ;
	m_Histogram.GetHistogramImage (rcHis.Height(), &m_imgHisRed, CHANNEL_RED) ;
	m_Histogram.GetHistogramImage (rcHis.Height(), &m_imgHisGreen, CHANNEL_GREEN) ;
	m_Histogram.GetHistogramImage (rcHis.Height(), &m_imgHisBlue, CHANNEL_BLUE) ;

	m_cmbChannel.SetCurSel(0) ;
	m_imgCurr = m_imgHisGray ;
	UpdateText() ;
    OnChangeHistogramChannel() ;
    Invalidate(FALSE) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgHistogram::OnOK()
{
	m_staChannel.SetBitmap (NULL) ;
	CDialog::OnOK();
}
//================================================================================
void DlgHistogram::OnChangeHistogramChannel() 
{
	FCObjImage     * pImgChannel = &m_imgHisGray ;
	switch (m_cmbChannel.GetCurSel())
	{
		case 0 :
			pImgChannel = &m_imgHisGray ;
			m_staChannel.SetBitmap (m_hGray) ;
			break ;
		case 1 :
			pImgChannel = &m_imgHisRed ;
			m_staChannel.SetBitmap (m_hRed) ;
			break ;
		case 2 :
			pImgChannel = &m_imgHisGreen ;
			m_staChannel.SetBitmap (m_hGreen) ;
			break ;
		case 3 :
			pImgChannel = &m_imgHisBlue ;
			m_staChannel.SetBitmap (m_hBlue) ;
			break ;
	}
	m_imgCurr.CoverBlock (*pImgChannel, 0, 0) ;
	m_Histogram.SetStart(0) ;
	m_Histogram.SetEnd(255) ;
	this->UpdateText() ;
	Invalidate(FALSE) ;
}
//================================================================================
void  DlgHistogram::UpdateText()
{
	IMAGE_CHANNEL    nChannel = CHANNEL_GRAY ;
	switch (m_cmbChannel.GetCurSel())
	{
		case 1 : nChannel = CHANNEL_RED ; break ;
		case 2 : nChannel = CHANNEL_GREEN ; break ;
		case 3 : nChannel = CHANNEL_BLUE ; break ;
	}
	CString		strShow ;
	strShow.Format (_T("%i .. %i"), m_Histogram.GetStart(), m_Histogram.GetEnd()) ;
	m_Range.SetWindowText (strShow) ;
	strShow.Format (_T("%i"), (m_Histogram.GetStart() + m_Histogram.GetEnd()) / 2) ;
	m_Mid.SetWindowText (strShow) ;
	strShow.Format (_T("%i"), m_Histogram.GetPixelNumber()) ;
	m_Pixel.SetWindowText (strShow) ;

	int		nCount = m_Histogram.GetCount(nChannel) ;
	strShow.Format (_T("%i"), nCount) ;
	m_Count.SetWindowText (strShow) ;

	strShow.Format (_T("%i"), m_Histogram.GetAverage(nChannel)) ;
	m_Ave.SetWindowText (strShow) ;

	strShow.Format (_T("%i"), nCount*100/m_Histogram.GetPixelNumber()) ;
	m_Percent.SetWindowText (strShow + _T(" %")) ;
}
//================================================================================
void  DlgHistogram::InvertSelection()
{
	FCObjImage     * pImgChannel = &m_imgHisGray ;
	switch (m_cmbChannel.GetCurSel())
	{
		case 1 : pImgChannel = &m_imgHisRed ; break ;
		case 2 : pImgChannel = &m_imgHisGreen ; break ;
		case 3 : pImgChannel = &m_imgHisBlue ; break ;
	}
	m_imgCurr.CoverBlock (*pImgChannel, 0, 0) ;

	CRect		rcHis ;
	m_staShow.GetClientRect (&rcHis) ;
	CRect		rcBlk(m_Histogram.GetStart(), 0, m_Histogram.GetEnd()+1, rcHis.Height()) ;
	FCObjImage	imgBlk ;
	pImgChannel->GetSubBlock (&imgBlk, rcBlk) ;
	imgBlk.SinglePixelProcessProc (FCPixelInvert()) ;
	m_imgCurr.CoverBlock (imgBlk, rcBlk.left, rcBlk.top) ;
	this->UpdateText() ;
}

void DlgHistogram::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ptAnchor = point ;
	this->ClientToScreen (&m_ptAnchor) ;
	m_staShow.ScreenToClient (&m_ptAnchor) ;
	m_Histogram.SetStart (m_ptAnchor.x) ;
	m_Histogram.SetEnd (m_ptAnchor.x) ;
	this->InvertSelection() ;
	this->UpdateText() ;
    Invalidate(FALSE) ;
}

void DlgHistogram::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
	{
		this->ClientToScreen (&point) ;
		m_staShow.ScreenToClient (&point) ;
		if (point.x > m_ptAnchor.x)
		{
			m_Histogram.SetStart (m_ptAnchor.x) ;
			m_Histogram.SetEnd (point.x) ;
		}
		else
		{
			m_Histogram.SetStart (point.x) ;
			m_Histogram.SetEnd (m_ptAnchor.x) ;
		}
		this->InvertSelection() ;
		this->UpdateText() ;
        Invalidate(FALSE) ;
	}
	CDialog::OnMouseMove(nFlags, point);
}
//================================================================================
void DlgHistogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    CDC     * pDC = m_staShow.GetDC() ;
    FCWin32::DrawImage (m_imgCurr, pDC->m_hDC, 0, 0) ;
    m_staShow.ReleaseDC (pDC) ;
}
//================================================================================
