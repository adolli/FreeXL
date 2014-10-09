/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2005-3-11
 *   Home     :  www.crazy-bit.com
 *   Mail     :  crazybitwps@hotmail.com
 *   History  :  
 */
#pragma once

/**
 *  Memory DC class (to avoid screen's flash)
 */
class FCMemDC : public CDC  
{
public:
    /// Constructor
    FCMemDC (CDC* pDC, LPCRECT lpRect)
    {
        CreateCompatibleDC (pDC) ;
        m_rcDC = lpRect ; ASSERT(!m_rcDC.IsRectEmpty());
        m_memBmp.CreateCompatibleBitmap (pDC, m_rcDC.Width(), m_rcDC.Height()) ;
        m_pOldBitmap = SelectObject (&m_memBmp) ;
        m_pDC = pDC ; ASSERT(pDC);
    }
    virtual ~FCMemDC()
    {
        if (m_pOldBitmap)
        {
            ASSERT(FALSE) ;
            SelectObject (m_pOldBitmap) ;
			m_pOldBitmap = NULL;
        }
    }
    /// Draw content of memory DC to actual DC
    void FinalBlt()
    {
        m_pDC->BitBlt (m_rcDC.left, m_rcDC.top, m_rcDC.Width(), m_rcDC.Height(), this, 0, 0, SRCCOPY) ;
        m_pDC = NULL ;
        SelectObject (m_pOldBitmap) ;
        m_pOldBitmap = NULL ;
    }

private:
    CBitmap     m_memBmp ;
    CRect       m_rcDC ;
    CBitmap     * m_pOldBitmap ;
    CDC         * m_pDC ;
};


/**
 *  Auto restore DC's attribute (textcolor, bkmode, font, pen, brush)
 @verbatim
 example:
     FCAutoRestoreDCAttribute   _dc_attri (pDC) ;
     CFont   ft ;
     ft.CreatePointFont (90, _T("Arial")) ;
     ::SelectObject (pDC->GetSafeHdc(), ft.m_hObject) ;
     _dc_attri.Restore() ; // must restore before ft's deconstructor
 @endverbatim
 */
class FCAutoRestoreDCAttribute
{
public:
    /// Save pDC's attributes when constructor
    FCAutoRestoreDCAttribute (CDC* pDC) : m_pDC(pDC), m_bHasRestore(FALSE)
    {
        m_crTxt = pDC->GetTextColor() ;
        m_nBkMode = pDC->GetBkMode() ;
        m_hFont = ::GetCurrentObject (pDC->GetSafeHdc(), OBJ_FONT) ; ASSERT(m_hFont);
        m_hPen = ::GetCurrentObject (pDC->GetSafeHdc(), OBJ_PEN) ; ASSERT(m_hPen);
        m_hBrush = ::GetCurrentObject (pDC->GetSafeHdc(), OBJ_BRUSH) ; ASSERT(m_hBrush);
    }
    /// Restore pDC's attributes if you haven't call Restore() method
    ~FCAutoRestoreDCAttribute()
    {
        Restore() ;
    }

    /// Restore pDC's attributes
    void Restore()
    {
        if (m_bHasRestore)
            return ;

        if (::GetCurrentObject (m_pDC->GetSafeHdc(), OBJ_FONT) != m_hFont)
            ::SelectObject (m_pDC->GetSafeHdc(), m_hFont) ;
        if (::GetCurrentObject (m_pDC->GetSafeHdc(), OBJ_PEN) != m_hPen)
            ::SelectObject (m_pDC->GetSafeHdc(), m_hPen) ;
        if (::GetCurrentObject (m_pDC->GetSafeHdc(), OBJ_BRUSH) != m_hBrush)
            ::SelectObject (m_pDC->GetSafeHdc(), m_hBrush) ;

        m_pDC->SetBkMode(m_nBkMode) ;
        m_pDC->SetTextColor(m_crTxt) ;
        m_bHasRestore = TRUE ;
    }
    
private:
    CDC        * m_pDC ;
    COLORREF   m_crTxt ;
    int        m_nBkMode ;
    HGDIOBJ    m_hFont ;
    HGDIOBJ    m_hPen ;
    HGDIOBJ    m_hBrush ;
    BOOL       m_bHasRestore ;
};
