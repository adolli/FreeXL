#ifdef _MSC_VER
    #if _MSC_VER >= 1400
        #define _CRT_SECURE_NO_DEPRECATE
    #endif
#endif

#include <stdio.h>
#define PCL_3RD_LIBRARY_USE_FREEIMAGE
#define FREEIMAGE_LIB
#include "../../../ImageStone.h"

//================================================================================
int main (int argc, char* argv[])
{
    FCObjImage::SetImageHandleFactory (new FCImageHandleFactory_FreeImage) ;

	const char* szTestFile = "test.gif" ; // image to load

    // not found
    FILE   * pf = fopen (szTestFile, "rb") ;
    if (pf)
        fclose(pf) ;
    if (!pf)
    {
        printf ("can't load %s, please put a gif file named %s in same folder of exe file.\n", szTestFile, szTestFile) ;
        return 0 ;
    }

    FCObjMultiFrame   img ;
    if (!img.Load (szTestFile))
    {
        printf ("can't load %s, please put %s in same folder.\n", szTestFile, szTestFile) ;
        return 0 ;
    }

    printf ("%s loade success.\n\n", szTestFile) ;

    for (int i=0 ; i < img.GetFrameCount() ; i++)
    {
        std::string   s = FCOXOHelper::X2A(i) + ".jpg" ;
        img.GetFrame(i)->ConvertTo24Bit() ;
        img.GetFrame(i)->Save (s.c_str(), 90) ; // quality == 90
        printf ("page-%d save as %s\n", i, s.c_str()) ;
    }
    printf ("\n\n") ;

    return 0 ;
}
