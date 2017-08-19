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

#include "cef_scheme_handler.h"

#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"


namespace {
const char kScheme[] = "local";
const char kDomain[] = "test";
} // namespace {


static
CefRefPtr<CefSchemeHandlerFactory>
createClientSchemeHandlerFactory();

static
CefRefPtr<CefResourceHandler>
createClientSchemeHandler();



void registerSchemeHandlerFactory()
{
    const bool bRet = CefRegisterSchemeHandlerFactory(
        kScheme
        , kDomain
        , createClientSchemeHandlerFactory()
        );

    DCHECK(bRet);
}


void addCustomScheme( CefRawPtr<CefSchemeRegistrar> registrar )
{
    const bool is_standard = true;
    const bool is_local = false;
    const bool is_display_isolated = false;
    const bool is_secure = true;
    const bool is_cors_enabled = false;
    const bool is_csp_bypassing = false;

    const bool bRet = registrar->AddCustomScheme(
        kScheme
        , is_standard
        , is_local
        , is_display_isolated
        , is_secure
        , is_cors_enabled
#if CHROME_VERSION_BUILD > 2987
        , is_csp_bypassing
#endif // CHROME_VERSION_BUILD > 2987
        );
    DCHECK( bRet );
}









class ClientSchemeHandlerFactory
    : public CefSchemeHandlerFactory
{
public:
    ClientSchemeHandlerFactory() {}

    CefRefPtr<CefResourceHandler>
    Create(
        CefRefPtr<CefBrowser> browser
        , CefRefPtr<CefFrame> frame
        , const CefString& scheme_name
        , CefRefPtr<CefRequest> request
        ) OVERRIDE;

private:
    IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
    DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
};


CefRefPtr<CefResourceHandler>
ClientSchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser
    , CefRefPtr<CefFrame> frame
    , const CefString& scheme_name
    , CefRefPtr<CefRequest> request
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    return createClientSchemeHandler();
}


CefRefPtr<CefSchemeHandlerFactory>
createClientSchemeHandlerFactory()
{
    return NULL;
}







class ClientSchemeHandler
    : public CefResourceHandler
{
public:
    ClientSchemeHandler() {}

    bool
    ProcessRequest(
        CefRefPtr<CefRequest> request
        , CefRefPtr<CefCallback> callback
        ) OVERRIDE;

    void
    GetResponseHeaders(
        CefRefPtr<CefResponse> response
        , int64& response_length
        , CefString& redirectUrl
        ) OVERRIDE;

    bool
    ReadResponse(
        void* data_out
        , int bytes_to_read
        , int& bytes_read
        , CefRefPtr<CefCallback> callback
        ) OVERRIDE;

    void
    Cancel() OVERRIDE;

private:
    IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
    DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
};



bool
ClientSchemeHandler::ProcessRequest(
    CefRefPtr<CefRequest> request
    , CefRefPtr<CefCallback> callback
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    return false;
}

void
ClientSchemeHandler::GetResponseHeaders(
    CefRefPtr<CefResponse> response
    , int64& response_length
    , CefString& redirectUrl
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();
}

bool
ClientSchemeHandler::ReadResponse(
    void* data_out
    , int bytes_to_read
    , int& bytes_read
    , CefRefPtr<CefCallback> callback
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    return true;
}

void
ClientSchemeHandler::Cancel() // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();
}





CefRefPtr<CefResourceHandler>
createClientSchemeHandler()
{
    return new ClientSchemeHandler();
}
