#include "stdafx.h"
#include "DlgSaveFile.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgSaveFile

DlgSaveFile::DlgSaveFile(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE ;
	m_ofn.lpTemplateName = MAKEINTRESOURCE (IDD_SAVE_IMAGEFILE) ;
	m_ofn.lpstrTitle = _T("Save Image As") ;

    // file extend name
    int      nFileFormat ;
    m_ImageType = IMG_BMP ;
    switch (m_ImageType)
    {
        case IMG_BMP : lstrcpy (m_DefExt, _T("bmp")) ; nFileFormat = 1 ; break ;
        case IMG_JPG : lstrcpy (m_DefExt, _T("jpg")) ; nFileFormat = 2 ; break ;
        case IMG_GIF : lstrcpy (m_DefExt, _T("gif")) ; nFileFormat = 3 ; break ;
        case IMG_PNG : lstrcpy (m_DefExt, _T("png")) ; nFileFormat = 4 ; break ;
        case IMG_TIF : lstrcpy (m_DefExt, _T("tif")) ; nFileFormat = 5 ; break ;
        case IMG_PCX : lstrcpy (m_DefExt, _T("pcx")) ; nFileFormat = 6 ; break ;
        case IMG_TGA : lstrcpy (m_DefExt, _T("tga")) ; nFileFormat = 7 ; break ;
        case IMG_XPM : lstrcpy (m_DefExt, _T("xpm")) ; nFileFormat = 8 ; break ;
        case IMG_PHOXO : lstrcpy (m_DefExt, _T("oXo")) ; nFileFormat = 9 ; break ;
        default : ASSERT(FALSE) ; lstrcpy (m_DefExt, _T("jpg")) ; nFileFormat = 2 ; break ;
    }
    m_ofn.nFilterIndex = nFileFormat ;
    m_ofn.lpstrDefExt = m_DefExt ;
    m_nQuality = 85 ;

	m_nColorIndex = 4 ; // 24 bit
}

BEGIN_MESSAGE_MAP(DlgSaveFile, CFileDialog)
	//{{AFX_MSG_MAP(DlgSaveFile)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_1, OnColorBitsChanged)
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_4, OnColorBitsChanged)
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_8, OnColorBitsChanged)
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_16, OnColorBitsChanged)
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_24, OnColorBitsChanged)
	ON_BN_CLICKED(IDC_SAVEFILE_BITS_32, OnColorBitsChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void DlgSaveFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgSaveFile)
	DDX_Control(pDX, IDC_SAVE_FILE_QUALITY, m_FileQuality);
	DDX_Text(pDX, IDC_SHOW_SAVE_QUALITY, m_nQuality) ; // show value
	//}}AFX_DATA_MAP
}
//=============================================================================
BOOL DlgSaveFile::OnInitDialog() 
{
	CFileDialog::OnInitDialog();

	m_FileQuality.SetScrollRange (1, 100, FALSE) ;
	m_FileQuality.SetScrollPos (m_nQuality) ;
	CheckRadioButton (IDC_SAVEFILE_BITS_1, IDC_SAVEFILE_BITS_32, IDC_SAVEFILE_BITS_24) ;

    UpdateCtrl (m_ofn.nFilterIndex) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//=============================================================================
void DlgSaveFile::UpdateCtrl (int nFilterIndex)
{
    m_nColorIndex = 4 ; // 24λɫ
    CheckRadioButton (IDC_SAVEFILE_BITS_1, IDC_SAVEFILE_BITS_32, IDC_SAVEFILE_BITS_24) ;

    BOOL   bC1=FALSE, bC4=FALSE, bC8=FALSE, bC16=FALSE, bC24=FALSE, bC32=FALSE, bQua=FALSE ;
    if (nFilterIndex == 1) // bmp file
    {
        m_ImageType = IMG_BMP ;
        bC1=bC4=bC8=bC16=bC24=bC32=TRUE ;
        lstrcpy (m_DefExt, _T("bmp")) ;
    }
    else if (nFilterIndex == 2) // tga file
    {
        m_ImageType = IMG_TGA ;
        bC8=bC16=bC24=bC32=TRUE ;
        lstrcpy (m_DefExt, _T("tga")) ;
    }
    else if (nFilterIndex == 3) // tiff file
    {
        m_ImageType = IMG_TIF ;
        bC8=bC24=bC32=TRUE ;
        lstrcpy (m_DefExt, _T("tif")) ;
    }
    else if (nFilterIndex == 4) // png file
    {
        m_ImageType = IMG_PNG ;
        bC1=bC4=bC8=bC16=bC24=bC32=TRUE ;
        lstrcpy (m_DefExt, _T("png")) ;
    }
    else if (nFilterIndex == 5) // jpg file
    {
        m_ImageType = IMG_JPG ;
        bC24=TRUE ;
        lstrcpy (m_DefExt, _T("jpg")) ;
    }

    m_FileQuality.EnableWindow (bQua) ;
    GetDlgItem(IDC_SAVEFILE_BITS_1)->EnableWindow (bC1) ;
    GetDlgItem(IDC_SAVEFILE_BITS_4)->EnableWindow (bC4) ;
    GetDlgItem(IDC_SAVEFILE_BITS_8)->EnableWindow (bC8) ;
    GetDlgItem(IDC_SAVEFILE_BITS_16)->EnableWindow (bC16) ;
    GetDlgItem(IDC_SAVEFILE_BITS_24)->EnableWindow (bC24) ;
    GetDlgItem(IDC_SAVEFILE_BITS_32)->EnableWindow (bC32) ;
}
//=============================================================================
BOOL DlgSaveFile::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	OFNOTIFY	* notify = (OFNOTIFY *)lParam ;

	if (notify->hdr.code == CDN_TYPECHANGE)
	{
        UpdateCtrl (notify->lpOFN->nFilterIndex) ;
        return TRUE ;
	}
	return CFileDialog::OnNotify(wParam, lParam, pResult);
}
//=============================================================================
void DlgSaveFile::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Get Current Position
	if ((nSBCode != SB_THUMBPOSITION) && (nSBCode != SB_THUMBTRACK))
		nPos = pScrollBar->GetScrollPos () ;
	switch (nSBCode)
	{
		case SB_LINELEFT  : nPos-- ; break ;
		case SB_LINERIGHT : nPos++ ; break ;
		case SB_PAGELEFT  : nPos -= 5 ; break ;
		case SB_PAGERIGHT :	nPos += 5 ; break ;
	}
	int		minpos, maxpos ;
	pScrollBar->GetScrollRange (&minpos, &maxpos) ;
	m_nQuality = FClamp ((int)nPos, minpos, maxpos) ; // Bound nPos
	pScrollBar->SetScrollPos (m_nQuality) ; // update scrollbar's position

	// show current value
	this->UpdateData (FALSE) ;
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//=============================================================================
void DlgSaveFile::OnColorBitsChanged()
{
	if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_1) != 0)
		m_nColorIndex = 0 ;
	else if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_4) != 0)
		m_nColorIndex = 1 ;
	else if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_8) != 0)
		m_nColorIndex = 2 ;
	else if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_16) != 0)
		m_nColorIndex = 3 ;
	else if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_24) != 0)
		m_nColorIndex = 4 ;
	else if (IsDlgButtonChecked(IDC_SAVEFILE_BITS_32) != 0)
		m_nColorIndex = 5 ;
    else
    {
        ASSERT(FALSE) ;
    }
}
//=============================================================================
