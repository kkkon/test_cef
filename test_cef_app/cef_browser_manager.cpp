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

#include "cef_browser_manager.h"

#include "include/cef_app.h" // CefQuitMessageLoop
#include "include/cef_browser.h"
#include "include/wrapper/cef_helpers.h"


static
BrowserManager*         s_clientManager;


//static
BrowserManager*
BrowserManager::getInstance()
{
    CEF_REQUIRE_UI_THREAD();
    DCHECK(s_clientManager);

    return s_clientManager;
}

//static
void
BrowserManager::terminate()
{
    s_clientManager = NULL;
}




BrowserManager::BrowserManager()
{
    s_clientManager = this;
}

BrowserManager::~BrowserManager()
{
    DCHECK( mListBrowser.empty() );
    s_clientManager = NULL;
}


void
BrowserManager::appendBrowser( CefRefPtr<CefBrowser> browser )
{
    CEF_REQUIRE_UI_THREAD();

    DCHECK( s_clientManager );
    DCHECK( this );

    {
        base::AutoLock  scoped_lock(this->mLock);

        this->mListBrowser.push_back( browser );
    }
}


void
BrowserManager::removeBrowser( CefRefPtr<CefBrowser> browser )
{
    CEF_REQUIRE_UI_THREAD();

    DCHECK( s_clientManager );
    DCHECK( this );

    {
        base::AutoLock  scoped_lock(this->mLock);

        std::list< CefRefPtr<CefBrowser> >::iterator it =
            this->mListBrowser.begin();
        bool found = false;
        for ( ; it != this->mListBrowser.end(); ++it )
        {
            CefRefPtr<CefBrowser>& rBrowser = *it;
            if ( rBrowser->IsSame( browser ) )
            {
                this->mListBrowser.erase( it );
                found = true;
                break;
            }
        }
        DCHECK( found );

        if ( this->mListBrowser.empty() )
        {
            CefQuitMessageLoop();
        }
    }
}

bool
BrowserManager::isEmpty()
{
    DCHECK( s_clientManager );
    DCHECK( this );

    bool isEmpty = false;
    {
        base::AutoLock  scoped_lock(this->mLock);

        if ( this->mListBrowser.empty() )
        {
            isEmpty = true;
        }
    }

    return isEmpty;
}

void
BrowserManager::closeAllBrowser( bool forceClose )
{
    DCHECK( s_clientManager );
    DCHECK( this );

    {
        base::AutoLock  scoped_lock(this->mLock);

        std::list< CefRefPtr<CefBrowser> >::iterator it =
            this->mListBrowser.begin();
        bool found = false;
        for ( ; it != this->mListBrowser.end(); ++it )
        {
            CefRefPtr<CefBrowser>& rBrowser = *it;

            CefRefPtr<CefBrowserHost> host = rBrowser->GetHost();
            if ( NULL != host )
            {
                host->CloseBrowser( forceClose );
            }
        }
    }
}

