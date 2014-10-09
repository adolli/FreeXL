// 009.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
FCObjImage imgBack, imgShow ;
FCAutoInitGDIPlus _iniGDIP ;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    if (PathFileExists(_T("c:\\test.jpg")))
    {
        imgBack.Load ("c:\\test.jpg") ;
    }
    else
    {
        ::MessageBox (NULL, _T("Please put an image at c:\\test.jpg"), _T("Tip"), MB_OK) ;
        imgBack.Create (600, 300, 24) ;
        imgBack.SinglePixelProcessProc (FCPixelFillColor(FCColor::crWhite())) ;
    }
    imgShow = imgBack ;

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY009, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY009);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MY009);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= ::CreateSolidBrush(RGB(255,255,255)) ;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY009);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
        case WM_CREATE:
            PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_FILL_COLOR,0), 0) ;
            return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
            FCWin32::DrawImage (imgShow, hdc, 0, 0) ;
			EndPaint(hWnd, &ps);
			break;

        // process command
        case WM_COMMAND :
            {
                wmId    = LOWORD(wParam);
                // Parse the menu selections:
                switch (wmId)
                {
                case IDM_ABOUT:
                    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                }

                imgShow = imgBack ;
                FCObjTextLayer  imgT ;
                PACK_TextLayer  tp ;
                tp.m_bAddShadow = false ;
                tp.m_bAntiAliased = true ;
                tp.m_bBold = true ;
                tp.m_bItalic = true ;
                tp.m_crFont = PCL_RGBA(0,0,255) ;
                tp.m_nFontSize = 128 ;
                tp.m_strFace = "Arial" ;
                tp.m_strText = "Hello" ;
                FCWin32::GDIPlus_CreateTextLayer (imgT, tp) ;
                RECT   rc = {0, 0, imgT.Width(), imgT.Height()} ;
                if (wmId == ID_FILL_COLOR)
                {
                }
                else if (wmId == ID_FILL_PATTERN)
                {
                    FCObjImage   * p = new FCObjImage ;
                    FCWin32::LoadImageRes (*p, MAKEINTRESOURCE(IDB_PATTERN), _T("JPG"), IMG_JPG) ;
                    FCPixelFillPattern   aCmd(p, 255, false) ;
                    FCObjImage   imgAlpha ;
                    imgT.GetAlphaChannel(&imgAlpha) ;
                    imgT.SinglePixelProcessProc (aCmd) ;
                    imgT.AppendAlphaChannel(imgAlpha) ;
                }
                else if (wmId == ID_FILL_GRADIENT)
                {
                    POINT   pt1={0,0}, pt2={0,rc.bottom} ;
                    FCPixelGradientLine   aCmd (pt1, pt2, PCL_RGBA(0,0,255), FCColor::crWhite()) ;
                    imgT.SinglePixelProcessProc (aCmd) ;
                }

                // add shadow
                SHADOWDATA   sh ;
                sh.nOffsetX = sh.nOffsetY = 5 ;
                sh.crShadow = PCL_RGBA(75,75,75) ;
                sh.nAlpha = 75 ;
                sh.nSmooth = 10 ;
                imgT.SinglePixelProcessProc (FCPixelAddShadow(sh)) ;

                imgShow.AlphaBlend (imgT, rc, rc, 90) ;
                ::InvalidateRect(hWnd, NULL, TRUE) ;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
