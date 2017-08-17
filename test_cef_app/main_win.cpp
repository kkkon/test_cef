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
#include "include/cef_app.h"


#if defined(_MSC_VER)
#pragma comment(lib,"libcef.lib")
#pragma comment(lib,"VS2013/libcef_dll_wrapper.lib")
#endif // defined(_MSC_VER)

//#define CEF_USE_SANDBOX

#if defined(CEF_USE_SANDBOX)
#include "include/cef_sandbox_win.h"
#if defined(_MSC_VER)
#pragma comment(lib,"cef_sandbox.lib")
#endif // defined(_MSC_VER)
#endif defined(CEF_USE_SANDBOX)



#include "main_util.h"
#include "cef_app_factory.h"


int main_win(HINSTANCE hInstance)
{
    void*   sandbox_info = NULL;

#if defined(OS_WIN)
    CefEnableHighDPISupport();

#if defined(CEF_USE_SANDBOX)
    CefScopedSandboxInfo    scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif defined(CEF_USE_SANDBOX)

    CefMainArgs     main_args(hInstance);
#endif // defined(OS_WIN)

    CefRefPtr<CefApp>   app;

    CefRefPtr<CefCommandLine>   command_line = createCommandLine(main_args);
    switch (getProcessType(command_line))
    {
    case enmProcessType_Browser:
        app = createAppBrowserProcess();
        break;
    case enmProcessType_Renderer:
        //app = createAppRendererProcess();
        break;
    case enmProcessType_Other:
        //app = createAppOtherProcess();
        break;
    }

    const int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
    if (0 <= exit_code)
    {
        return exit_code;
    }

    CefSettings         settings;
#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif // !defined(CEF_USE_SANDBOX)

    CefInitialize( main_args, settings, app, sandbox_info );

    CefRunMessageLoop();

    CefShutdown();

    return 0;
}
