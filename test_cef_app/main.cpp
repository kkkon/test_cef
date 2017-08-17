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

#if defined(_WIN32)

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif // defined(_MSC_VER)

extern int main_win(HINSTANCE hInstance);

static
HINSTANCE           s_hInstance = NULL;

int
APIENTRY
_tWinMain(
    HINSTANCE hInstance
    ,HINSTANCE hPrevInstance
    ,LPTSTR lpCmdLine
    ,int nCmdShow
)
{
#if defined(_MSC_VER)
#if defined(_DEBUG)
    {
        int flag = ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flag |= _CRTDBG_LEAK_CHECK_DF;
        flag |= _CRTDBG_CHECK_ALWAYS_DF;
        ::_CrtSetDbgFlag(flag);
    }
#endif // defined(_DEBUG)
#endif // defined(_MSC_VER)

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    s_hInstance = hInstance;

    return main_win(s_hInstance);
}

#else
int main(int argc, char* argv[])
{
    
}
#endif
