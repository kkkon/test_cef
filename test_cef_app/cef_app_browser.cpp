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

#include "include/cef_base.h"

#include "cef_app_browser.h"

#include "cef_scheme_handler.h"

CefRefPtr<CefApp>
createAppBrowserProcess()
{
    return new AppBrowser();
}


void
AppBrowser::OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar
) // OVERRIDE
{
    addCustomScheme( registrar );
}


#include "cef_client.h"
#include "cef_browser_util.h"

namespace {
//const char kStartupURL[] = "https://www.google.com";
const char kStartupURL[] = "file:///resources_scheme/test.html";
} // namespace {

void
AppBrowser::OnContextInitialized() //OVERRIDE
{
    registerSchemeHandlerFactory();

    createBrowser(new Client(), kStartupURL, CefBrowserSettings());
}
