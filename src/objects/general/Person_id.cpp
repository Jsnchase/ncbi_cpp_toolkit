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
 * Author:  .......
 *
 * File Description:
 *   .......
 *
 * Remark:
 *   This code was originally generated by application DATATOOL
 *   using specifications from the ASN data definition file
 *   'general.asn'.
 */

// standard includes
#include <ncbi_pch.hpp>
#include <algorithm>

// generated includes
#include <objects/general/Person_id.hpp>
#include <objects/general/Name_std.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

// destructor
CPerson_id::~CPerson_id(void)
{
}


void CPerson_id::GetLabel(string* label, ETypeLabel type) const
{
    if (!label) {
        return;
    }

    char sep = (type == eGenbank) ? ',' : ' ';    
    switch (Which()) {
    case e_Name:
        if (!GetName().GetLast().empty()) {
            *label += GetName().GetLast();
            if (GetName().IsSetInitials()) {
                *label += sep + GetName().GetInitials();
            }
            if (GetName().IsSetSuffix()) {
                *label += string(" ") + GetName().GetSuffix();
            }
        } else if (GetName().IsSetFull()) {
            *label += GetName().GetFull();
        }
        return;
    case e_Ml:
        *label += GetMl();
        break;
    case e_Str:
        *label += GetStr();
        break;
    default:
        *label += "Unsupported PersonID";
        return;
    }
    
    if (type == eEmbl) {
        // Replace comma with a space
        replace(label->begin(), label->end(), ',', ' ');
    }         
}


END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

/* Original file checksum: lines: 61, chars: 1888, CRC32: 542a93ef */
