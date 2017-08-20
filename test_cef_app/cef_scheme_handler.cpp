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
    return new ClientSchemeHandlerFactory();
}







class ClientSchemeHandler
    : public CefResourceHandler
{
public:
    ClientSchemeHandler()
        : is_binary_(false)
        , delay_count_(0)
        , offset_(0)
    {
    }

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
    std::string                 mime_type_;
    std::vector<uint8_t>        data_;
    bool                        is_binary_;
    size_t                      delay_count_;
    size_t                      offset_;

private:
    IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
    DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
};

static
bool
ends_with(const std::string& str, const std::string& suffix)
{
    if ( str.size() < suffix.size() )
    {
        return false;
    }
    return std::equal( std::rbegin(suffix), std::rend(suffix), std::rbegin(str) );
    //return ( 0 == str.compare( str.size() - suffix.size(), suffix.size(), suffix ));
}

static
bool
parseURL(std::string& scheme, std::string& domain, std::string& path, const std::string& url)
{
    const size_t pos_scheme = url.find( "://" );
    if ( std::string::npos == pos_scheme )
    {
        return false;
    }

    scheme = url.substr( 0, pos_scheme );

    if ( url.size() <= (pos_scheme + 3/*strlen("://")*/) )
    {
        return false;
    }

    const size_t pos_domain = url.find( '/', (pos_scheme + 3/*strlen("://")*/) );
    if ( std::string::npos == pos_domain )
    {
        return false;
    }
    domain = url.substr( (pos_scheme + 3/*strlen("://")*/), pos_domain - (pos_scheme + 3/*strlen("://")*/) );

    if ( url.size() <= (pos_domain + 1/*strlen("/")*/) )
    {
        return false;
    }

    path = url.substr( (pos_domain + 1/*strlen("/")*/) );

    return true;
}

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif // defined(_MSC_VER)

static
bool
readResource( const std::string& path, std::vector<uint8_t>& data )
{
    std::string     relPath;
    relPath = "resources_scheme/";
    relPath += path;

    FILE* fp = fopen( relPath.c_str(), "rb" );
    if ( NULL == fp )
    {
        return false;
    }

    data.reserve( 256*1024 );

    char buff[16];
    size_t pos = 0;
    size_t len;
    while ( 0 < (len = fread( buff, 1, sizeof(buff), fp ) ) )
    {
        if ( data.size() < ( pos + len ) )
        {
            data.resize( (pos + len) );
        }
        memcpy( &(data.at(pos)), buff, len );
        pos += len;
    }

    fclose( fp );
    fp = NULL;

#if 0 // defined(OS_WIN)
    {
        char buff[128];
        ::wsprintfA( buff, "size=%u\n", data.size() );
        ::OutputDebugStringA( buff );
    }
    {
        std::string str;
        str.append( (const char*)&data.at(0), data.size() );
        ::OutputDebugStringA( str.c_str() );
    }
#endif

    return true;
}

bool
ClientSchemeHandler::ProcessRequest(
    CefRefPtr<CefRequest> request
    , CefRefPtr<CefCallback> callback
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    bool handled = false;

    std::string url = request->GetURL();

    std::string scheme;
    std::string domain;
    std::string path;
    {
        const bool bRet = parseURL( scheme, domain, path, url );
    }

    if ( ends_with( url, ".html" ) )
    {
        const bool bRet = readResource( path, this->data_ );
        if ( bRet )
        {
            handled = true;
            this->mime_type_ = "text/html";
        }
    }

    if ( handled )
    {
        callback->Continue();
    }

    return handled;
}

void
ClientSchemeHandler::GetResponseHeaders(
    CefRefPtr<CefResponse> response
    , int64& response_length
    , CefString& redirectUrl
) // OVERRIDE
{
    CEF_REQUIRE_IO_THREAD();

    DCHECK( false == this->data_.empty() );

    response->SetMimeType( this->mime_type_ );
    response->SetStatus( 200 );

    response_length = this->data_.size();
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

    bool transfer_data = false;

    bytes_read = 0;

    bool notready = true;
    {
        this->delay_count_ += 1;
        if ( 3 < this->delay_count_ )
        {
            this->delay_count_ = 0;
            notready = false;
        }
    }

    if ( notready )
    {
        bytes_read = 0;
        callback->Continue();
        transfer_data = true;
    }
    else
    {
        if ( this->offset_ < this->data_.size() )
        {
            bytes_to_read = 16;
            const int transfer_size =
                std::min(
                    bytes_to_read
                    , static_cast<int>( this->data_.size() - this->offset_ )
                    );
            memcpy( data_out, &(this->data_.at(this->offset_)), transfer_size );
            this->offset_ += transfer_size;

            bytes_read = transfer_size;
            transfer_data = true;
        }
    }

    return transfer_data;
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
