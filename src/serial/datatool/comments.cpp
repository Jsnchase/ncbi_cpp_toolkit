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
* Author: Eugene Vasilchenko
*
* File Description:
*   !!! PUT YOUR DESCRIPTION HERE !!!
*
* ---------------------------------------------------------------------------
* $Log$
* Revision 1.2  2001/05/17 15:07:11  lavr
* Typos corrected
*
* Revision 1.1  2000/11/29 17:42:44  vasilche
* Added CComment class for storing/printing ASN.1/XML module comments.
* Added srcutil.hpp file to reduce file dependency.
*
* ===========================================================================
*/

#include <corelib/ncbistd.hpp>
#include <corelib/ncbiutil.hpp>
#include <serial/datatool/comments.hpp>
#include <serial/datatool/srcutil.hpp>

BEGIN_NCBI_SCOPE

CComments::CComments(void)
{
}

CComments::~CComments(void)
{
}

void CComments::Add(const string& s)
{
    m_Comments.push_back(s);
}

CNcbiOstream& CComments::Print(CNcbiOstream& out,
                               const string& before,
                               const string& between,
                               const string& after) const
{
    out << before;

    iterate ( TComments, i, m_Comments ) {
        if ( i != m_Comments.begin() )
            out << between;
        out << *i;
    }

    return out << after;
}

CNcbiOstream& CComments::PrintDTD(CNcbiOstream& out, int flags) const
{
    if ( Empty() ) // no comments
        return out;

    if ( !(flags & eDoNotWriteBlankLine) ) {
        // prepend comments by empty line to separate from previous comments
        out << '\n';
    }

    // comments start
    out <<
        "<!--";

    if ( !(flags & eAlwaysMultiline) && OneLine() ) {
        // one line comment
        out << m_Comments.front() << ' ';
    }
    else {
        // multiline comments
        out << '\n';
        iterate ( TComments, i, m_Comments ) {
            out << *i << '\n';
        }
    }

    // comments end
    out << "-->";
    
    if ( !(flags & eNoEOL) )
        out << '\n';

    return out;
}

CNcbiOstream& CComments::PrintASN(CNcbiOstream& out,
                                  int indent, int flags) const
{
    if ( Empty() ) // no comments
        return out;

    bool newLine = (flags & eDoNotWriteBlankLine) == 0;
    // prepend comments by empty line to separate from previous comments

    iterate ( TComments, i, m_Comments ) {
        if ( newLine )
            PrintASNNewLine(out, indent);
        out << "--" << *i;
        newLine = true;
    }

    if ( (flags & eNoEOL) == 0 )
        PrintASNNewLine(out, indent);

    return out;
}

END_NCBI_SCOPE
