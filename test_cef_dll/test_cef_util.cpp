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
#include "test_cef_util.h"

namespace {

const char kProcessType[] = "type";

const char kRendererProcess[] = "renderer";
#if defined(LINUX)
const char kZygoteProcess[] = "zygote";
#endif // defined(LINUX)

} // namespace



CefRefPtr<CefCommandLine>
createCommandLine( const CefMainArgs& main_args )
{
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();

#if defined(_WIN32)
    command_line->InitFromString(::GetCommandLineW());
#else
    command_line->InitFromArgv( main_args.argc, main_args.argv );
#endif

    return command_line;
}


ProcessType
getProcessType( const CefRefPtr<CefCommandLine>& command_line )
{
    if ( command_line->HasSwitch( kProcessType ) )
    {
        // none
    }
    else
    {
        return enmProcessType_Browser;
    }

    const std::string& process_type = command_line->GetSwitchValue( kProcessType );

    if ( process_type == kRendererProcess )
    {
        return enmProcessType_Renderer;
    }

#if defined(LINUX)
    if ( process_type == kZygoteProcess )
    {
        return enmProcessType_Renderer;
    }
#endif // defined(LINUX)

    return enmProcessType_Other;
}

