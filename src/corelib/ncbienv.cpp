/*  $Id$
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Authors:  Denis Vakatov, Eugene Vasilchenko
 *
 * File Description:
 *   Unified interface to application:
 *      environment     -- CNcbiEnvironment
 *      cmd.-line args  -- CNcbiArguments
 *
 */

#include <corelib/ncbienv.hpp>
#include <corelib/ncbifile.hpp>
#include <algorithm>

#ifdef NCBI_OS_LINUX
#include <unistd.h>
#endif

BEGIN_NCBI_SCOPE


///////////////////////////////////////////////////////
//  CNcbiEnvironment::


CNcbiEnvironment::CNcbiEnvironment(void)
{
    return;
}


CNcbiEnvironment::CNcbiEnvironment(const char* const* envp)
{
    Reset(envp);
}


CNcbiEnvironment::~CNcbiEnvironment(void)
{
    return;
}


void CNcbiEnvironment::Reset(const char* const* envp)
{
    // delete old environment values
    m_Cache.clear();


    // load new environment values from "envp"
    if ( !envp )
        return;

    for ( ;  *envp;  envp++) {
        const char* s = *envp;
        const char* eq = strchr(s, '=');
        if ( !eq ) {
            ERR_POST("CNcbiEnvironment: bad string '" << s << "'");
            continue;
        }
        m_Cache[string(s, eq)] = eq + 1;
    }
}


const string& CNcbiEnvironment::Get(const string& name) const
{
    map<string, string>::const_iterator i = m_Cache.find(name);
    if ( i != m_Cache.end() )
        return i->second;
    return m_Cache[name] = Load(name);
}


string CNcbiEnvironment::Load(const string& name) const
{
    const char* s = getenv(name.c_str());
    if ( !s )
        return NcbiEmptyString;
    else
        return s;
}




///////////////////////////////////////////////////////
//  CNcbiArguments::


CNcbiArguments::CNcbiArguments(int argc, const char* const* argv,
                               const string& program_name)
{
    Reset(argc, argv, program_name);
}


CNcbiArguments::~CNcbiArguments(void)
{
    return;
}


CNcbiArguments::CNcbiArguments(const CNcbiArguments& args)
    : m_ProgramName(args.m_ProgramName),
      m_Args(args.m_Args)
{
    return;
}


CNcbiArguments& CNcbiArguments::operator= (const CNcbiArguments& args)
{
    if (&args == this)
        return *this;

    m_ProgramName = args.m_ProgramName;
    m_Args.clear();
    copy(args.m_Args.begin(), args.m_Args.end(), back_inserter(m_Args));
    return *this;
}


void CNcbiArguments::Reset(int argc, const char* const* argv,
                           const string& program_name)
{
    // check args
    if (argc < 0) {
        NCBI_THROW(CArgumentsException,eNegativeArgc,
            "Negative number of command-line arguments");
    }

    if ((argc == 0) != (argv == 0)) {
        if (argv == 0) {
            NCBI_THROW(CArgumentsException,eNoArgs,
                "Command-line arguments are absent");
        }
        ERR_POST(Info <<
                 "CNcbiArguments(): zero \"argc\", non-zero \"argv\"");
    }

    // clear old args, store new ones
    m_Args.clear();
    for (int i = 0;  i < argc;  i++) {
        if ( !argv[i] ) {
            ERR_POST(Warning <<
                     "CNcbiArguments() -- NULL cmd.-line arg #" << i);
            continue;
        }
        m_Args.push_back(argv[i]);
    }

    // set application name
    SetProgramName(program_name);
}


const string& CNcbiArguments::GetProgramName(bool follow_links) const
{
    if (follow_links) {
        CFastMutexGuard LOCK(m_ResolvedNameMutex);
        if ( !m_ResolvedName.size() ) {
#ifdef NCBI_OS_LINUX
            string proc_link = "/proc/" + NStr::IntToString(getpid()) + "/exe";
            m_ResolvedName = CDirEntry::NormalizePath(proc_link, true);
#else
            m_ResolvedName = CDirEntry::NormalizePath(GetProgramName(false),
                                                      true);
#endif
        }
        return m_ResolvedName;
    } else if ( !m_ProgramName.empty() ) {
        return m_ProgramName;
    } else if ( m_Args.size() ) {
        return m_Args[0];
    } else {
        static const string kDefProgramName("ncbi");
        return kDefProgramName;
    }
}


string CNcbiArguments::GetProgramBasename(bool follow_links) const
{
    const string& name = GetProgramName(follow_links);
    SIZE_TYPE base_pos = name.find_last_of("/\\:");
    if (base_pos == NPOS)
        return name;
    return name.substr(base_pos + 1);
}


string CNcbiArguments::GetProgramDirname(bool follow_links) const
{
    const string& name = GetProgramName(follow_links);
    SIZE_TYPE base_pos = name.find_last_of("/\\:");
    if (base_pos == NPOS)
        return NcbiEmptyString;
    return name.substr(0, base_pos + 1);
}


void CNcbiArguments::SetProgramName(const string& program_name)
{
    m_ProgramName = program_name;
    CFastMutexGuard LOCK(m_ResolvedNameMutex);
    m_ResolvedName.erase();
}


void CNcbiArguments::Add(const string& arg)
{
    m_Args.push_back(arg);
}


END_NCBI_SCOPE


/*
 * ===========================================================================
 * $Log$
 * Revision 1.7  2003/09/30 15:10:11  ucko
 * CNcbiArguments::GetProgram{Name,Basename,Dirname}: optionally resolve symlinks.
 *
 * Revision 1.6  2002/07/15 18:17:24  gouriano
 * renamed CNcbiException and its descendents
 *
 * Revision 1.5  2002/07/11 14:18:26  gouriano
 * exceptions replaced by CNcbiException-type ones
 *
 * Revision 1.4  2002/04/11 21:08:02  ivanov
 * CVS log moved to end of the file
 *
 * Revision 1.3  2000/08/31 23:50:21  vakatov
 * CNcbiArguments:: Inlined Size() and operator[];   use <deque>
 *
 * Revision 1.2  2000/01/20 16:36:04  vakatov
 * Added class CNcbiArguments::   application command-line arguments & name
 * Added CNcbiEnvironment::Reset(), and comments to CNcbiEnvironment::
 * Dont #include <ncbienv.inl>
 *
 * Revision 1.1  1999/05/04 16:14:46  vasilche
 * Fixed problems with program environment.
 * Added class CNcbiEnvironment for cached access to C environment.
 *
 * ===========================================================================
 */
