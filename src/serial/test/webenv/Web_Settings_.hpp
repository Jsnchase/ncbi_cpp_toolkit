/* $Id$
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
 */

/// @file Web_Settings_.hpp
/// Data storage class.
///
/// This file was generated by application DATATOOL
/// using the following specifications:
/// 'twebenv.asn'.
///
/// ATTENTION:
///   Don't edit or commit this file into CVS as this file will
///   be overridden (by DATATOOL) without warning!

#ifndef WEB_SETTINGS_BASE_HPP
#define WEB_SETTINGS_BASE_HPP

// standard includes
#include <serial/serialbase.hpp>

// generated includes
#include <list>


// forward declarations
class CArgument;
class CDb_Env;


// generated classes

/////////////////////////////////////////////////////////////////////////////
class CWeb_Settings_Base : public ncbi::CSerialObject
{
    typedef ncbi::CSerialObject Tparent;
public:
    // constructor
    CWeb_Settings_Base(void);
    // destructor
    virtual ~CWeb_Settings_Base(void);

    // type info
    DECLARE_INTERNAL_TYPE_INFO();

    // types
    typedef std::list< ncbi::CRef< CArgument > > TArguments;
    typedef std::list< ncbi::CRef< CDb_Env > > TDb_Env;

    // getters
    // setters

    /// optional
    /// typedef std::list< ncbi::CRef< CArgument > > TArguments
    bool IsSetArguments(void) const;
    bool CanGetArguments(void) const;
    void ResetArguments(void);
    const TArguments& GetArguments(void) const;
    TArguments& SetArguments(void);

    /// optional
    /// typedef std::list< ncbi::CRef< CDb_Env > > TDb_Env
    bool IsSetDb_Env(void) const;
    bool CanGetDb_Env(void) const;
    void ResetDb_Env(void);
    const TDb_Env& GetDb_Env(void) const;
    TDb_Env& SetDb_Env(void);

    /// Reset the whole object
    virtual void Reset(void);


private:
    // Prohibit copy constructor and assignment operator
    CWeb_Settings_Base(const CWeb_Settings_Base&);
    CWeb_Settings_Base& operator=(const CWeb_Settings_Base&);

    // data
    Uint4 m_set_State[1];
    TArguments m_Arguments;
    TDb_Env m_Db_Env;
};






///////////////////////////////////////////////////////////
///////////////////// inline methods //////////////////////
///////////////////////////////////////////////////////////
inline
bool CWeb_Settings_Base::IsSetArguments(void) const
{
    return ((m_set_State[0] & 0x3) != 0);
}

inline
bool CWeb_Settings_Base::CanGetArguments(void) const
{
    return true;
}

inline
const std::list< ncbi::CRef< CArgument > >& CWeb_Settings_Base::GetArguments(void) const
{
    return m_Arguments;
}

inline
std::list< ncbi::CRef< CArgument > >& CWeb_Settings_Base::SetArguments(void)
{
    m_set_State[0] |= 0x1;
    return m_Arguments;
}

inline
bool CWeb_Settings_Base::IsSetDb_Env(void) const
{
    return ((m_set_State[0] & 0xc) != 0);
}

inline
bool CWeb_Settings_Base::CanGetDb_Env(void) const
{
    return true;
}

inline
const std::list< ncbi::CRef< CDb_Env > >& CWeb_Settings_Base::GetDb_Env(void) const
{
    return m_Db_Env;
}

inline
std::list< ncbi::CRef< CDb_Env > >& CWeb_Settings_Base::SetDb_Env(void)
{
    m_set_State[0] |= 0x4;
    return m_Db_Env;
}

///////////////////////////////////////////////////////////
////////////////// end of inline methods //////////////////
///////////////////////////////////////////////////////////






#endif // WEB_SETTINGS_BASE_HPP
