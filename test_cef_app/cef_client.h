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

class Client
    : public CefClient
    //, public CefDisplayHandler
    , public CefLifeSpanHandler
    , public CefLoadHandler
{
public:
    Client() {}

    CefRefPtr<CefLifeSpanHandler>
    GetLifeSpanHandler() OVERRIDE
    {
        return this;
    }

    // CefLifeSpanHandler
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

private:
    IMPLEMENT_REFCOUNTING(Client);
    DISALLOW_COPY_AND_ASSIGN(Client);
};
