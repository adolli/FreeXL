#include "stdafx.h"
#include "resource.h"
#include "DlgEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RGBQUAD DlgEffect::m_crBlind = FCColor::crBlack() ;

//=============================================================================
DlgEffect::DlgEffect(const FCObjImage& img, UINT nIDTemplate, CWnd* pParent)
    : CDialog(nIDTemplate, pParent)
{
    m_nDlgID = nIDTemplate ;
    m_Thumb = img ;
    m_Thumb.ConvertTo24Bit() ;
}
//=============================================================================
DlgEffect::~DlgEffect ()
{
    while (!m_listTexture.empty ())
    {
        FCObjImage  * node = m_listTexture.front() ;
        m_listTexture.pop_front () ;
        delete node ;
    }
}
//=============================================================================
void DlgEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    // 2th, 3th slider
	switch (m_nDlgID)
	{
		case IDD_EFF_ADJUSTRGB :
        case IDD_EFF_COLORBALANCE :
        case IDD_EFF_SOFTGLOW :
            DDX_Control(pDX, IDC_SCROLL_PARAM3, m_Slide_3);
            DDX_Text(pDX, IDC_SHOWPARAM3, m_nScroll_3);
            DDX_Slider(pDX, IDC_SCROLL_PARAM3, m_nScroll_3);
        case IDD_EFF_ADJUSTHS :
        case IDD_EFF_COLORLEVEL :
        case IDD_EFF_STRIPE :
        case IDD_EFF_WHIRLPINCH :
        case IDD_EFF_TILEREFLECT :
        case IDD_EFF_RIPPLE :
        case IDD_EFF_WAVE :
        case IDD_EFF_BLINDS :
        case IDD_EFF_INNERBEVEL_FRAME :
            DDX_Control(pDX, IDC_SCROLL_PARAM2, m_Slide_2);
            DDX_Text(pDX, IDC_SHOWPARAM2, m_nScroll_2);
            DDX_Slider(pDX, IDC_SCROLL_PARAM2, m_nScroll_2);
			break ;
    }

    // at least
    DDX_Control(pDX, IDS_PICPREVIEW, m_Preview);
    DDX_Control(pDX, IDC_SCROLL_PARAM, m_Slide_1);
    DDX_Text(pDX, IDC_SHOWPARAM, m_nScroll_1);
    DDX_Slider(pDX, IDC_SCROLL_PARAM, m_nScroll_1);

    // has Radio
    switch(m_nDlgID)
    {
        case IDD_EFF_GAMMA :
        case IDD_EFF_COLORLEVEL :
        case IDD_EFF_COLORBALANCE :
        case IDD_EFF_RIPPLE :
        case IDD_EFF_BLINDS :
        case IDD_EFF_ADJUSTBRIGHTNESS :
        case IDD_EFF_ADJUSTCONTRAST :
            DDX_Radio(pDX, IDC_EFF_RADIO1, m_nRadio_1); break;
    }

    // has Check
    switch(m_nDlgID)
    {
        case IDD_EFF_COLORBALANCE :
        case IDD_EFF_LENS :
            DDX_Check(pDX, IDC_EFF_CHECKBOX1, m_bCheck_1); break;
    }

    // has Combobox
    switch(m_nDlgID)
    {
        case IDD_EFF_VIDEO :
            DDX_CBIndex(pDX, IDC_COMBO_PARAM, m_nCombo_1); break;
    }

    // custom filter data
    if (m_nDlgID == IDD_EFF_CUSTOMFILTER)
    {
        for (int i=0 ; i < 25 ; i++)
            {DDX_Text(pDX, IDC_CUSTOM_FILTER0+i, m_nElement[i]) ;}
        DDX_Text(pDX, IDC_CUSTOM_FILTER_SCALE, m_nFilterScale) ;
        DDX_Text(pDX, IDC_CUSTOM_FILTER_OFFSET, m_nFilterOffset) ;
    }
}
//=============================================================================
BEGIN_MESSAGE_MAP(DlgEffect, CDialog)
	//{{AFX_MSG_MAP(DlgEffect)
	ON_WM_HSCROLL()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_EFF_RADIO1, OnDataChanged)
    ON_BN_CLICKED(IDC_EFF_RADIO2, OnDataChanged)
    ON_BN_CLICKED(IDC_EFF_RADIO3, OnDataChanged)
    ON_BN_CLICKED(IDC_EFF_RADIO4, OnDataChanged)
    ON_BN_CLICKED(IDC_EFF_CHECKBOX1, OnDataChanged)
    ON_CBN_SELCHANGE(IDC_COMBO_PARAM, OnDataChanged)
    ON_BN_CLICKED(IDC_EFF_BUTTON1, OnClickButton1)
    ON_BN_CLICKED(IDC_EFF_BUTTON2, OnClickButton2)
    ON_BN_CLICKED(IDC_EFF_BUTTON3, OnClickButton3)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()

    // custom filter
    ON_EN_CHANGE(IDC_CUSTOM_FILTER0, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER1, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER2, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER3, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER4, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER5, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER6, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER7, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER8, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER9, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER10, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER11, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER12, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER13, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER14, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER15, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER16, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER17, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER18, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER19, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER20, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER21, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER22, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER23, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER24, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER_SCALE, OnDataChanged)
    ON_EN_CHANGE(IDC_CUSTOM_FILTER_OFFSET, OnDataChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//=============================================================================
BOOL DlgEffect::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// preview size
    CRect     rcPre ;
    m_Preview.GetClientRect(rcPre) ;

	// aspect thumbnail
	CRect     rcOrigin (0,0,m_Thumb.Width(),m_Thumb.Height()) ;
	if ((rcOrigin.Width() <= rcPre.Width()) && (rcOrigin.Height() <= rcPre.Height()))
	{
		m_fScale = 1.0 ; // no scale
	}
	else
	{
		double		duW = rcPre.Width() / (double)rcOrigin.Width(),
					duH = rcPre.Height() / (double)rcOrigin.Height() ;
		m_fScale = min (duW, duH) ; // scale
		int			nThumbW = (int)(rcOrigin.Width() * m_fScale),
					nThumbH = (int)(rcOrigin.Height() * m_fScale) ;
		m_Thumb.Stretch_Smooth (nThumbW, nThumbH) ; // stretch
	}
	ASSERT(m_fScale <= 1.0) ;
    m_Curr = m_Thumb ;

#define SET_SLIDE_1(x,y,z)  {m_Slide_1.SetRange (x,y,TRUE); m_nScroll_1=z;}
#define SET_SLIDE_2(x,y,z)  {m_Slide_2.SetRange (x,y,TRUE); m_nScroll_2=z;}
#define SET_SLIDE_3(x,y,z)  {m_Slide_3.SetRange (x,y,TRUE); m_nScroll_3=z;}

	// init
	switch (m_nDlgID)
	{
    case IDD_EFF_EMBOSS : SET_SLIDE_1(1,10,2); break;
    case IDD_EFF_MOSAIC : SET_SLIDE_1(2,200,10); break;
    case IDD_EFF_ADJUSTRGB : SET_SLIDE_1(-128,128,0); SET_SLIDE_2(-128,128,0); SET_SLIDE_3(-128,128,0); break;
    case IDD_EFF_ADJUSTHS : SET_SLIDE_1(0,200,100); SET_SLIDE_2(0,200,100); break;
    case IDD_EFF_GAMMA : SET_SLIDE_1(1,200,100); m_nRadio_1=0; break; // RGB channel
    case IDD_EFF_COLORLEVEL : SET_SLIDE_1(0,255,0); SET_SLIDE_2(0,255,255); m_nRadio_1=0; break;
    case IDD_EFF_COLORBALANCE : SET_SLIDE_1(-100,100,0); SET_SLIDE_2(-100,100,0); SET_SLIDE_3(-100,100,0);
        m_bCheck_1=FALSE; m_nRadio_1=0; break; // not preserve lum/shadows
    case IDD_EFF_THRESHOLD : SET_SLIDE_1(0,255,128); break;
    case IDD_EFF_SOLARIZE : SET_SLIDE_1(0,255,128); break;
    case IDD_EFF_COLORTONE : m_crTone=PCL_RGBA(254,168,33); break;
    case IDD_EFF_ADJUSTBRIGHTNESS : SET_SLIDE_1(0,200,100); m_nRadio_1=0; break; // RGB channel
    case IDD_EFF_ADJUSTCONTRAST : SET_SLIDE_1(0,200,100); m_nRadio_1=0; break; // RGB channel
    case IDD_EFF_NOISE : SET_SLIDE_1(1,200,100); break;
    case IDD_EFF_NOISIFY : SET_SLIDE_1(1,100,50); break;
    case IDD_EFF_OILPAINT : SET_SLIDE_1(1,15,2); break;
    case IDD_EFF_SPLASH : SET_SLIDE_1(3,64,5); break;
    case IDD_EFF_POSTERIZE : SET_SLIDE_1(2,20,3); break;
    case IDD_EFF_SHIFT : SET_SLIDE_1(1,200,20); break;
    case IDD_EFF_LENS : SET_SLIDE_1(100,300,150); m_bCheck_1=FALSE; break;
    case IDD_EFF_STRIPE : SET_SLIDE_1(0,100,35); SET_SLIDE_2(0,100,25); break;
    case IDD_EFF_WHIRLPINCH : SET_SLIDE_1(-360,360,45); SET_SLIDE_2(-100,100,45); break;
    case IDD_EFF_TILEREFLECT : SET_SLIDE_1(2,200,40); SET_SLIDE_2(-20,20,8); break;
    case IDD_EFF_FRACTALTRACE : SET_SLIDE_1(1,4,3); break;
    case IDD_EFF_ILLUSION : SET_SLIDE_1(0,10,3); break;
    case IDD_EFF_RIPPLE : SET_SLIDE_1(1,200,30); SET_SLIDE_2(1,200,10); m_nRadio_1=1; break;
    case IDD_EFF_WAVE : SET_SLIDE_1(1,50,10); SET_SLIDE_2(1,50,30); break;
    case IDD_EFF_AVERAGE_BLUR : SET_SLIDE_1(2,32,5); break;
    case IDD_EFF_GAUSS_BLUR : SET_SLIDE_1(2,32,5); break;
    case IDD_EFF_DETECTEDGE : SET_SLIDE_1(1,10,2); break;
    case IDD_EFF_ZOOM_BLUR : SET_SLIDE_1(1,30,2); break;
    case IDD_EFF_LENSFLARE : m_pt=CPoint(50,50); break;
    case IDD_EFF_RADIAL_BLUR : SET_SLIDE_1(1,90,15); break;
    case IDD_EFF_INNERBEVEL_FRAME : SET_SLIDE_1(1,100,20); SET_SLIDE_2(2,64,10); break;
    case IDD_EFF_SOFTGLOW : SET_SLIDE_1(2,64,10); SET_SLIDE_2(0,200,100); SET_SLIDE_3(0,200,100); break;
    case IDD_EFF_CUSTOMFILTER :
        ZeroMemory (m_nElement, 25*sizeof(int)); m_nElement[12]=1;
        m_nFilterScale = 1; m_nFilterOffset = 0;
        break;
    case IDD_EFF_ROTATE : SET_SLIDE_1(1,359,45); break;
    case IDD_EFF_BLINDS :
        {
            const int     nWidth = FMax (rcOrigin.Width(), rcOrigin.Height()) ;
            SET_SLIDE_1(2,nWidth,FClamp(100,2,nWidth)) ;
            SET_SLIDE_2(1,100,100) ;
            m_nRadio_1=0 ; // ˮƽ
        }
        break;
    case IDD_EFF_VIDEO : m_nCombo_1=0; break;
	}

	// show current value
	UpdateData (FALSE) ;

    m_Preview.ShowWindow(SW_HIDE) ; m_Preview.EnableWindow(FALSE) ;
	// set preview control property
	OnDataChanged() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//=============================================================================
void DlgEffect::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    UpdateData(TRUE) ;

    m_Curr = m_Thumb ;
    m_Curr.ConvertTo32Bit() ;
    FCSinglePixelProcessBase   * pCmd = NULL ;
	switch (m_nDlgID)
	{
    case IDD_EFF_EMBOSS : pCmd=new FCPixelEmboss(m_nScroll_1); break;
    case IDD_EFF_MOSAIC : pCmd=new FCPixelMosaic(int(m_nScroll_1*m_fScale)); break;
    case IDD_EFF_ADJUSTRGB : pCmd=new FCPixelAdjustRGB(m_nScroll_1, m_nScroll_2, m_nScroll_3); break;
    case IDD_EFF_ADJUSTHS : pCmd=new FCPixelHueSaturation(m_nScroll_1, m_nScroll_2); break;
    case IDD_EFF_GAMMA : pCmd=new FCPixelGamma(m_nScroll_1/100.0, GetColorChannel()); break;
    case IDD_EFF_COLORLEVEL : pCmd=new FCPixelColorLevel(FALSE, m_nScroll_1, m_nScroll_2, GetColorChannel()); break;
    case IDD_EFF_COLORBALANCE : pCmd=new FCPixelColorBalance(m_bCheck_1 ? true : false, (TONE_REGION)m_nRadio_1, m_nScroll_1, m_nScroll_2, m_nScroll_3); break;
    case IDD_EFF_THRESHOLD : pCmd=new FCPixelThreshold(m_nScroll_1); break;
    case IDD_EFF_SOLARIZE : pCmd=new FCPixelSolarize(m_nScroll_1); break;
    case IDD_EFF_COLORTONE : pCmd=new FCPixelColorTone(m_crTone); break;
    case IDD_EFF_ADJUSTBRIGHTNESS : pCmd=new FCPixelBrightness (m_nScroll_1, GetColorChannel()); break;
    case IDD_EFF_ADJUSTCONTRAST : pCmd=new FCPixelContrast  (m_nScroll_1, GetColorChannel()); break;
    case IDD_EFF_NOISE : pCmd=new FCPixelNoisify(m_nScroll_1); break;
    case IDD_EFF_NOISIFY : pCmd=new FCPixelNoisify(m_nScroll_1); break;
    case IDD_EFF_OILPAINT : pCmd=new FCPixelOilPaint(int(m_nScroll_1*m_fScale)); break;
    case IDD_EFF_SPLASH : pCmd=new FCPixelSplash(int(m_nScroll_1*m_fScale)); break;
    case IDD_EFF_POSTERIZE : pCmd=new FCPixelPosterize(m_nScroll_1); break;
    case IDD_EFF_SHIFT : pCmd=new FCPixelShift(int(m_nScroll_1*m_fScale)); break;
    case IDD_EFF_LENS : pCmd=new FCPixelLens(m_nScroll_1/100.0, m_bCheck_1 ? true : false); break;
    case IDD_EFF_STRIPE : pCmd=new FCPixelRibbon(m_nScroll_2, m_nScroll_1); break;
    case IDD_EFF_WHIRLPINCH : pCmd=new FCPixelWhirlPinch(AngleToRadian(m_nScroll_1), m_nScroll_2/100.0); break;
    case IDD_EFF_FRACTALTRACE : pCmd=new FCPixelFractalTrace(m_nScroll_1); break;
    case IDD_EFF_ILLUSION : pCmd=new FCPixelIllusion(m_nScroll_1); break;
    case IDD_EFF_TILEREFLECT : pCmd=new FCPixelTileReflection (45, m_nScroll_1*m_fScale, m_nScroll_2); break;
    case IDD_EFF_RIPPLE : pCmd=new FCPixelRipple(int(m_nScroll_1*m_fScale), int(m_nScroll_2*m_fScale), m_nRadio_1 ? true : false); break;
    case IDD_EFF_WAVE : pCmd=new FCPixelWave(int(m_nScroll_2*m_fScale), m_nScroll_1); break;
    case IDD_EFF_DETECTEDGE : pCmd=new FCPixelDetectEdges(m_nScroll_1); break;
    case IDD_EFF_LENSFLARE : pCmd=new FCPixelLensFlare(m_pt); break;
    case IDD_EFF_INNERBEVEL_FRAME : pCmd=new FCPixelInnerBevel(m_nScroll_1*m_fScale, m_nScroll_2*m_fScale); break;
    case IDD_EFF_SOFTGLOW : pCmd=new FCPixelSoftGlow(m_nScroll_1*m_fScale,m_nScroll_2,m_nScroll_3); break;
    case IDD_EFF_CUSTOMFILTER :
        {
            pCmd = new FCPixelConvolute ;
            ((FCPixelConvolute*)pCmd)->SetKernel (m_nElement, 5, m_nFilterScale, m_nFilterOffset) ;
        }
        break;
    case IDD_EFF_ROTATE :
        pCmd = new FCPixelRotate(m_nScroll_1) ;
        break ;
    case IDD_EFF_AVERAGE_BLUR :
        pCmd = new FCPixelBlur_Box(m_nScroll_1*m_fScale, true) ;
        break;
    case IDD_EFF_GAUSS_BLUR :
        pCmd = new FCPixelBlur_Gauss_IIR(m_nScroll_1*m_fScale, m_nScroll_1*m_fScale) ;
        break;
    case IDD_EFF_ZOOM_BLUR :
        pCmd = new FCPixelBlur_Zoom(m_nScroll_1) ;
        break;
    case IDD_EFF_RADIAL_BLUR :
        pCmd = new FCPixelBlur_Radial(m_nScroll_1) ;
        break;
    case IDD_EFF_BLINDS : pCmd=new FCPixelBlinds(m_nRadio_1 ? FCPixelBlinds::BLIND_Y : FCPixelBlinds::BLIND_X, int(m_nScroll_1*m_fScale), m_nScroll_2, m_crBlind); break;
    case IDD_EFF_VIDEO : pCmd=new FCPixelVideo((FCPixelVideo::VIDEO_TYPE)m_nCombo_1); break;
	}

    if (pCmd == NULL)
        {ASSERT(FALSE); return;}

    m_Curr.SinglePixelProcessProc (*pCmd) ;
    delete pCmd ;
    m_Curr.ConvertTo24Bit() ;

	// show current value
	this->UpdateData (FALSE) ;

    Invalidate(FALSE) ;
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//=============================================================================
IMAGE_CHANNEL DlgEffect::GetColorChannel()
{
	IMAGE_CHANNEL    nChannel = CHANNEL_RGB ;
	switch (m_nRadio_1)
	{
		case 0 : nChannel = CHANNEL_RGB ; break ;
		case 1 : nChannel = CHANNEL_RED ; break ;
		case 2 : nChannel = CHANNEL_GREEN ; break ;
		case 3 : nChannel = CHANNEL_BLUE ; break ;
	}
	return nChannel ;
}
//=============================================================================
void DlgEffect::OnClickButton1()
{
    if (m_nDlgID == IDD_EFF_COLORBALANCE)
    {
        m_nScroll_1 = m_nScroll_2 = m_nScroll_3 = 0 ;
        m_bCheck_1=FALSE; m_nRadio_1=0; // not preserve lum/shadows
        UpdateData(FALSE) ;
        OnDataChanged() ;
        return ;
    }
}
void DlgEffect::OnClickButton2()
{
    if (m_nDlgID == IDD_EFF_CUSTOMFILTER)
    {
        // Load ACF file
        CFileDialog  dlg(TRUE, _T("acf"), _T(""), OFN_HIDEREADONLY, _T("Custom Filters (*.ACF)|*.acf||")) ;
        if (dlg.DoModal() == IDOK)
        {
            // load filter from .acf
            std::deque<int>     listEle ;
            if (LoadPhotoshopACF (dlg.GetPathName(), listEle))
            {
                for (int i=0 ; i < 25 ; i++)
                    m_nElement[i] = listEle[i] ;
                m_nFilterScale = listEle[25] ;
                m_nFilterOffset = listEle[26] ;
                UpdateData(FALSE) ;
                OnDataChanged() ;
            }
        }
        return ;
    }
}
void DlgEffect::OnClickButton3()
{
    if (m_nDlgID == IDD_EFF_CUSTOMFILTER)
    {
        // save ACF file
        CFileDialog  dlg(FALSE, _T("acf"), _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Custom Filters (*.ACF)|*.acf||")) ;
        if (dlg.DoModal() == IDOK)
        {
            std::deque<int>     listEle ;
            for (int i=0 ; i < 25 ; i++)
                listEle.push_back (m_nElement[i]) ;
            listEle.push_back (m_nFilterScale) ;
            listEle.push_back (m_nFilterOffset) ;

            ::DeleteFile (dlg.GetPathName()) ;
            SavePhotoshopACF (dlg.GetPathName(), listEle) ;
        }
        return ;
    }
}
//=============================================================================
void DlgEffect::OnDataChanged()
{
    SendMessage (WM_HSCROLL, MAKEWPARAM (SB_THUMBPOSITION,m_nScroll_1), (LPARAM)m_Slide_1.m_hWnd) ;
}
//=============================================================================
void DlgEffect::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if (m_nDlgID == IDD_EFF_COLORTONE)
    {
        OnMouseMove (MK_LBUTTON, point) ;
        return ;
    }
    else if (m_nDlgID == IDD_EFF_BLINDS)
    {
        CWnd		* pWnd = this->GetDlgItem (IDC_BK_COLOR) ;
        CRect		rcWnd ;
        pWnd->GetClientRect (rcWnd) ;
        pWnd->ClientToScreen (rcWnd) ;
        ScreenToClient (rcWnd) ;

        if (rcWnd.PtInRect(point))
        {
            CColorDialog   dlg (RGBQUADtoRGB(m_crBlind), CC_FULLOPEN) ;
            if (dlg.DoModal() == IDOK)
            {
                m_crBlind = RGBtoRGBQUAD (dlg.GetColor()) ;
                OnDataChanged() ;
            }
        }
    }
    else if (m_nDlgID == IDD_EFF_LENSFLARE)
    {
        CRect   rc ;
        m_Preview.GetClientRect (rc) ;
        m_Preview.ClientToScreen (rc) ;
        this->ScreenToClient (rc) ;

        // position on preview control
        int   x = (rc.Width() - m_Curr.Width()) / 2,
              y = (rc.Height() - m_Curr.Height()) / 2 ;

        m_pt.x = point.x - (rc.left + x) ;
        m_pt.y = point.y - (rc.top + y) ;

        OnDataChanged() ;
        return ;
    }
    CDialog::OnLButtonDown(nFlags, point) ;
}
//=============================================================================
void DlgEffect::OnMouseMove(UINT nFlags, CPoint point) 
{
    if ((m_nDlgID == IDD_EFF_COLORTONE) && (nFlags & MK_LBUTTON))
    {
        CStatic		* staTone = (CStatic*)GetDlgItem(IDC_COLOR_SELECT) ;
        CRect		rcClient ;
        staTone->GetClientRect (&rcClient) ;
        this->ClientToScreen (&point) ;
        staTone->ScreenToClient (&point) ;
        if (rcClient.PtInRect (point))
        {
            CDC		* pDC = staTone->GetDC() ;
            m_crTone = RGBtoRGBQUAD(pDC->GetPixel(point)) ;
            staTone->ReleaseDC (pDC) ;
            OnDataChanged() ;
        }
        return ;
    }
    CDialog::OnMouseMove(nFlags, point);
}
//=============================================================================
void DlgEffect::OnPaint() 
{
    CPaintDC   dc(this); // device context for painting

    // draw thumbnail image
    CRect   rcThumb ;
    m_Preview.GetClientRect (&rcThumb) ;
    m_Preview.ClientToScreen (&rcThumb) ;
    this->ScreenToClient (&rcThumb) ;

    dc.FillSolidRect (rcThumb, RGB(191,203,251)) ;
    FCWin32::DrawImageAspect (m_Curr, dc.GetSafeHdc(), rcThumb) ;
    rcThumb.InflateRect(1,1,1,1) ;
    dc.FrameRect (&rcThumb, &CBrush(RGB(192,192,192))) ;

    if (m_nDlgID == IDD_EFF_BLINDS)
    {
        CWnd     * pWnd = this->GetDlgItem (IDC_BK_COLOR) ;
        CRect    rcWnd ;
        pWnd->GetClientRect (rcWnd) ;
        pWnd->ClientToScreen (rcWnd) ;  
        ScreenToClient (rcWnd) ;
        dc.FillRect (rcWnd, &CBrush(RGBQUADtoRGB(m_crBlind))) ;
        dc.FrameRect (rcWnd, &CBrush(~RGBQUADtoRGB(m_crBlind))) ;
    }
    // Do not call DlgPhoXoBase::OnPaint() for painting messages
}
//=============================================================================
static int __PS_WORD2INT (const BYTE* pWord)
{
    BYTE   bySwap[2] = {pWord[1], pWord[0]} ;
    return *(short*)bySwap ;
}
//=============================================================================
bool DlgEffect::LoadPhotoshopACF (LPCTSTR szFileName, std::deque<int>& listElem)
{
    listElem.clear() ;
    CFile   inFile ;
    if (!inFile.Open (bstr_t(szFileName), CFile::modeRead))
        return false ;

    // get file's size
    int   nFileSize = inFile.GetLength() ;
    if (nFileSize != 54) // 27.elem * 2.bytes
        return false ;

    // read file into memory.
    PCL_array<short>   pStart (new BYTE[nFileSize]) ;
    inFile.Read (pStart.get(), nFileSize) ;

    // 5 x 5 filter & scale & offset
    for (int i=0 ; i < 25 ; i++)
    {
        listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[i])) ;
    }
    listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[25])) ;
    listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[26])) ;
    return true ;
}
//=============================================================================
bool DlgEffect::SavePhotoshopACF (LPCTSTR szFileName, const std::deque<int>& listElem)
{
    if (listElem.size() != 27)  {assert(false); return false;}

    // create file
    CFile     outFile ;
    if (!outFile.Open (szFileName, CFile::modeWrite))
        return false ;

    BYTE    pElem[2] ;
    for (size_t i=0 ; i < listElem.size() ; i++)
    {
        *(short*)pElem = (short)listElem[i] ;
        FSwap (pElem[0], pElem[1]) ;
        outFile.Write (pElem, 2) ;
    }
    return true ;
}
//=============================================================================
void DlgEffect::OnOK()
{
    if (m_nDlgID == IDD_EFF_LENSFLARE)
    {
        m_pt.x = (int)(m_pt.x / m_fScale) ;
        m_pt.y = (int)(m_pt.y / m_fScale) ;
    }
    CDialog::OnOK() ;
}

