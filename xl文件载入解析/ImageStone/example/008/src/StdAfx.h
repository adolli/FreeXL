// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4FBE3B8F_68EC_4EAD_8C1F_49E210266515__INCLUDED_)
#define AFX_STDAFX_H__4FBE3B8F_68EC_4EAD_8C1F_49E210266515__INCLUDED_
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_DEPRECATE

#ifndef WINVER
    #define WINVER  0x0400
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../../../ImageStone.h"
#include "FCMemDC.h"

// color convert ------------------------------------------------------------
inline RGBQUAD  RGBtoRGBQUAD (COLORREF color)
{
    RGBQUAD   rgb = {GetBValue(color), GetGValue(color), GetRValue(color), (BYTE)(color>>24)} ;
    return rgb ;
}
inline COLORREF  RGBQUADtoRGB (RGBQUAD rgb)
{
    return RGB (rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue) ;
}
// --------------------------------------------------------------------------
void AFXAPI DDX_SpinCtrl(CDataExchange* pDX, int nIDC, int& value) ;
// --------------------------------------------------------------------------
// 67831 --> 67,831
CString OXO_FormatCommaNumber (long nNum) ;
// --------------------------------------------------------------------------
CString OXO_GetAppPath() ;
// --------------------------------------------------------------------------
void OXO_SetStatusBarText (int nBlock, LPCTSTR szText) ;
// --------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4FBE3B8F_68EC_4EAD_8C1F_49E210266515__INCLUDED_)
