// DlgAppSetting.cpp : implementation file
//

#include "stdafx.h"
#include "008.h"
#include "DlgAppSetting.h"
#include "008Doc.h"
#include "008View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgAppSetting dialog


DlgAppSetting::DlgAppSetting(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAppSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgAppSetting)
	m_nUndoLevel = 0;
	//}}AFX_DATA_INIT
    if (GetCurrentView())
    {
        m_nUndoLevel = GetCurrentView()->GetDocument()->GetUndoLevel() ;
    }
}


void DlgAppSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgAppSetting)
	DDX_Text(pDX, IDC_UNDOLEVEL, m_nUndoLevel);
	DDV_MinMaxInt(pDX, m_nUndoLevel, 0, 9999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgAppSetting, CDialog)
	//{{AFX_MSG_MAP(DlgAppSetting)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgAppSetting message handlers
