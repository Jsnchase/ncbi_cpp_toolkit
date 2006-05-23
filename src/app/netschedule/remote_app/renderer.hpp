#ifndef __NETSCHEDULE_INFO_RENDERER__HPP
#define __NETSCHEDULE_INFO_RENDERER__HPP

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
 * Authors: Maxim Didenko
 *
 * File Description:
 *
 */

#include <corelib/ncbimisc.hpp>
#include <connect/ncbi_types.h>

#include "info_collector.hpp"

BEGIN_NCBI_SCOPE

///////////////////////////////////////////////////////////////////
class ITagWriter
{
public:
    typedef list<pair<string, string> > TAttributes;

    virtual ~ITagWriter() {}

    void WriteTag(const string& name, const string& text = "") 
    { x_WriteTag(name, NULL, text, true); }
    void WriteBeginTag(const string& name) { x_WriteTag(name, NULL,  "", false); }

    void WriteTag(const string& name, const TAttributes& attr, const string& text = "") 
    { x_WriteTag(name, &attr, text, true); }
    void WriteBeginTag(const string& name,  const TAttributes& attr)
    { x_WriteTag(name, &attr, "", false); }

    void WriteCloseTag(const string& name) { x_WriteCloseTag(name); }

    void WriteText(const string& text) { x_WriteText(text); }
    void WriteStream(CNcbiIstream& is) { x_WriteStream(is); }


private:

    virtual void x_WriteText(const string& text) = 0;
    virtual void x_WriteStream(CNcbiIstream& is) = 0;

    virtual void x_WriteTag(const string& name, 
                            const TAttributes* attr,
                            const string& text,
                            bool close_tag) = 0;

    virtual void x_WriteCloseTag(const string& name) = 0;
};

///////////////////////////////////////////////////////////////////
///
class CTextTagWriter : public ITagWriter
{
public:
    CTextTagWriter(CNcbiOstream& os) : m_Os(os) {}
    virtual ~CTextTagWriter() {}

private:
    
    virtual void x_WriteText(const string& text);
    virtual void x_WriteStream(CNcbiIstream& is);

    virtual void x_WriteTag(const string& name, 
                              const TAttributes* attr,
                              const string& text,
                              bool close_tag);

    virtual void x_WriteCloseTag(const string& name);

    CNcbiOstream& m_Os;
};

///////////////////////////////////////////////////////////////////
///
class CXmlTagWriter : public ITagWriter
{
public:
    CXmlTagWriter(CNcbiOstream& os, const string& indent = "   ");
    virtual ~CXmlTagWriter();

private:
    virtual void x_WriteText(const string& text);
    virtual void x_WriteStream(CNcbiIstream& is);
    
    virtual void x_WriteTag(const string& name, 
                             const TAttributes* attr,
                             const string& text,
                             bool close_tag);

    virtual void x_WriteCloseTag(const string& name);

    void x_WriteIndent();

    CNcbiOstream& m_Os;
    int m_IndentCount;
    string m_IndentString;
};

///////////////////////////////////////////////////////////////////
class CNSInfoRenderer
{
public:
    enum EItem {
        eId        = 1 << 0,
        eStatus    = 1 << 1,
        eRetCode   = 1 << 2,
        eCmdLine   = 1 << 3,
        eErrMsg    = 1 << 4,
        eStdIn     = 1 << 5,
        eStdOut    = 1 << 6,
        eStdErr    = 1 << 7,
        eRawInput  = 1 << 8,
        eRawOutput = 1 << 9,
        
        eMinimal   = eId | eStatus | eRetCode,
        eStandard  = eMinimal | eCmdLine | eErrMsg,
        eFull      = eStandard | eStdIn | eStdOut | eStdErr
    };
    typedef int TFlags;

    CNSInfoRenderer(ITagWriter& writer, CNSInfoCollector& collector);
    ~CNSInfoRenderer();

    void RenderJob(const string& job_id, TFlags flags = eMinimal);
    void RenderJob(const CNSJobInfo& info, TFlags flags = eMinimal);
    void RenderJobByStatus(CNetScheduleClient::EJobStatus status,
                           TFlags flags = eMinimal);

    void RenderBlob(const string& blob_id);

    void RenderWNodes(TFlags flags = eMinimal);
    void RenderWNode(const CWNodeInfo& info, TFlags flags = eMinimal);
private:
    void x_RenderString(const string& name, const string& value);
    void x_RenderStream(const string& name, CNcbiIstream& is);

    ITagWriter& m_Writer;
    CNSInfoCollector& m_Collector;
};

END_NCBI_SCOPE

/*
 * ===========================================================================
 * $Log$
 * Revision 1.3  2006/05/23 14:05:36  didenko
 * Added wnlist, shutdown_nodes and kill_nodes commands
 *
 * Revision 1.2  2006/05/22 18:13:35  didenko
 * + Jobs err_msg report
 *
 * Revision 1.1  2006/05/19 13:40:40  didenko
 * Added ns_remote_job_control utility
 *
 * ===========================================================================
 */

#endif 

