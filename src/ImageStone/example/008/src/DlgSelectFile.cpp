#include "stdafx.h"
#include "resource.h"
#include "DlgSelectFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DlgSelectFile::DlgSelectFile(BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
		LPCTSTR lpszFilter, CWnd* pParentWnd)
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
		dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE ;
	m_ofn.lpTemplateName = MAKEINTRESOURCE (IDD_SELECTFILE_PREVIEW) ;
	m_ofn.lpstrTitle = _T("Select Image") ;

    // office style file-dialog, only for 2000/XP
    OSVERSIONINFO     osv = {sizeof(OSVERSIONINFO)} ;
    ::GetVersionEx (&osv) ;
    if ((osv.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osv.dwMajorVersion == 5))
        m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;
}


BEGIN_MESSAGE_MAP(DlgSelectFile, CDialog)
	//{{AFX_MSG_MAP(DlgSelectFile)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//=============================================================================
BOOL DlgSelectFile::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	OFNOTIFY	* notify = (OFNOTIFY *)lParam ;
	if (notify->hdr.code == CDN_SELCHANGE)
	{
        TCHAR         name[MAX_PATH] = {0} ;
		::SendMessage (notify->hdr.hwndFrom, CDM_GETFILEPATH, MAX_PATH, (LPARAM)name) ;
		if (m_imgPreview.Load (_bstr_t(name)))
		{
			// set width/height/bpp
			CString			strShow ;
			strShow.Format (_T("%d"), m_imgPreview.Width()) ;
			GetDlgItem(IDC_IMAGE_WIDTH)->SetWindowText(strShow) ;
			strShow.Format (_T("%d"), m_imgPreview.Height()) ;
			GetDlgItem(IDC_IMAGE_HEIGHT)->SetWindowText(strShow) ;
			strShow.Format (_T("%d Bits"), m_imgPreview.ColorBits()) ;
			GetDlgItem(IDC_IMAGE_COLORBIT)->SetWindowText(strShow) ;

            // set file's size
			CFile		filesize(name, CFile::modeRead) ;
			if (filesize.GetLength() < 1024)
			{
				strShow.Format (_T("%d Byte"), filesize.GetLength()) ;
			}
			else
			{
                strShow = OXO_FormatCommaNumber((long)(filesize.GetLength() / 1024)) + _T(" Kb") ;
			}
            GetDlgItem(IDC_IMAGE_FILESIZE)->SetWindowText(strShow) ;
            Invalidate(FALSE) ;
			return TRUE ;
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}
//=============================================================================
void DlgSelectFile::OnPaint() 
{
	CPaintDC   dc(this) ;

    CRect         rcPre ;
    GetDlgItem(IDC_PICPREVIEW)->GetClientRect(rcPre) ;

    CClientDC     preDC (GetDlgItem(IDC_PICPREVIEW)) ;
    FCMemDC       memDC (&preDC, rcPre) ;
    memDC.FillSolidRect (rcPre, RGB(255,255,255)) ;
    FCWin32::DrawImageAspect (m_imgPreview, memDC.m_hDC, rcPre) ;
    memDC.FinalBlt() ;
}
//=============================================================================
