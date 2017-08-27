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

#pragma once

#include "include/cef_client.h"

#include "include/base/cef_lock.h"
#include <list>


class Client
    : public CefClient
    //, public CefDisplayHandler
    , public CefLifeSpanHandler
    , public CefLoadHandler
    , public CefRequestHandler
#if defined(USE_CEF_OFFSCREEN)
    , public CefRenderHandler
#endif // defined(USE_CEF_OFFSCREEN)
    , public CefFocusHandler
{
public:
    Client() {}

    CefRefPtr<CefLifeSpanHandler>
    GetLifeSpanHandler() OVERRIDE
    {
        return this;
    }

    // CefLifeSpanHandler
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;


    CefRefPtr<CefLoadHandler>
    GetLoadHandler() OVERRIDE
    {
        return this;
    }

    // CefLoadHandler
    void
    OnLoadingStateChange(
        CefRefPtr<CefBrowser> browser
        , bool isLoading
        , bool canGoBack
        , bool canGoForward
        ) OVERRIDE;

#if 0
    void
        OnLoadEnd(
        CefRefPtr<CefBrowser> browser
        , CefRefPtr<CefFrame> frame
        , int httpStatusCode
        ) OVERRIDE;

    void
    OnLoadError(
        CefRefPtr<CefBrowser> browser
        , CefRefPtr<CefFrame> frame
        , ErrorCode errorCode
        , const CefString& errorText
        , const CefString& failedURL
        ) OVERRIDE;
#endif

    CefRefPtr<CefRequestHandler>
    GetRequestHandler() OVERRIDE
    {
        return this;
    }

    // CefRequestHandler
    CefRefPtr<CefResourceHandler>
    GetResourceHandler(
        CefRefPtr<CefBrowser> browser
        , CefRefPtr<CefFrame> frame
        , CefRefPtr<CefRequest> request
        ) OVERRIDE;

    // CefResourceHandler
    CefRequestHandler::ReturnValue
    OnBeforeResourceLoad(
        CefRefPtr<CefBrowser> browser
        , CefRefPtr<CefFrame> frame
        , CefRefPtr<CefRequest> request
        , CefRefPtr<CefRequestCallback> callback
        ) OVERRIDE;


#if defined(USE_CEF_OFFSCREEN)
    CefRefPtr<CefRenderHandler>
    GetRenderHandler() OVERRIDE
    {
        return this;
    }

    // CefRenderHandler
    bool
    GetViewRect(
        CefRefPtr<CefBrowser> browser
        , CefRect& rect
        ) OVERRIDE;

    void
    OnPaint(
        CefRefPtr<CefBrowser> browser
        , PaintElementType type
        , const RectList& dirtyRects
        , const void* buffer
        , int width, int height
        ) OVERRIDE;

#endif // defined(USE_CEF_OFFSCREEN)


    CefRefPtr<CefFocusHandler>
    GetFocusHandler() OVERRIDE
    {
        return this;
    }

    // CefFocusHandler
    void OnGotFocus(
        CefRefPtr<CefBrowser> browser
        ) OVERRIDE;


private:
    IMPLEMENT_REFCOUNTING(Client);
    DISALLOW_COPY_AND_ASSIGN(Client);
};

