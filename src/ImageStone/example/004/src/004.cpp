#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "../../../ImageStone.h"

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------------
__declspec(dllexport) HBITMAP CreateDDB_ImageStone (const char* szFilename)
{
    FCObjImage   img ;
    img.Load (szFilename) ;
    return FCWin32::CreateDDBHandle(img) ;
}
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
