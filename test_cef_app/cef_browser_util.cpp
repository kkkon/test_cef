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

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"


//#define USE_CEF_VIEWS

#if defined(USE_CEF_VIEWS)
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"

namespace {

class WindowDelegate
    : public CefWindowDelegate
{
public:
    explicit WindowDelegate(CefRefPtr<CefBrowserView> browser_view)
        : browser_view_(browser_view)
    {
    }

private:
    CefRefPtr<CefBrowserView>   browser_view_;

    IMPLEMENT_REFCOUNTING(WindowDelegate);
    DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
};

} // namespace {
#endif // defined(USE_CEF_VIEWS)

static
bool
isUseViews(void)
{
    bool use_views = false;

#if defined(USE_CEF_VIEWS)
#if defined(OS_WIN) || defined(OS_LINUX)
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();

    use_views = command_line->HasSwitch("use-views");
#endif // defined(OS_WIN) || defined(OS_LINUX)
#endif // defined(USE_CEF_VIEWS)

    return use_views;
}



void createBrowser(
    CefRefPtr<CefClient> client
    , const CefString& startup_url
    , const CefBrowserSettings& settings
)
{
    CEF_REQUIRE_UI_THREAD();

    const bool use_views = isUseViews();

    if ( use_views )
    {
#if defined(USE_CEF_VIEWS)
        CefRefPtr<CefBrowserView> browser_view =
            CefBrowserView::CreateBrowserView(
                client, startup_url, settings, NULL, NULL
                );
        CefWindow::CreateTopLevelWindow( new WindowDelegate(browser_view) );
#endif // defined(USE_CEF_VIEWS)
    }
    else
    {
        CefWindowInfo   window_info;

#if defined(OS_WIN)
        CefWindowHandle parent = NULL;
        const CefString kName = "test_cef";
        window_info.SetAsPopup( parent, kName );

        window_info.width = 1280;
        window_info.height = 720;
#endif // defined(OS_WIN)

        CefRefPtr<CefRequestContext> request_context = NULL;

        CefBrowserHost::CreateBrowser(
            window_info, client, startup_url, settings, request_context
            );

    }

}

