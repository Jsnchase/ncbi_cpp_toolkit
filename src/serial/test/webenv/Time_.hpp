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
 * File Description:
 *   This code is generated by application DATATOOL
 *   using specifications from the ASN data definition file
 *   'twebenv.asn'.
 *
 * ATTENTION:
 *   Don't edit or check-in this file to the CVS as this file will
 *   be overridden (by DATATOOL) without warning!
 * ===========================================================================
 */

#ifndef TIME_BASE_HPP
#define TIME_BASE_HPP

// standard includes
#include <serial/serialbase.hpp>

// forward declarations
class CFull_Time;


// generated classes

class CTime_Base : public ncbi::CSerialObject
{
    typedef ncbi::CSerialObject Tparent;
public:
    // constructor
    CTime_Base(void);
    // destructor
    virtual ~CTime_Base(void);

    // type info
    DECLARE_INTERNAL_TYPE_INFO();

    // choice state enum
    enum E_Choice {
        e_not_set = 0,
        e_Unix,
        e_Full
    };

    // reset selection to none
    virtual void Reset(void);

    // choice state
    E_Choice Which(void) const;
    // throw exception if current selection is not as requested
    void CheckSelected(E_Choice index) const;
    // throw exception about wrong selection
    void ThrowInvalidSelection(E_Choice index) const;
    // return selection name (for diagnostic purposes)
    static std::string SelectionName(E_Choice index);

    // select requested variant if needed
    void Select(E_Choice index, ncbi::EResetVariant reset = ncbi::eDoResetVariant);

    // variants' types
    typedef int TUnix;
    typedef CFull_Time TFull;

    // variants' getters
    // variants' setters
    bool IsUnix(void) const;
    const int& GetUnix(void) const;

    int& GetUnix(void);
    int& SetUnix(void);
    void SetUnix(const int& value);

    bool IsFull(void) const;
    const CFull_Time& GetFull(void) const;

    CFull_Time& GetFull(void);
    CFull_Time& SetFull(void);
    void SetFull(const CFull_Time& value);


private:
    // copy constructor and assignment operator
    CTime_Base(const CTime_Base& );
    CTime_Base& operator=(const CTime_Base& );

    // choice state
    E_Choice m_choice;
    // helper methods
    void DoSelect(E_Choice index);

    static const char* const sm_SelectionNames[];
    // variants' data
    union {
        TUnix m_Unix;
        NCBI_NS_NCBI::CSerialObject *m_object;
    };
};






///////////////////////////////////////////////////////////
///////////////////// inline methods //////////////////////
///////////////////////////////////////////////////////////
inline
CTime_Base::E_Choice CTime_Base::Which(void) const
{
    return m_choice;
}

inline
void CTime_Base::CheckSelected(E_Choice index) const
{
    if ( m_choice != index )
        ThrowInvalidSelection(index);
}

inline
void CTime_Base::Select(E_Choice index, NCBI_NS_NCBI::EResetVariant reset)
{
    if ( reset == NCBI_NS_NCBI::eDoResetVariant || m_choice != index ) {
        if ( m_choice != e_not_set )
            Reset();
        DoSelect(index);
    }
}

inline
bool CTime_Base::IsUnix(void) const
{
    return m_choice == e_Unix;
}

inline
const CTime_Base::TUnix& CTime_Base::GetUnix(void) const
{
    CheckSelected(e_Unix);
    return m_Unix;
}

inline
CTime_Base::TUnix& CTime_Base::GetUnix(void)
{
    CheckSelected(e_Unix);
    return m_Unix;
}

inline
CTime_Base::TUnix& CTime_Base::SetUnix(void)
{
    Select(e_Unix, NCBI_NS_NCBI::eDoNotResetVariant);
    return m_Unix;
}

inline
void CTime_Base::SetUnix(const TUnix& value)
{
    Select(e_Unix, NCBI_NS_NCBI::eDoNotResetVariant);
    m_Unix = value;
}

inline
bool CTime_Base::IsFull(void) const
{
    return m_choice == e_Full;
}

///////////////////////////////////////////////////////////
////////////////// end of inline methods //////////////////
///////////////////////////////////////////////////////////






#endif // TIME_BASE_HPP
