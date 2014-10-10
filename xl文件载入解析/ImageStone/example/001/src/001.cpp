
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "../../../ImageStone.h"

// operation menu
const char * szOpMenu = "please enter a number(1 to 25) to choice a effect.\n\n\
    1) invert image color\n\
    2) gray scale\n\
    3) threshold\n\
    4) flip\n\
    5) emboss\n\
    6) splash\n\
    7) mosaic\n\
    8) oil paint\n\
    9) add 3D grid\n\
    10) whirl & pinch\n\
    11) gradient fill radial\n\
    12) adjust gamma\n\
    13) rotate 90'\n\
    14) ribbon\n\
    15) halftone\n\
    16) lens flare\n\
    17) adjust saturation\n\
    18) box blur\n\
    19) zoom blur\n\
    20) radial blur\n\
    21) motion blur\n\
    22) add shadow\n\
    23) color tone\n\
    24) soft glow\n\
    25) tile reflection\n\
" ;

//================================================================================
class FCShowProgress : public FCObjProgress
{
public :
    virtual void ResetProgress()
    {
        m_nLast = 0 ;
    }
    virtual void SetProgress (int nNew)
    {
        if (nNew - m_nLast >= 2)
        {
            printf ("#") ;
            m_nLast = nNew ;
        }
    }
protected :
    int     m_nLast ;
} ;
//================================================================================
int main (int argc, char* argv[])
{
	const char* szTestFile = "test.bmp" ; // image to load
    const char* szSaveFile = "save.bmp" ; // image to save

    // not found
    FILE   * pf = fopen (szTestFile, "rb") ;
    if (pf)
        fclose(pf) ;
    if (!pf)
    {
        printf ("can't load %s, please put a bmp file named %s in same folder to binary file.\n", szTestFile, szTestFile) ;
        return 0 ;
    }

    // load test image
    FCObjImage   img ;
    if (!img.Load (szTestFile))
    {
        printf ("load %s failed, the image must BMP format.\n", szTestFile) ;
        return 0 ;
    }

    printf ("now, the %s image has been loaded successfully!\n\n", szTestFile) ;
    printf ("image's infomation:\n") ;
    printf ("width   : %d\n", img.Width()) ;
    printf ("height  : %d\n", img.Height()) ;
    printf ("color   : %d\n\n", img.ColorBits()) ;

    img.ConvertTo32Bit() ;

    // print menu && choice a effect
    char     szInput[255] = {0} ;
    printf ("%s", szOpMenu) ;
    scanf ("%s", szInput) ;

    FCSinglePixelProcessBase   * pEffect = NULL ;
    switch (atoi(szInput))
    {
        case 1 : pEffect = new FCPixelInvert() ; break ;
        case 2 : pEffect = new FCPixelGrayscale() ; break ;
        case 3 : pEffect = new FCPixelThreshold(100) ; break ;
        case 4 : pEffect = new FCPixelFlip() ; break ;
        case 5 : pEffect = new FCPixelEmboss(2) ; break ;
        case 6 : pEffect = new FCPixelSplash(15) ; break ;
        case 7 : pEffect = new FCPixelMosaic(10) ; break ;
        case 8 : pEffect = new FCPixelOilPaint(7) ; break ;
        case 9 : pEffect = new FCPixel3DGrid(16,100) ; break ;
        case 10 : pEffect = new FCPixelWhirlPinch(LIB_PI/2.0,10) ; break ;
        case 11 :
            {
                RECT    rcEllipse = {0,0,img.Width(),img.Height()} ;
                RGBQUAD crStart = {255,255,255}, crEnd = {255,0,0} ;
                pEffect = new FCPixelGradientRadial(rcEllipse,crStart,crEnd) ;
            }
            break ;
        case 12 : pEffect = new FCPixelGamma(2) ; break ;
        case 13 : pEffect = new FCPixelRotate90() ; break ;
        case 14 : pEffect = new FCPixelRibbon(35,25) ; break ;
        case 15 : pEffect = new FCPixelHalftoneM3() ; break ;
        case 16 :
            {
                POINT   pt = {100, 100} ;
                pEffect = new FCPixelLensFlare(pt) ;
            }
            break ;
        case 17 : pEffect = new FCPixelHueSaturation(100,150) ; break ;
        case 18 : pEffect = new FCPixelBlur_Box(5,true) ; break ;
        case 19 : pEffect = new FCPixelBlur_Zoom(5) ; break ;
        case 20 : pEffect = new FCPixelBlur_Radial(5) ; break ;
        case 21 : pEffect = new FCPixelBlur_Motion(30, DIRECT_LEFT) ; break ;
        case 22 :
            {
                SHADOWDATA    ShData ;
                ShData.crShadow = PCL_RGBA(0,0,0) ;
                ShData.nAlpha = 75 ;
                ShData.nSmooth = 10 ;
                ShData.nOffsetX = 5 ;
                ShData.nOffsetY = 5 ;
                pEffect = new FCPixelAddShadow(ShData) ;
            }
            break ;
        case 23 : pEffect = new FCPixelColorTone(PCL_RGBA(254,168,33)) ; break ;
        case 24 : pEffect = new FCPixelSoftGlow(10, 60, 110) ; break ;
        case 25 : pEffect = new FCPixelTileReflection(45, 20, 8) ; break ;
        default :
            printf ("choice invalid. quit do nothing.\n") ;
            return 0 ;
    }

    // to show progress, it's obvious in large image and slower algorithm (such as : OilPaint)
    FCShowProgress     showPro ;
    printf ("\ncurrent progress : ") ;
    img.SinglePixelProcessProc (*pEffect, &showPro) ;
    printf ("\n\n") ;
    delete pEffect ;

    // save image
    FCObjImage   imgWhite ;
    imgWhite.Create (img.Width(), img.Height(), 24) ;
    FCPixelFillColor    cmdFillWhite(FCColor::crWhite()) ;
    imgWhite.SinglePixelProcessProc (cmdFillWhite) ;

    RECT   rcImg = {0, 0, img.Width(), img.Height()} ;
    imgWhite.AlphaBlend (img, rcImg, rcImg, 100) ;
    imgWhite.Save (szSaveFile) ;
    printf ("the effected image has been saved into %s !\n\n", szSaveFile) ;

    return 0 ;
}
