
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "../../../ImageStone.h"

//=============================================================================
/// add dot mask on image.
class FCPixelMyProcessor : public FCSinglePixelProcessBase
{
public:
    virtual void ProcessPixel (FCObjImage* pImg, int x, int y, BYTE* pPixel)
    {
        if (x%2 && y%2)
        {
            PCL_R(pPixel) = 49 ;
            PCL_G(pPixel) = 106 ;
            PCL_B(pPixel) = 197 ;
        }
    }
};

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
        printf ("can't load %s, please put a bmp file named %s in same folder of exe file.\n", szTestFile, szTestFile) ;
        return 0 ;
    }

    // load test image
    FCObjImage   img ;
    if (!img.Load (szTestFile))
    {
        printf ("load %s failed.\n", szTestFile) ;
        return 0 ;
    }
    img.ConvertTo24Bit() ;

    FCPixelMyProcessor   aCmd ;
    img.SinglePixelProcessProc (aCmd) ;

    img.Save (szSaveFile) ;
    printf ("the effected image has been saved into %s !\n\n", szSaveFile) ;

    return 0 ;
}
