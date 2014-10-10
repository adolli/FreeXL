
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "../../../ImageStone.h"

//================================================================================
/// Our format handler
class FCImageHandle_OurFormat : public FCImageHandleBase
{
    virtual bool LoadImageMemory (const BYTE* pStart, int nMemSize,
                                  PCL_Interface_Composite<FCObjImage>& rImageList,
                                  std::auto_ptr<FCImageProperty>& rImageProp)
    {
        std::string   strHead = "ImageStone" ;
        if (memcmp (pStart, strHead.c_str(), strHead.length()) != 0)
        {
            assert(false); return false;
        }

        FCObjImage   * pImg = new FCObjImage ;
        pImg->Serialize (false, const_cast<BYTE*>(pStart) + strHead.length()) ;
        rImageList.PCL_PushObject (pImg) ;
        return true ;
    }

    virtual bool SaveImageFile (const char* szFileName,
                                const std::deque<const FCObjImage*>& rImageList,
                                const FCImageProperty& rImageProp)
    {
        if (rImageList.empty() || !rImageList[0]->IsValidImage())
            return false ;
        FCObjImage   &img = *const_cast<FCObjImage*>(rImageList[0]) ;

        // create image file, if the file already exists, its contents are discarded.
        FILE   * pf = fopen (szFileName, "wb") ;
        if (!pf)
            return false ;

        PCL_array<BYTE>   pBuf (img.GetPitch()*img.Height() + 1024) ;
        int               nSize = img.Serialize (true, pBuf.get()) ;

        // image header - "ImageStone"
        std::string   strHead = "ImageStone" ;
        fwrite (strHead.c_str(), 1, strHead.length(), pf) ;
        fwrite (pBuf.get(), 1, nSize, pf) ;
        fclose (pf) ;
        return true ;
    }
};
//================================================================================
class FCImageHandleFactory_OurFormat : public FCImageHandleFactory_Mini
{
    virtual IMAGE_TYPE QueryImageFileType (const char* szFileName)
    {
        if (FCOXOHelper::GetFileExt(szFileName) == "foo")
            return IMG_CUSTOM ;
        return FCImageHandleFactory::QueryImageFileType (szFileName) ;
    }

    virtual FCImageHandleBase* CreateImageHandle (IMAGE_TYPE imgType)
    {
        if (imgType == IMG_CUSTOM)
        {
            return new FCImageHandle_OurFormat ;
        }
        return FCImageHandleFactory_Mini::CreateImageHandle (imgType) ;
    }
};
//================================================================================
int main (int argc, char* argv[])
{
    FCObjImage::SetImageHandleFactory (new FCImageHandleFactory_OurFormat) ;

    const char* szSaveFile = "save.foo" ; // image to save our format

    // make a gradient image
    FCObjImage   img ;
    img.Create (800, 600, 24) ;

    POINT     ptStart={0,0}, ptEnd={100,100} ;
    FCPixelGradientConicalSym   aCmd (ptStart, ptEnd, FCColor::crWhite(), PCL_RGBA(0,0,255), REPEAT_NONE) ;
    img.SinglePixelProcessProc (aCmd) ;

    // save image
    img.Save (szSaveFile) ;

    // test load image
    FCObjImage   imgLoad ;
    imgLoad.Load (szSaveFile) ;
    imgLoad.Save ("save.bmp") ;

    printf ("the effected image has been saved into %s !\n\n", szSaveFile) ;
    return 0 ;
}
