/**
 * The MIT License
 * 
 * Copyright (C) 2017 Kiyofumi Kondoh
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "stdafx.h"
#include "test_cef_offscreen.h"

extern int main_win(HINSTANCE hInstance);
extern int main_win_term();

#include "include/base/cef_lock.h"


#define MAX_LOADSTRING 100

HWND        s_hWnd = NULL;
HDC         s_memDC = NULL;
HBITMAP     s_memBitmap = NULL;
void*       s_memBitmapPixel = NULL;
base::Lock  s_memBitmapPixelLock;
HBITMAP     s_memBitmapPrev = NULL;

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    {
        const int nRet = main_win(hInstance);
        if ( 0 <= nRet )
        {
            return nRet;
        }
    }


    MSG msg;
    HACCEL hAccelTable;

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TEST_CEF_OFFSCREEN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST_CEF_OFFSCREEN));

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    {
        main_win_term();
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST_CEF_OFFSCREEN));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_TEST_CEF_OFFSCREEN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   DWORD dwStyle = WS_OVERLAPPEDWINDOW;
   dwStyle ^= WS_MINIMIZEBOX;
   dwStyle ^= WS_MAXIMIZEBOX;
   dwStyle ^= WS_THICKFRAME;

   DWORD dwExStyle = 0;

   int width = CW_USEDEFAULT;
   int height = CW_USEDEFAULT;
   {
       RECT     rect;

       rect.left = 0;
       rect.top = 0;
       rect.bottom = 720;
       rect.right = 1280;

       ::AdjustWindowRectEx(&rect, dwStyle, TRUE, dwExStyle);

       width = rect.right - rect.left;
       height = rect.bottom - rect.top;
   }


   hWnd = CreateWindow(szWindowClass, szTitle, dwStyle,
      CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   s_hWnd = hWnd;

   return TRUE;
}


#include "../test_cef_app/cef_client.h"
#include "../test_cef_app/cef_browser_manager.h"

extern CefRefPtr<CefBrowser> getBrowser();

#include <windowsx.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
        {
            hdc = GetDC( hWnd );

            s_memDC = CreateCompatibleDC( hdc );
            {
                BITMAPINFO bi;

                ZeroMemory( &bi, sizeof(bi) );
                bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bi.bmiHeader.biBitCount = 32;
                bi.bmiHeader.biPlanes = 1;
                bi.bmiHeader.biWidth = 1280;
                bi.bmiHeader.biHeight = -720;

                {
                    base::AutoLock  lock_scope(s_memBitmapPixelLock);

                    s_memBitmap = CreateDIBSection( NULL, &bi, DIB_RGB_COLORS, &s_memBitmapPixel, NULL, 0 );
                    if ( NULL == s_memBitmap )
                    {
                        ReleaseDC( hWnd, hdc );
                        return -1;
                    }

                    {
                        DWORD* p = reinterpret_cast<DWORD*>(s_memBitmapPixel);
                        for ( int x = 0; x < 1280; ++x )
                        {
                            for ( int y = 0; y < 720; ++y )
                            {
                                // A,R,G,B
                                //p[y*1280 + x] = 0xffffffffUL;
                                p[y*1280 + x] = 0x0000ff00UL;
                            }
                        }
                    }
                }
            }
            s_memBitmapPrev = (HBITMAP)SelectObject( s_memDC, s_memBitmap );

            ReleaseDC( hWnd, hdc );
        }
        break;

    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        BitBlt( hdc, 0, 0, 1280, 720, s_memDC, 0, 0, SRCCOPY );
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        {
            if ( NULL != s_memDC )
            {
                if ( NULL != s_memBitmap )
                {
                    base::AutoLock  lock_scope(s_memBitmapPixelLock);

                    SelectObject( s_memDC, s_memBitmapPrev );
                    s_memBitmapPrev = NULL;
                    DeleteObject( s_memBitmap );
                    s_memBitmap = NULL;
                    s_memBitmapPixel = NULL;
                }

                DeleteDC( s_memDC );
                s_memDC = NULL;
            }
        }
        PostQuitMessage(0);
        break;

#if 0
    case WM_CLOSE:
        {
            bool cancelClose = false;

            BrowserManager* pManager = BrowserManager::getInstance();
            pManager->closeAllBrowser( false );

            for ( int retry = 0; retry < 3; ++retry )
            {
                ::Sleep( 3 * 1000 );
                if ( pManager->isEmpty() )
                {
                    break;
                }
            }

            if ( !cancelClose )
            {
                DestroyWindow( hWnd );
            }
        }
        break;
#endif



    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        {
            CefRefPtr<CefBrowser> browser = getBrowser();
            if ( NULL != browser )
            {
                CefRefPtr<CefBrowserHost> host = browser->GetHost();
                if ( NULL != host )
                {
                    CefMouseEvent event;
                    event.x = GET_X_LPARAM(lParam);
                    event.y = GET_Y_LPARAM(lParam);
                    event.modifiers = 0;
                    const bool btnUp = (WM_LBUTTONUP == message)?(true):(false);
                    host->SendMouseClickEvent( event, MBT_LEFT, btnUp, 1 );
                }
            }
        }
        break;

    case WM_MOUSEMOVE:
        {
            CefRefPtr<CefBrowser> browser = getBrowser();
            if ( NULL != browser )
            {
                CefRefPtr<CefBrowserHost> host = browser->GetHost();
                if ( NULL != host )
                {
                    CefMouseEvent event;
                    event.x = GET_X_LPARAM(lParam);
                    event.y = GET_Y_LPARAM(lParam);
                    event.modifiers = 0;
                    const bool mouseLeave = false;
                    host->SendMouseMoveEvent( event, mouseLeave );
                }
            }
        }
        break;

    case WM_MOUSEWHEEL:
        {
            CefRefPtr<CefBrowser> browser = getBrowser();
            if ( NULL != browser )
            {
                CefRefPtr<CefBrowserHost> host = browser->GetHost();
                if ( NULL != host )
                {
                    POINT screenPoint = {
                        GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)
                    };
                    HWND scrolledWnd = ::WindowFromPoint( screenPoint );
                    if ( hWnd == scrolledWnd )
                    {
                        ::ScreenToClient( hWnd, &screenPoint );
                        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

                        CefMouseEvent event;
                        event.x = screenPoint.x;
                        event.y = screenPoint.y;
                        event.modifiers = 0;
                        if ( ::GetKeyState( VK_SHIFT ) & 0x8000U )
                        {
                            host->SendMouseWheelEvent( event, delta, 0 );
                        }
                        else
                        {
                            host->SendMouseWheelEvent( event, 0, delta );
                        }
                    }
                }
            }
        }
        break;

    //case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        {
            CefRefPtr<CefBrowser> browser = getBrowser();
            if ( NULL != browser )
            {
                const DWORD fwKeys = GET_KEYSTATE_WPARAM( wParam );
                const DWORD fwButton = GET_XBUTTON_WPARAM( wParam );

                if ( fwButton & XBUTTON1 )
                {
                    if ( browser->CanGoBack() )
                    {
                        browser->GoBack();
                    }
                }
                else
                if ( browser->CanGoForward() )
                {
                    if ( browser->CanGoForward() )
                    {
                        browser->GoForward();
                    }
                }
            }
        }
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
        {
            CefRefPtr<CefBrowser> browser = getBrowser();
            if ( NULL != browser )
            {
                bool needSendKeyEvent = true;

                if (
                    (::GetKeyState( VK_RIGHT ) & 0x8000U )
                    && (::GetKeyState( VK_CONTROL ) & 0x8000U )
                )
                {
                    DCHECK( needSendKeyEvent );
                    if ( browser->CanGoForward() )
                    {
                        browser->GoForward();
                        needSendKeyEvent = false;
                    }
                }

                if (
                    (::GetKeyState( VK_LEFT ) & 0x8000U )
                    && (::GetKeyState( VK_CONTROL ) & 0x8000U )
                )
                {
                    DCHECK( needSendKeyEvent );
                    if ( browser->CanGoBack() )
                    {
                        browser->GoBack();
                        needSendKeyEvent = false;
                    }
                }


                if (
                    (::GetKeyState( VK_F5 ) & 0x8000U )
                )
                {
                    DCHECK( needSendKeyEvent );
                    if ( browser->IsLoading() )
                    {
                        browser->StopLoad();
                    }

                    browser->Reload();
                    needSendKeyEvent = false;
                }


                if (
                    (::GetKeyState( 'N' ) & 0x8000U )
                    && (::GetKeyState( VK_CONTROL ) & 0x8000U )
                )
                {
                    DCHECK( needSendKeyEvent );
                    extern void createNewBrowser();
                    createNewBrowser();
                    needSendKeyEvent = false;
                }


                if ( needSendKeyEvent )
                {
                    CefRefPtr<CefBrowserHost> host = browser->GetHost();
                    if ( NULL != host )
                    {
                        CefKeyEvent event;
                        event.windows_key_code = wParam;
                        event.native_key_code = lParam;
                        event.is_system_key = 0;
                        event.type = (WM_KEYDOWN == message)?(KEYEVENT_RAWKEYDOWN):(KEYEVENT_KEYUP);
                        event.modifiers = 0;

                        if ( ::GetKeyState( VK_CONTROL ) & 0x8000U )
                        {
                            event.modifiers |= EVENTFLAG_CONTROL_DOWN;
                        }
                        if ( ::GetKeyState( VK_SHIFT ) & 0x8000U )
                        {
                            event.modifiers |= EVENTFLAG_SHIFT_DOWN;
                        }

                        host->SendKeyEvent( event );
                    }
                }
            }
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

