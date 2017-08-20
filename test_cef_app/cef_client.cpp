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

#include "cef_client.h"

#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"

void
Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) // OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();

    // need counting
    CefQuitMessageLoop();
}

void
Client::OnLoadingStateChange(
    CefRefPtr<CefBrowser> browser
    , bool isLoading
    , bool canGoBack
    , bool canGoForward
) //OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();
}

#if 0
void
Client::OnLoadEnd(
    CefRefPtr<CefBrowser> browser
    , CefRefPtr<CefFrame> frame
    , int httpStatusCode
) //OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();
}

void
Client::OnLoadError(
    CefRefPtr<CefBrowser> browser
    , CefRefPtr<CefFrame> frame
    , ErrorCode errorCode
    , const CefString& errorText
    , const CefString& failedURL
) // OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();
}
#endif


CefRefPtr<CefResourceHandler>
Client::GetResourceHandler(
    CefRefPtr<CefBrowser> browser
    , CefRefPtr<CefFrame> frame
    , CefRefPtr<CefRequest> request
) //OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();
    return NULL;
}

// CefResourceHandler
CefRequestHandler::ReturnValue
Client::OnBeforeResourceLoad(
    CefRefPtr<CefBrowser> browser
    , CefRefPtr<CefFrame> frame
    , CefRefPtr<CefRequest> request
    , CefRefPtr<CefRequestCallback> callback
) //OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    request->GetURL();

    CefRequest::HeaderMap   headerMap;
    request->GetHeaderMap(headerMap);

    bool needSetHeaderMap = false;
    {
        std::multimap<CefString, CefString>::iterator it =
            headerMap.find( "User-Agent" );
        if ( it != headerMap.end() )
        {
            std::string     newUA;
            newUA = it->second;
            newUA.append( " test" );
            it->second = newUA;
            needSetHeaderMap = true;

            ++it;
            if ( it != headerMap.end() )
            {
                // multi-value ??
            }
        }
        else
        {
            //
        }
    }

    if ( needSetHeaderMap )
    {
        request->SetHeaderMap(headerMap);
    }

    return RV_CONTINUE;
}





#if defined(USE_CEF_OFFSCREEN)

extern HWND s_hWnd;

bool
Client::GetViewRect(
    CefRefPtr<CefBrowser> browser
    , CefRect& rect
) // OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();

    bool result = false;

#if defined(OS_WIN)
    {
        RECT    rectNative;
        const BOOL BRet = ::GetClientRect(s_hWnd, &rectNative);
        rect.width = rectNative.right - rectNative.left;
        rect.height = rectNative.bottom - rectNative.top;

        result = (BRet)?(true):(false);
    }
#endif // defined(OS_WIN)

    return result;
}

void
Client::OnPaint(
    CefRefPtr<CefBrowser> browser
    , PaintElementType type
    , const RectList& dirtyRects
    , const void* buffer
    , int width, int height
) // OVERRIDE
{
    CEF_REQUIRE_UI_THREAD();
}

#endif // defined(USE_CEF_OFFSCREEN)
