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
 *   'seq.asn'.
 */

// standard includes
#include <ncbi_pch.hpp>
#include <serial/enumvalues.hpp>

// generated includes
#include <objects/general/Date.hpp>
#include <objects/general/Dbtag.hpp>
#include <objects/general/User_object.hpp>
#include <objects/pub/Pub_equiv.hpp>
#include <objects/seq/Seqdesc.hpp>
#include <objects/seq/MolInfo.hpp>
#include <objects/seqfeat/Org_ref.hpp>
#include <objects/seqfeat/BioSource.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

// destructor
CSeqdesc::~CSeqdesc(void)
{
}


static void s_GetContentLabel(const CSeqdesc& desc, string* const label)
{
    const CEnumeratedTypeValues* tv;
    switch (desc.Which()) {
    case CSeqdesc::e_Comment:
        (*label) += desc.GetComment();
        return;
    case CSeqdesc::e_Region:
        (*label) += desc.GetRegion();
        return;
    case CSeqdesc::e_Het:
        (*label) += desc.GetHet();
        return;
    case CSeqdesc::e_Title:
        (*label) += desc.GetTitle();
        return;
    case CSeqdesc::e_Name:
        (*label) += desc.GetName();
        return;
    case CSeqdesc::e_Create_date:
        desc.GetCreate_date().GetDate(label);
        return;
    case CSeqdesc::e_Update_date:
        desc.GetUpdate_date().GetDate(label);
        return;
    case CSeqdesc::e_Org:
        desc.GetOrg().GetLabel(label);
        return;
    case CSeqdesc::e_Pub:
        desc.GetPub().GetPub().GetLabel(label);
        return;
    case CSeqdesc::e_User:
        desc.GetUser().GetLabel(label);
        return;
    case CSeqdesc::e_Method:
        tv = GetTypeInfo_enum_EGIBB_method();
        (*label) += tv->FindName(desc.GetMethod(), true);
        return;
    case CSeqdesc::e_Mol_type:
        tv = GetTypeInfo_enum_EGIBB_mol();
        (*label) += tv->FindName(desc.GetMol_type(), true);
        return;
    case CSeqdesc::e_Modif:
        ITERATE(list<EGIBB_mod>, it, desc.GetModif()) {
            tv = GetTypeInfo_enum_EGIBB_mod();
            if (!label->empty()) {
                (*label) += string(",") + tv->FindName(*it, true);
            } else {
                (*label) += tv->FindName(*it, true);
            }
        }
        return;
    case CSeqdesc::e_Source:
        desc.GetSource().GetOrg().GetLabel(label);
        return;
    case CSeqdesc::e_Maploc:
        desc.GetMaploc().GetLabel(label);
        return;
    case CSeqdesc::e_Molinfo:
        desc.GetMolinfo().GetLabel(label);
        return;
    case CSeqdesc::e_Dbxref:
        desc.GetDbxref().GetLabel(label);
        return;
    default:
        return;
    }
}


void CSeqdesc::GetLabel(string* const label, ELabelType label_type) const
{
    // Return if label is null
    if (!label) {
        return;
    }

    // Get type name for choice of this CSeqdesc object
    string name = SelectionName(Which());

    // Append label based on label_type
    switch (label_type) {
    case eBoth:
        (*label) += name + ": ";
    case eContent:
        s_GetContentLabel(*this, label);
        return;
    case eType:
        (*label) += name;
        return;
    }
}

END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE


/*
* ===========================================================================
*
* $Log$
* Revision 6.4  2004/05/19 17:25:14  gorelenk
* Added include of PCH - ncbi_pch.hpp
*
* Revision 6.3  2003/03/11 15:53:25  kuznets
* iterate -> ITERATE
*
* Revision 6.2  2002/10/03 19:09:42  clausen
* Removed extra whitespace
*
* Revision 6.1  2002/10/03 16:50:56  clausen
* Added GetLabel()
*
*
* ===========================================================================
*/
/* Original file checksum: lines: 64, chars: 1873, CRC32: d5933a71 */
