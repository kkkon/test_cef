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
#include "test_cef_dll.h"

#include "include/cef_base.h"
#include "include/cef_app.h"

#if defined(_MSC_VER)
#pragma comment(lib,"libcef.lib")
#pragma comment(lib,"VS2013/libcef_dll_wrapper.lib")
#endif // defined(_MSC_VER)


#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)


BOOL
test_cef_core__init(void)
{
    return TRUE;
}

BOOL
test_cef_core__term(void)
{
    CefShutdown();

    return TRUE;
}




#if defined(__cplusplus)
} // extern "C" {
#endif // defined(__cplusplus)


