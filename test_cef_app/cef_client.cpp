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

