// stdafx.cpp : source file that includes just the standard includes
//	008.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#pragma warning (disable : 4786)
#include "stdafx.h"

/*************************************************************************
功能：	判断指定的键是否按下
入口：	vKey: 虚拟键码
出口：	TRUE：按下；FALSE：未按下
*************************************************************************/
BOOL FIsKeyDown(int vKey)
{
    ASSERT(vKey!=VK_CAPITAL && vKey!=VK_NUMLOCK && vKey!=VK_SCROLL);
    SHORT nKeyState = GetKeyState(vKey);
    int nMask = 1 << (sizeof(SHORT)*8 - 1);
    return ((nKeyState & nMask)!=0);
}
//=============================================================================
void AFXAPI DDX_SpinCtrl(CDataExchange* pDX, int nIDC, int& value)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    if (pDX->m_bSaveAndValidate)
        value = (int)(__int16)::SendMessage(hWndCtrl, UDM_GETPOS, 0, 0);
    else
        ::SendMessage(hWndCtrl, UDM_SETPOS, 0, MAKELPARAM((__int16)value, 0));
}
//=============================================================================
// 67831 --> 67,831
CString OXO_FormatCommaNumber (long nNum)
{
    NUMBERFMT	nf ;
    nf.NumDigits = 0 ;
    nf.LeadingZero = FALSE ;
    nf.Grouping = 3 ;
    nf.lpDecimalSep = _T(".") ;
    nf.lpThousandSep = _T(",") ;
    nf.NegativeOrder = 0 ;
    TCHAR     szComma[64] = {0} ;
    
    ::GetNumberFormat (LOCALE_USER_DEFAULT, 0, _bstr_t(FCOXOHelper::X2A(nNum).c_str()), &nf, szComma, 64) ;
    return CString(szComma) ;
}
//=============================================================================
void OXO_SetStatusBarText (int nBlock, LPCTSTR szText)
{
    CStatusBar   * pStatus = DYNAMIC_DOWNCAST(CStatusBar, ((CMDIFrameWnd*)::AfxGetMainWnd())->GetMessageBar());
    if (pStatus)
        pStatus->SetPaneText (nBlock, szText) ;
}
//=============================================================================
CString OXO_GetAppPath()
{
    TCHAR     szExeName[MAX_PATH],
              szDriver[_MAX_DRIVE],
              szPath[MAX_PATH] ;
    ::GetModuleFileName (NULL, szExeName, MAX_PATH) ;
    ::_tsplitpath (szExeName, szDriver, szPath, NULL, NULL) ;
    ::_tmakepath (szExeName, szDriver, szPath, NULL, NULL) ;
    return CString(szExeName) ;
}
//=============================================================================
