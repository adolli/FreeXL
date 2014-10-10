// 008Doc.cpp : implementation of the CMy008Doc class
//

#include "stdafx.h"
#include "008.h"

#include "008Doc.h"
#include "DlgHistogram.h"
#include "ProgressBar.h"
#include "DlgSaveFile.h"
#include "DlgAppSetting.h"
#include "DlgEffect.h"
#include "Dlg3DGrid.h"
#include "DlgFillGrid.h"
#include "DlgImgSize.h"
#include "DlgLayerSmoothEdge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy008Doc

IMPLEMENT_DYNCREATE(CMy008Doc, CDocument)

BEGIN_MESSAGE_MAP(CMy008Doc, CDocument)
    ON_COMMAND_RANGE(ID_FILTER_START, ID_FILTER_END, OnImageEffect)
	//{{AFX_MSG_MAP(CMy008Doc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_APP_SETTING, OnAppSetting)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_ACTUAL, OnZoomActual)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_FILTER_SAVE_ASCII, OnFilterSaveAscii)
	ON_COMMAND(ID_COUNT_COLOR, OnCountColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
/// command
void CUndoCommand::Execute (CMy008Doc* pDoc, FCInterface_PixelProcess* pProcessor, FCObjProgress* pProgress)
{
    if (pProcessor)
    {
        m_Undo = pDoc->m_img ;
        pDoc->m_img.SinglePixelProcessProc (*pProcessor, pProgress) ;
    }
}
void CUndoCommand::Undo (CMy008Doc* pDoc)
{
    m_Redo = pDoc->m_img ;
    pDoc->m_img = m_Undo ;
    pDoc->UpdateAllViews(NULL) ;
}
void CUndoCommand::Redo (CMy008Doc* pDoc)
{
    m_Undo = pDoc->m_img ;
    pDoc->m_img = m_Redo ;
    pDoc->UpdateAllViews(NULL) ;
}
void CUndoCommand_Stretch::Execute (CMy008Doc* pDoc, FCInterface_PixelProcess* pProcessor, FCObjProgress* pProgress)
{
    m_Undo = pDoc->m_img ;
    if (m_nType == INTERPOLATION_NONE)
        pDoc->m_img.Stretch (m_nWidth, m_nHeight) ;
    else if (m_nType == INTERPOLATION_BILINEAR)
        pDoc->m_img.Stretch_Smooth (m_nWidth, m_nHeight, pProgress) ;
}


/////////////////////////////////////////////////////////////////////////////
// CMy008Doc construction/destruction

CMy008Doc::CMy008Doc()
{
	// TODO: add one-time construction code here
}

CMy008Doc::~CMy008Doc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMy008Doc serialization

void CMy008Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMy008Doc diagnostics

#ifdef _DEBUG
void CMy008Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy008Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy008Doc commands

BOOL CMy008Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

    if (!m_img.Load (bstr_t(lpszPathName)))
    {
        ::AfxMessageBox (IDS_UNKNOW_IMAGE, MB_ICONEXCLAMATION) ;
        return FALSE ;
    }
    m_img.ConvertTo24Bit() ;
    UpdateAllViews(NULL) ;
    return TRUE;
}

void CMy008Doc::OnFileSaveAs() 
{
    // remove ext name
    TCHAR     szPath[_MAX_PATH] ;
    {
        TCHAR   szDrive[_MAX_DRIVE],
                szDir[_MAX_DIR],
                szFname[_MAX_FNAME] ;
        _tsplitpath (GetPathName(), szDrive, szDir, szFname, NULL) ;
        _tmakepath (szPath, szDrive, szDir, szFname, _T("")) ;
    }

    // set title to path
    if (CString(szPath).IsEmpty())
    {
        TCHAR     szCurrDir[MAX_PATH] ;
        ::GetCurrentDirectory (MAX_PATH, szCurrDir) ;

        CString   strTmp (szCurrDir) ;
        if (strTmp[strTmp.GetLength()-1] == _T('\\'))
            strTmp += GetTitle() ;
        else
            strTmp += _T("\\") + GetTitle() ;
        lstrcpy (szPath, strTmp) ;
    }

    CString       Filter ;
    Filter.LoadString (IDS_SAVEFILE_FORMAT) ; // file format
    DlgSaveFile   dlgFile (FALSE, NULL, szPath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, Filter) ;
    if (dlgFile.DoModal() == IDCANCEL)
        return ;

    bstr_t      strFile = (LPCTSTR)dlgFile.GetPathName() ;
    if (!m_img.Save (strFile))
    {
        ::AfxMessageBox (IDS_SAVEFILE_ERROR, MB_ICONEXCLAMATION) ;
    }
}

