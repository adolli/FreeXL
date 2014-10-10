#include "stdafx.h"
#include "008.h"
#include "DlgImgSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=============================================================================
DlgImgSize::DlgImgSize(int nWidth, int nHeight, CWnd* pParent) : CDialog(IDD_ADJUST_SIZE, pParent)
{
    m_nOldW = FMax(1,nWidth) ; m_nOldH = FMax(1,nHeight) ;
    m_nStretchType = 1 ;
    m_bRatio = TRUE ;

    m_nHeight = m_nOldH ; m_nWidth = m_nOldW ;
    m_nPerHeight = m_nPerWidth = 100 ;
}
//=============================================================================
void DlgImgSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIZE_SCALE_WIDTH, m_nPerWidth);
	DDX_Text(pDX, IDC_SIZE_SCALE_HEIGHT, m_nPerHeight);
	DDX_Text(pDX, IDC_SIZE_NEWHEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_SIZE_NEWWIDTH, m_nWidth);
    DDX_Check(pDX, IDC_SIZE_RATIO, m_bRatio);
    DDX_Radio(pDX, IDC_STRETCH_NORMAL, m_nStretchType);
}
//=============================================================================
BEGIN_MESSAGE_MAP(DlgImgSize, CDialog)
	//{{AFX_MSG_MAP(DlgImgSize)
    ON_EN_CHANGE(IDC_SIZE_NEWWIDTH, OnChangeWidth)
    ON_EN_CHANGE(IDC_SIZE_NEWHEIGHT, OnChangeHeight)
    ON_EN_CHANGE(IDC_SIZE_SCALE_WIDTH, OnChangeWidthScale)
    ON_EN_CHANGE(IDC_SIZE_SCALE_HEIGHT, OnChangeHeightScale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//=============================================================================
void DlgImgSize::OnChangeWidth()
{
    UpdateData (TRUE) ;
    m_nPerWidth = FRound (100 * m_nWidth / (double)m_nOldW) ;
    if (m_bRatio)
    {
        m_nPerHeight = m_nPerWidth ;
        m_nHeight = FRound (m_nOldH * m_nWidth / (double)m_nOldW) ;
    }
    UpdateData (FALSE) ;
}
void DlgImgSize::OnChangeHeight()
{
    UpdateData (TRUE) ;
    m_nPerHeight = FRound (100 * m_nHeight / (double)m_nOldH) ;
    if (m_bRatio)
    {
        m_nPerWidth = m_nPerHeight ;
        m_nWidth = FRound (m_nOldW * m_nHeight / (double)m_nOldH) ;
    }
    UpdateData (FALSE) ;
}
void DlgImgSize::OnChangeWidthScale()
{
    UpdateData (TRUE) ;
    m_nWidth = FRound (m_nOldW * m_nPerWidth / 100.0) ;
    if (m_bRatio)
    {
        m_nPerHeight = m_nPerWidth ;
        m_nHeight = FRound (m_nOldH * m_nPerHeight / 100.0) ;
    }
    UpdateData (FALSE) ;
}
void DlgImgSize::OnChangeHeightScale()
{
    UpdateData (TRUE) ;
    m_nHeight = FRound (m_nOldH * m_nPerHeight / 100.0) ;
    if (m_bRatio)
    {
        m_nPerWidth = m_nPerHeight ;
        m_nWidth = FRound (m_nOldW * m_nPerWidth / 100.0) ;
    }
    UpdateData (FALSE) ;
}
//=============================================================================
// 24-bit image size (KB)
static int __pcl_ImageSize (int nWidth, int nHeight)
{
    return max (1, ((nWidth * 24 + 31) / 32) * nHeight / 256) ;
}
void DlgImgSize::OnOK()
{
    UpdateData(TRUE) ;
    if ((m_nWidth <= 0) || (m_nHeight <= 0))
    {
        ::AfxMessageBox (IDS_INPUT_IMGSIZE_ZERO, MB_ICONEXCLAMATION) ;
        return ;
    }

    m_nWidth = FClamp(m_nWidth, 1, 10000) ;
    m_nHeight = FClamp(m_nHeight, 1, 10000) ;
    int     nFileSize = __pcl_ImageSize(m_nWidth,m_nHeight) ;
    if (nFileSize > 200*1024)
    {
        ::AfxMessageBox (IDS_INPUT_IMGSIZE_LARGE, MB_ICONEXCLAMATION) ;
        return ;
    }
    UpdateData(FALSE) ;
    CDialog::OnOK();
}
//=============================================================================
