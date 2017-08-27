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

#include <list>
#include "include/cef_base.h"
#include "include/base/cef_lock.h"

class CefBrowser;

class BrowserManager
{
public:
    static BrowserManager* getInstance();
    static void terminate();

public:
    BrowserManager();
    ~BrowserManager();

public:
    void appendBrowser( CefRefPtr<CefBrowser> browser );
    void removeBrowser( CefRefPtr<CefBrowser> browser );

    bool isEmpty();
    void closeAllBrowser( bool forceClose );

private:
    base::Lock                                  mLock;
    std::list< CefRefPtr<CefBrowser> >          mListBrowser;

private:
    DISALLOW_COPY_AND_ASSIGN(BrowserManager);
};

