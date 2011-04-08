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
 *   'biblio.asn'.
 */

// standard includes

// generated includes
#include <ncbi_pch.hpp>
#include <objects/biblio/Title.hpp>

#include <corelib/ncbiexpt.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

// destructor
CTitle::~CTitle(void)
{
}

const string& CTitle::GetTitle(C_E::E_Choice type) const
{
    if (Get().empty()) {
        NCBI_THROW(CException, eUnknown, "Title not set");
    } else if (type == C_E::e_not_set) {
        return GetTitle(*Get().front());
    } else {
        ITERATE (Tdata, it, Get()) {
            if ((*it)->Which() == type) {
                return GetTitle(**it);
            }
        }
        NCBI_THROW(CException, eUnknown,
                   "No title of requested type " + C_E::SelectionName(type));
    }
}


const string& CTitle::GetTitle(const C_E& t)
{
    switch (t.Which()) {
    case C_E::e_Name:     return t.GetName();
    case C_E::e_Tsub:     return t.GetTsub();
    case C_E::e_Trans:    return t.GetTrans();
    case C_E::e_Jta:      return t.GetJta();
    case C_E::e_Iso_jta:  return t.GetIso_jta();
    case C_E::e_Ml_jta:   return t.GetMl_jta();
    case C_E::e_Coden:    return t.GetCoden();
    case C_E::e_Issn:     return t.GetIssn();
    case C_E::e_Abr:      return t.GetAbr();
    case C_E::e_Isbn:     return t.GetIsbn();
    default:
        NCBI_THROW(CException, eUnknown,
                   "Unsupported title type" + C_E::SelectionName(t.Which()));
    }
}

END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

/* Original file checksum: lines: 61, chars: 1874, CRC32: f44f543e */
