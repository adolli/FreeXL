// 010Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "010.h"
#include "010Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy010Dlg dialog

CMy010Dlg::CMy010Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy010Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy010Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy010Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy010Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy010Dlg, CDialog)
	//{{AFX_MSG_MAP(CMy010Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy010Dlg message handlers

BOOL CMy010Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy010Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy010Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy010Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy010Dlg::OnOK() 
{
    CFileDialog   dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("JPEG Files (*.jpg)|*.jpg||"), this) ;
    if (dlg.DoModal() == IDOK)
    {
        FCObjImage   img ;
        FCImageProperty   prop ;
        img.Load (bstr_t((LPCTSTR)dlg.GetPathName()), &prop) ;

        bstr_t   s ;

        s += "EquipMake : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EquipMake).c_str() ;
        s += "\n" ;

        s += "EquipModel : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EquipModel).c_str() ;
        s += "\n" ;

        s += "Date : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_DTOrig).c_str() ;
        s += "\n" ;

        s += "ExposureTime : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_ExposureTime).c_str() ;
        s += " seconds\n" ;

        s += "FNumber : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_FNumber).c_str() ;
        s += "\n" ;

        s += "FocalLength : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_FocalLength).c_str() ;
        s += " millimeters\n" ;

        s += "ISOSpeed : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_ISOSpeed).c_str() ;
        s += "\n" ;

        s += "ExposureBias : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_ExposureBias).c_str() ;
        s += "\n" ;

        s += "MaxAperture : " ;
        s += prop.QueryPropertyValue(IMAGE_TAG_EXIF_MaxAperture).c_str() ;
        s += "\n" ;

        s += "Flash : " ;
        std::string   v = prop.QueryPropertyValue(IMAGE_TAG_EXIF_Flash) ;
        int   n ;
        FCOXOHelper::A2X(v, n) ;
        s += ((n & 1) ? "On" : "Off") ;
        s += "\n" ;

        s += "MeteringMode : " ;
        v = prop.QueryPropertyValue(IMAGE_TAG_EXIF_MeteringMode) ;
        FCOXOHelper::A2X(v, n) ;
        switch (n)
        {
            case 0 : s += "unknown" ; break;
            case 1 : s += "Average" ; break;
            case 2 : s += "CenterWeightedAverage" ; break;
            case 3 : s += "Spot" ; break;
            case 4 : s += "MultiSpot" ; break;
            case 5 : s += "Pattern" ; break;
            case 6 : s += "Partial" ; break;
            case 255 : s += "other" ; break;
        }
        s += "\n" ;
        
        s += "ExposureProg : " ;
        v = prop.QueryPropertyValue(IMAGE_TAG_EXIF_ExposureProg) ;
        FCOXOHelper::A2X(v, n) ;
        switch (n)
        {
            case 0 : s += "not defined" ; break;
            case 1 : s += "manual" ; break;
            case 2 : s += "normal program" ; break;
            case 3 : s += "aperture priority" ; break;
            case 4 : s += "shutter priority" ; break;
            case 5 : s += "creative program (biased toward depth of field)" ; break;
            case 6 : s += "action program (biased toward fast shutter speed)" ; break;
            case 7 : s += "portrait mode (for close-up photos with the background out of focus)" ; break;
            case 8 : s += "landscape mode (for landscape photos with the background in focus)" ; break;
        }
        s += "\n" ;

        AfxMessageBox(s) ;
    }
}