void CMy008Doc::OnHistogram() 
{
	DlgHistogram   dlg(m_img) ;
    dlg.DoModal() ;
}

void CMy008Doc::OnEditCopy() 
{
    FCWin32::CopyToClipboard(m_img) ;
    ::AfxMessageBox (IDS_COPYTO_CLIPBOARD) ;
}
void CMy008Doc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable (m_img.IsValidImage()) ;
}

void CMy008Doc::PCL_Implement_Undo (CUndoCommand* pCmd)
{
    pCmd->Undo(this) ;
}

void CMy008Doc::PCL_Implement_Redo (CUndoCommand* pCmd)
{
    pCmd->Redo(this) ;
}

// show progress
class CShowProgress : public FCObjProgress
{
public:
    CShowProgress ()
    {
        m_StatusProgress.Create (_T(""), 100, 100, TRUE) ;
    }
    virtual void ResetProgress ()
    {
        m_StatusProgress.SetPos (0) ;
        FCObjProgress::ResetProgress () ;
    }
    virtual void SetProgress (int nNew)
    {
        if (GetProgress() != nNew)
            m_StatusProgress.SetPos (nNew) ;
        FCObjProgress::SetProgress (nNew) ;
    }
private :
    CProgressBar	m_StatusProgress ;
};

void CMy008Doc::PerformImgEffect (CUndoCommand* pCmd)
{
    CWaitCursor     aWaitCursor ;

    pCmd->Execute (this, NULL, &CShowProgress()) ;
    AddCommand (pCmd) ;
    OXO_SetStatusBarText (0, _T(" ")) ;

    SetModifiedFlag (TRUE) ;
    UpdateAllViews (NULL) ;
}
void CMy008Doc::PerformImgEffect (FCInterface_PixelProcess* pCmd)
{
    if (pCmd == NULL)
        return ;

    CUndoCommand    * pUndo = new CUndoCommand() ;
    pUndo->Execute (this, pCmd, &CShowProgress()) ;
    AddCommand (pUndo) ;
    OXO_SetStatusBarText (0, _T(" ")) ;

    SetModifiedFlag (TRUE) ;
    UpdateAllViews (NULL) ;
}

void CMy008Doc::OnEditUndo() 
{
    Undo() ;
}

void CMy008Doc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable (IsUndoEnable()) ;
}

void CMy008Doc::OnAppSetting() 
{
    DlgAppSetting   dlg ;
    if (dlg.DoModal() == IDOK)
    {
        SetUndoLevel (dlg.m_nUndoLevel) ;
    }
}

void CMy008Doc::OnEditRedo() 
{
    Redo() ;
}

void CMy008Doc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable (IsRedoEnable()) ;
}

void CMy008Doc::OnZoomIn() 
{
    int   n = GetZoomScale() ;
    if (n < MAX_ZOOM)
    {
        if (n == -1)
            n = 1 ;
        SetZoomScale (++n) ;
    }
    UpdateAllViews(NULL) ;
}

void CMy008Doc::OnZoomActual() 
{
	SetZoomScale(1) ;
    UpdateAllViews(NULL) ;
}

void CMy008Doc::OnZoomOut() 
{
    int   n = GetZoomScale() ;
    if (n > MIN_ZOOM)
    {
        if (n == 1)
            n = -1 ;
        SetZoomScale (--n) ;
    }
    UpdateAllViews(NULL) ;
}

