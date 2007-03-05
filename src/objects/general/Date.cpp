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
 * Author:  Aaron Ucko, NCBI
 *
 * File Description:
 *   Useful member functions for dates: comparison and formatting
 *
 * Remark:
 *   This code was originally generated by application DATATOOL
 *   using specifications from the ASN data definition file
 *   'general.asn'.
 */

// standard includes

// generated includes
#include <ncbi_pch.hpp>
#include <objects/general/Date.hpp>

#include <objects/general/Date_std.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

// destructor
CDate::~CDate(void)
{
}


void CDate::SetToTime(const CTime& time, CDate::EPrecision prec)
{
    switch (Which()) {
    case e_not_set:
    case e_Std:
        SetStd().SetToTime(time, prec);
        break;
    case e_Str:
        SetStr(time.AsString());
        break;
    }
}

CTime CDate::AsCTime(CTime::ETimeZone tz) const
{
    switch (Which()) {
    case e_Std:
        return GetStd().AsCTime(tz);
    case e_Str:
        return CTime(GetStr()); // attempt to parse
    default:
        return CTime(CTime::eEmpty);
    }
}


CDate::ECompare CDate::Compare(const CDate& date) const
{
    if (IsStd()  &&  date.IsStd() ) {
        return GetStd().Compare(date.GetStd());
    } else if (IsStr()  &&  date.IsStr()  &&  GetStr() == date.GetStr()) {
        return eCompare_same;
    } else {
        return eCompare_unknown;
    }
}

void CDate::GetDate(string* label, const string& format) const
{
    // Ensure that date exists
    if (!label) {
        return;
    }
    
    switch (Which()) {
    case e_Str:
        *label += GetStr();
        return;
    case e_Std:
        GetStd().GetDate(label, format);
        return;
    default:
        return;
    }
}
        


END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

/* Original file checksum: lines: 61, chars: 1873, CRC32: d1b93689 */
