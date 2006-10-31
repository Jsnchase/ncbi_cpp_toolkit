#ifndef CORELIB___SYSLOG__HPP
#define CORELIB___SYSLOG__HPP

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
 * Author:  Aaron Ucko
 *
 */

/// @file syslog.hpp
/// Portable system-logging API.
///
/// This interface encapsulates the syslog() facility on Unix.

#include <corelib/ncbidiag.hpp>
#include <corelib/ncbimtx.hpp>
#include <corelib/ncbistr.hpp>

/** @addtogroup Diagnostics
 *
 * @{
 */

BEGIN_NCBI_SCOPE

class IRegistry;

class NCBI_XNCBI_EXPORT CSysLog : public CDiagHandler {
public:
    enum EFlags {
        fNoOverride        = 0x40000000, ///< never call openlog() ourselves
        fCopyToStderr      = 0x20000000, ///< maps to LOG_PERROR if available
        fFallBackToConsole = 0x10000000, ///< LOG_CONS
        fIncludePID        = 0x08000000, ///< LOG_PID
        fConnectNow        = 0x04000000, ///< LOG_NDELAY
        fNoChildWait       = 0x02000000, ///< LOG_NOWAIT
        fAllFlags          = 0x7e000000
    };
    typedef int TFlags; // binary OR of EFlags

    enum EPriority {
        eEmergency,
        eAlert,
        eCritical,
        eError,
        eWarning,
        eNotice,
        eInfo,
        eDebug
    };

    enum EFacility {
        eDefaultFacility,
        eKernel,
        eUser,
        eMail,
        eDaemon,
        eAuth,
        eSysLog,
        eLPR,
        eNews,
        eUUCP,
        eCron,
        eAuthPriv,
        eFTP,
        eLocal0,
        eLocal1,
        eLocal2,
        eLocal3,
        eLocal4,
        eLocal5,
        eLocal6,
        eLocal7
    };

    CSysLog(const string& ident = kEmptyStr, TFlags flags = fNoOverride,
            EFacility default_facility = eDefaultFacility);
    CSysLog(const string& ident, TFlags flags, int default_facility);
    ~CSysLog();
    
    void Post(const SDiagMessage& mess);
    void Post(const string& message, EPriority priority,
              EFacility facility = eDefaultFacility);
    void Post(const string& message, EPriority priority, int facility);

    void HonorRegistrySettings(IRegistry* reg = 0);

    string GetLogName(void) { return "SYSLOG"; }

private:
    static int x_TranslateFlags   (TFlags flags);
           int x_TranslateFacility(EFacility facility);

    void x_Connect(void);

    DECLARE_CLASS_STATIC_MUTEX(sm_Mutex);
    static CSysLog* sm_Current;
    string          m_Ident;
    TFlags          m_Flags;
    int             m_DefaultFacility;
};

typedef CSysLog CSysLogDiagHandler;

END_NCBI_SCOPE

/* @} */

/*
 * ===========================================================================
 * $Log$
 * Revision 1.5  2006/10/31 18:41:16  grichenk
 * Redesigned diagnostics setup.
 * Moved the setup function to ncbidiag.cpp.
 *
 * Revision 1.4  2006/08/30 18:05:02  ucko
 * Add an HonorRegistrySettings method to support tuning the default facility.
 *
 * Revision 1.3  2005/12/29 15:39:26  jcherry
 * Added export specifier for CSysLog
 *
 * Revision 1.2  2005/07/28 17:24:14  ucko
 * Shift flags down one place to stay clear of the sign bit.
 *
 * Revision 1.1  2005/07/27 15:25:36  ucko
 * Add a new diagnostic handler class encapsulating system logging facilities.
 *
 * ===========================================================================
 */

#endif  /* CORELIB___SYSLOG__HPP */