void CMy008Doc::OnImageEffect (UINT nID) 
{
    FCInterface_PixelProcess   * pCmd = NULL ;
    switch (nID)
    {
        case ID_FILTER_ADJUSTRGB :
            {
                DlgEffect   dlg(m_img, IDD_EFF_ADJUSTRGB) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelAdjustRGB (dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.m_nScroll_3) ;
            }
            break;
        case ID_FILTER_ADJUSTHS :
            {
                DlgEffect   dlg(m_img, IDD_EFF_ADJUSTHS) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelHueSaturation (dlg.m_nScroll_1, dlg.m_nScroll_2) ;
            }
            break;
        case ID_FILTER_GAMMA : // adjust gamma
            {
                DlgEffect   dlg (m_img, IDD_EFF_GAMMA) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelGamma(dlg.m_nScroll_1/100.0, dlg.GetColorChannel()) ;
            }
            break ;
        case ID_FILTER_COLORLEVEL :
            {
                DlgEffect   dlg (m_img, IDD_EFF_COLORLEVEL) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelColorLevel(FALSE, dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.GetColorChannel()) ;
            }
            break ;
        case ID_FILTER_SOFTGLOW :
            {
                DlgEffect   dlg (m_img, IDD_EFF_SOFTGLOW) ;
                if (dlg.DoModal() == IDOK)
                {
                    pCmd = new FCPixelSoftGlow(dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.m_nScroll_3) ;
                }
            }
            break;
        case ID_FILTER_TILEREFLECT :
            {
                DlgEffect   dlg (m_img, IDD_EFF_TILEREFLECT) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelTileReflection (45, dlg.m_nScroll_1, dlg.m_nScroll_2) ;
            }
			break ;
        case ID_FILTER_LENSFLARE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_LENSFLARE) ;
                if (dlg.DoModal() == IDOK)
                {
                    pCmd = new FCPixelLensFlare (dlg.m_pt) ;
                }
            }
            break;
        case ID_FILTER_AVERAGE_BLUR :
            {
                DlgEffect   dlg (m_img, IDD_EFF_AVERAGE_BLUR) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBlur_Box(dlg.m_nScroll_1, true) ;
            }
            break ;
        case ID_FILTER_GAUSS_BLUR :
            {
                DlgEffect   dlg (m_img, IDD_EFF_GAUSS_BLUR) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBlur_Gauss_IIR(dlg.m_nScroll_1, dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_COLORBALANCE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_COLORBALANCE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelColorBalance(dlg.m_bCheck_1 ? true : false, (TONE_REGION)dlg.m_nRadio_1, dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.m_nScroll_3) ;
            }
            break ;
        case ID_FILTER_FLIP :
            pCmd = new FCPixelFlip ; break ;
        case ID_FILTER_MIRROR :
            pCmd = new FCPixelMirror ; break ;
        case ID_FILTER_INVERT :
            pCmd = new FCPixelInvert ; break ;
        case ID_FILTER_GRAYSCALE :
            pCmd = new FCPixelGrayscale ; break ;
        case ID_FILTER_AUTO_CONTRAST :
            pCmd = new FCPixelAutoContrast ;
            break ;
        case ID_FILTER_AUTO_COLORENHANCE :
            pCmd = new FCPixelAutoColorEnhance ; break ;
        case ID_FILTER_AUTO_COLORLEVEL : // 自动色阶/auto adjust color level
            pCmd = new FCPixelColorLevel(TRUE, 0, 255) ; break ;
        case ID_FILTER_DESPECKLE :
            pCmd = new FCPixelMedianFilter(3) ; break ;
        case ID_FILTER_THRESHOLD :
            {
                DlgEffect   dlg (m_img, IDD_EFF_THRESHOLD) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelThreshold(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_SOLARIZE : // 曝光过度/Solarize
            {
                DlgEffect   dlg (m_img, IDD_EFF_SOLARIZE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelSolarize(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_COLORTONE : // 单色调/single color tone
            {
                DlgEffect   dlg (m_img, IDD_EFF_COLORTONE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelColorTone(dlg.m_crTone) ;
            }
            break ;
        case ID_FILTER_ADJUSTBRIGHTNESS :
            {
                DlgEffect   dlg(m_img, IDD_EFF_ADJUSTBRIGHTNESS) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBrightness (dlg.m_nScroll_1, dlg.GetColorChannel()) ;
            }
            break;
        case ID_FILTER_CLOTH_TEXTURE :
            {
                FCObjImage   * p = new FCObjImage ;
                FCWin32::LoadImageBitmapRes(*p, MAKEINTRESOURCE(IDB_CLOTH_TEXTURE)) ;
                pCmd = new FCPixelFillPattern(p, 255, true) ;
            }
            break;
        case ID_FILTER_ADJUSTCONTRAST :
            {
                DlgEffect   dlg(m_img, IDD_EFF_ADJUSTCONTRAST) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelContrast (dlg.m_nScroll_1, dlg.GetColorChannel()) ;
            }
            break;
        case ID_FILTER_ZOOM_BLUR :
            {
                DlgEffect   dlg(m_img, IDD_EFF_ZOOM_BLUR) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBlur_Zoom (dlg.m_nScroll_1) ;
            }
            break;
        case ID_FILTER_RADIAL_BLUR :
            {
                DlgEffect   dlg(m_img, IDD_EFF_RADIAL_BLUR) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBlur_Radial (dlg.m_nScroll_1) ;
            }
            break;
        case ID_FILTER_SHARP_LOW :
            pCmd = new FCPixelSharp(14) ; break ;
        case ID_FILTER_SHARP :
            pCmd = new FCPixelSharp(8) ; break ;
        case ID_FILTER_SHARP_HIGH :
            pCmd = new FCPixelSharp(4) ; break ;
        case ID_FILTER_NOISE :
            {
                DlgEffect   dlg(m_img, IDD_EFF_NOISE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelNoisify(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_BLOT :
            {
                DlgEffect   dlg(m_img, IDD_EFF_NOISIFY) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelNoisify(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_OILPAINT :
            {
                DlgEffect   dlg (m_img, IDD_EFF_OILPAINT) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelOilPaint(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_EMBOSS :
            {
                DlgEffect   dlg (m_img, IDD_EFF_EMBOSS) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelEmboss(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_SPLASH :
            {
                DlgEffect   dlg (m_img, IDD_EFF_SPLASH) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelSplash(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_POSTERIZE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_POSTERIZE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelPosterize(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_SHIFT :
            {
                DlgEffect   dlg (m_img, IDD_EFF_SHIFT) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelShift(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_CYLINDER :
            pCmd = new FCPixelCylinder ;
            break ;
        case ID_FILTER_MOSAIC :
            {
                DlgEffect   dlg (m_img, IDD_EFF_MOSAIC) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelMosaic(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_ADD_SHADOW:
            {
                SHADOWDATA   SD ;
                SD.nOffsetX = SD.nOffsetY = 5 ;
                SD.nSmooth = 10 ;
                SD.nAlpha = 75 ;
                SD.crShadow = PCL_RGBA(128,128,128) ;
                pCmd = new FCPixelAddShadow(SD) ;
            }
            break;
        case ID_FILTER_LENS :
            {
                DlgEffect   dlg (m_img, IDD_EFF_LENS) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelLens(dlg.m_nScroll_1/100.0, dlg.m_bCheck_1 ? true : false) ;
            }
            break ;
        case ID_FILTER_STRIPE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_STRIPE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelRibbon(dlg.m_nScroll_2, dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_WHIRLPINCH :
            {
                DlgEffect   dlg (m_img, IDD_EFF_WHIRLPINCH) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelWhirlPinch(AngleToRadian(dlg.m_nScroll_1), dlg.m_nScroll_2/100.0) ;
            }
            break ;
        case ID_FILTER_FRACTALTRACE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_FRACTALTRACE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelFractalTrace(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_ILLUSION :
            {
                DlgEffect   dlg (m_img, IDD_EFF_ILLUSION) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelIllusion(dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_RIPPLE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_RIPPLE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelRipple(dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.m_nRadio_1 ? true : false) ;
            }
            break ;
        case ID_FILTER_WAVE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_WAVE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelWave(dlg.m_nScroll_2, dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_HALFTONE :
            pCmd = new FCPixelHalftoneM3 ; break ;
        case ID_FILTER_DETECTEDGE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_DETECTEDGE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelDetectEdges (dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_3DGRID :
            {
                Dlg3DGrid   dlg ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixel3DGrid(dlg.m_nSize, dlg.m_nDepth) ;
            }
            break ;
        case ID_FILTER_FILLGRID :
            {
                DlgFillGrid   dlg ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelFillGrid(dlg.m_crFill1, dlg.m_crFill2, dlg.m_nGridWidth) ;
            }
            break ;
        case ID_FILTER_CUSTOMFILTER :
            {
                DlgEffect   dlg (m_img, IDD_EFF_CUSTOMFILTER) ;
                if (dlg.DoModal() == IDOK)
                {
                    pCmd = new FCPixelConvolute ;
                    ((FCPixelConvolute*)pCmd)->SetKernel (dlg.m_nElement, 5, dlg.m_nFilterScale, dlg.m_nFilterOffset) ;
                }
            }
            break ;
        case ID_FILTER_ROTATE90 :
            pCmd = new FCPixelRotate90; break;
        case ID_FILTER_ROTATE270 :
            pCmd = new FCPixelRotate270; break;
        case ID_FILTER_IMAGESIZE :
            {
                DlgImgSize   dlg (m_img.Width(), m_img.Height()) ;
                if (dlg.DoModal() == IDOK)
                {
                    PerformImgEffect (new CUndoCommand_Stretch (dlg.m_nWidth, dlg.m_nHeight, (dlg.m_nStretchType == 0) ? INTERPOLATION_NONE : INTERPOLATION_BILINEAR)) ;
                    return ;
                }
            }
            break ;
        case ID_FILTER_SMOOTH_EDGE :
            {
                static int    s_SmoothEdgeInit = 5 ;
                DlgLayerSmoothEdge   dlg (IDD_LAYER_SMOOTHEDGE, 1, 32, s_SmoothEdgeInit) ;
                if (dlg.DoModal() == IDOK)
                {
                    pCmd = new FCPixelSmoothEdge(dlg.m_nScroll_1) ;
                    s_SmoothEdgeInit = dlg.m_nScroll_1 ;
                }
            }
            break ;
        case ID_FILTER_ROTATE :
            {
                DlgEffect   dlg (m_img, IDD_EFF_ROTATE) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelRotate (dlg.m_nScroll_1) ;
            }
            break ;
        case ID_FILTER_INNERBEVEL_FRAME :
            {
                DlgEffect   dlg (m_img, IDD_EFF_INNERBEVEL_FRAME) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelInnerBevel (dlg.m_nScroll_1, dlg.m_nScroll_2) ;
            }
            break ;
        case ID_FILTER_BLINDS :
            {
                DlgEffect   dlg (m_img, IDD_EFF_BLINDS) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelBlinds(dlg.m_nRadio_1 ? FCPixelBlinds::BLIND_Y : FCPixelBlinds::BLIND_X, dlg.m_nScroll_1, dlg.m_nScroll_2, dlg.m_crBlind) ;
            }
            break ;
        case ID_FILTER_VIDEO :
            {
                DlgEffect   dlg (m_img, IDD_EFF_VIDEO) ;
                if (dlg.DoModal() == IDOK)
                    pCmd = new FCPixelVideo((FCPixelVideo::VIDEO_TYPE)dlg.m_nCombo_1) ;
            }
            break ;
    }
    m_img.ConvertTo32Bit() ;
    PerformImgEffect (pCmd) ;
    if ((nID == ID_FILTER_SMOOTH_EDGE) || (nID == ID_FILTER_ADD_SHADOW))
    {
        FCObjImage   imgWhite (m_img.Width(), m_img.Height(), 24) ;
        imgWhite.SinglePixelProcessProc (FCPixelFillColor(FCColor::crWhite())) ;
        CRect        rc (0, 0, m_img.Width(), m_img.Height()) ;
        imgWhite.AlphaBlend (m_img, rc, rc, 100) ;
        m_img = imgWhite ;
    }
    m_img.ConvertTo24Bit() ;
    if (pCmd)
        delete pCmd ;
}

BOOL CMy008Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    ::AfxMessageBox (_T("Please use 'Save As' menu to save image")) ;
    return FALSE ;
}

void CMy008Doc::OnFilterSaveAscii() 
{
    CFileDialog   dlgFile(FALSE, _T("txt"), _T("c:\\PhoXo.txt"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("TEXT files (*.txt)|*.txt||")) ;
    if (dlgFile.DoModal() != IDOK)
        return ;

    // force to delete file
    CWaitCursor		aCursor ;
    std::string     strFile (_bstr_t(dlgFile.GetPathName())) ;
    m_img.SinglePixelProcessProc (FCPixelExportAscII (strFile.c_str())) ;
}

void CMy008Doc::OnCountColor() 
{
    FCPixelColorsCount   aCmd ;
    m_img.SinglePixelProcessProc (aCmd) ;
    CString   str ;
    str.Format (_T("image use %d color"), (int)aCmd.GetColorsNumber()) ;
    ::AfxMessageBox(str) ;
}
