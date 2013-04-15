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
 *   'seqfeat.asn'.
 */

// generated includes
#include <ncbi_pch.hpp>
#include <objects/seqfeat/Seq_feat.hpp>
#include <objects/seqfeat/RNA_ref.hpp>
#include <objects/seqloc/Seq_loc.hpp>
#include <objects/general/User_object.hpp>
#include <objects/general/User_field.hpp>
#include <objects/seq/seqport_util.hpp>
#include <vector>


BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::


// destructor
CSeq_feat::~CSeq_feat(void)
{
}


BEGIN_LOCAL_NAMESPACE;


static int s_TypeOrder[] = {
    3, // e_not_set = 0,
    0, // e_Gene,
    3, // e_Org,
    2, // e_Cdregion,
    3, // e_Prot,
    1, // e_Rna,
    3  // e_Pub, and the rest
};


static inline
bool s_IsOperon(const CSeqFeatData& fd)
{
    return fd.Which() == CSeqFeatData::e_Imp &&
        fd.GetSubtype() == CSeqFeatData::eSubtype_operon;
}


static inline 
int s_GetCdregionOrder(const CSeqFeatData& fd)
{
    CCdregion::EFrame frame = fd.GetCdregion().GetFrame();
    if ( frame == CCdregion::eFrame_not_set ) {
        frame = CCdregion::eFrame_one;
    }
    return frame;
}


END_LOCAL_NAMESPACE;


int CSeq_feat::GetTypeSortingOrder(CSeqFeatData::E_Choice type)
{
    return s_TypeOrder[min(size_t(type),
                           sizeof(s_TypeOrder)/sizeof(s_TypeOrder[0])-1)];
}


// Corresponds to SortFeatItemListByPos from the C toolkit
int CSeq_feat::CompareNonLocation(const CSeq_feat& f2,
                                  const CSeq_loc& loc1,
                                  const CSeq_loc& loc2) const
{
    const CSeqFeatData& data1 = GetData();
    const CSeqFeatData& data2 = f2.GetData();
    CSeqFeatData::E_Choice type1 = data1.Which();
    CSeqFeatData::E_Choice type2 = data2.Which();

    // operon first
    if ( int diff = s_IsOperon(data2) - s_IsOperon(data1) ) {
        return diff;
    }
    if ( type1 != type2 ) {
        // order by feature type
        int order1 = GetTypeSortingOrder(type1);
        int order2 = GetTypeSortingOrder(type2);
        int diff = order1 - order2;
        if ( diff != 0 )
            return diff;
    }

    // minus strand last
    ENa_strand strand1 = loc1.GetStrand();
    ENa_strand strand2 = loc2.GetStrand();
    if ( int diff = IsReverse(strand1) - IsReverse(strand2) ) {
        return diff;
    }

    if ( int diff = loc1.CompareSubLoc(loc2, strand1) ) {
        return diff;
    }

    {{ // compare subtypes
        CSeqFeatData::ESubtype subtype1 = data1.GetSubtype();
        CSeqFeatData::ESubtype subtype2 = data2.GetSubtype();
        int diff = subtype1 - subtype2;
        if ( diff != 0 )
            return diff;
    }}

    // subtypes are equal, types must be equal too
    _ASSERT(type1 == type2);

    // type dependent comparison
    if ( type1 == CSeqFeatData::e_Cdregion ) {
        // compare frames of identical CDS ranges
        if ( int diff = s_GetCdregionOrder(data1)-s_GetCdregionOrder(data2) ) {
            return diff;
        }
    }
    else if ( type1 == CSeqFeatData::e_Imp ) {
        // compare labels of imp features
        int diff = NStr::CompareNocase(data1.GetImp().GetKey(),
                                       data2.GetImp().GetKey());
        if ( diff != 0 )
            return diff;
    }

    // XXX - should compare parent seq-annots
    // XXX 1. parent Seq-annot idx.itemID
    // XXX 2. features itemID

    return 0; // unknown
}

const CGene_ref* CSeq_feat::GetGeneXref(void) const

{
    ITERATE(CSeq_feat::TXref, it, GetXref ()) {
        if ((*it)->IsSetData () && (*it)->GetData ().IsGene ()) {
            return &((*it)->GetData ().GetGene ());
        }
    }
    return 0;
}

void CSeq_feat::SetGeneXref(CGene_ref& value)

{
    TXref& xref = SetXref();
    NON_CONST_ITERATE(CSeq_feat::TXref, it, SetXref ()) {
        CSeqFeatXref& ref = **it;
        if (ref.IsSetData() && ref.GetData().IsGene()) {
            ref.SetData().SetGene(value);
            return;
        }
    }
    CRef<CSeqFeatXref> gref(new CSeqFeatXref);
    xref.push_back(gref);
    gref->SetData().SetGene(value);
}

CGene_ref& CSeq_feat::SetGeneXref(void)
{
    TXref& xref = SetXref();
    NON_CONST_ITERATE(CSeq_feat::TXref, it, xref) {
        CSeqFeatXref& ref = **it;
        if (ref.IsSetData() && ref.GetData().IsGene()) {
            return ref.SetData().SetGene();
        }
    }
    CRef<CSeqFeatXref> gref(new CSeqFeatXref);
    xref.push_back(gref);
    return gref->SetData().SetGene();
}

const CProt_ref* CSeq_feat::GetProtXref(void) const

{
    ITERATE(CSeq_feat::TXref, it, GetXref ()) {
        if ((*it)->IsSetData () && (*it)->GetData ().IsProt ()) {
            return &((*it)->GetData ().GetProt ());
        }
    }
    return 0;
}

void CSeq_feat::SetProtXref(CProt_ref& value)

{
    TXref& xref = SetXref();
    NON_CONST_ITERATE(CSeq_feat::TXref, it, xref) {
        CSeqFeatXref& ref = **it;
        if (ref.IsSetData() && ref.GetData().IsProt()) {
            ref.SetData().SetProt(value);
            return;
        }
    }
    CRef<CSeqFeatXref> pref(new CSeqFeatXref);
    xref.push_back(pref);
    pref->SetData().SetProt(value);
}

CProt_ref& CSeq_feat::SetProtXref(void)

{
    TXref& xref = SetXref();
    NON_CONST_ITERATE(CSeq_feat::TXref, it, xref) {
        CSeqFeatXref& ref = **it;
        if (ref.IsSetData() && ref.GetData().IsProt()) {
            return ref.SetData().SetProt();
        }
    }
    CRef<CSeqFeatXref> pref(new CSeqFeatXref);
    xref.push_back(pref);
    return pref->SetData().SetProt();
}


void CSeq_feat::AddQualifier(const string& qual_name, const string& qual_val)
{
    CRef<CGb_qual> qual(new CGb_qual());
    qual->SetQual(qual_name);
    qual->SetVal(qual_val);
    SetQual().push_back(qual);
}

void CSeq_feat::AddOrReplaceQualifier(
    const string& qual_name, const string& qual_val)
{
    if (IsSetQual()) {
        NON_CONST_ITERATE (TQual, iter, SetQual()) {
            if ( (*iter)->GetQual() == qual_name ) {
                (*iter)->SetVal(qual_val);
                return;
            }
        }
    }

    // we didn't find an already-existing qual, so we add it
    AddQualifier(qual_name, qual_val);
}

void CSeq_feat::RemoveQualifier(const string& qual_name)
{
    // qual is a vector so we have to be careful; carelessly
    // removing all quals that match as we find them
    // is a potentially quadratic-time operation.
    // Instead we construct a new qual vector and do a swap (swap should
    // be a constant-time operation)

    TQual new_qual_vec;
    new_qual_vec.reserve(GetQual().size());

    ITERATE (TQual, iter, GetQual()) {
        if ( (*iter)->GetQual() != qual_name ) {
            new_qual_vec.push_back(*iter);
        }
    }

    if( new_qual_vec.size() != GetQual().size() ) {
        // swap should be a constant-time operation
        SetQual().swap(new_qual_vec);
    }
}

void CSeq_feat::AddDbxref(const string& db, const string& tag)
{
    CRef<CDbtag> dbtag(new CDbtag());
    dbtag->SetDb(db);
    dbtag->SetTag().SetStr(tag);
    SetDbxref().push_back(dbtag);
}


void CSeq_feat::AddDbxref(const string& db, int tag)
{
    CRef<CDbtag> dbtag(new CDbtag());
    dbtag->SetDb(db);
    dbtag->SetTag().SetId(tag);
    SetDbxref().push_back(dbtag);
}


CConstRef<CDbtag> CSeq_feat::GetNamedDbxref(const string& db) const
{
    if (IsSetDbxref()) {
        ITERATE (TDbxref, iter, GetDbxref()) {
            if ( (*iter)->GetDb() == db) {
                return *iter;
            }
        }
    }

    return CConstRef<CDbtag>();
}


const string& CSeq_feat::GetNamedQual(const string& qual_name) const
{
    if (IsSetQual()) {
        ITERATE (TQual, iter, GetQual()) {
            if ( (*iter)->GetQual() == qual_name  &&  (*iter)->IsSetVal()) {
                return (*iter)->GetVal();
            }
        }
    }

    return kEmptyStr;
}

AutoPtr<CSeq_feat::TExceptionTextSet> 
CSeq_feat::GetTempExceptionTextSet(void) const
{
    AutoPtr<TExceptionTextSet> pAnswerSet( new TExceptionTextSet );

    if( ! IsSetExcept() || ! GetExcept() || ! IsSetExcept_text()  ) 
    {
        return pAnswerSet; // empty set
    }

    const string & raw_exception_texts = GetExcept_text();

    vector<CTempStringEx> exception_parts;
    NStr::Tokenize (raw_exception_texts, ",", exception_parts, 
        NStr::eMergeDelims);

    ITERATE( vector<CTempStringEx>, part_it, exception_parts ) {
        pAnswerSet->insert( NStr::TruncateSpaces(*part_it) );
    }

    return pAnswerSet;
}

bool CSeq_feat::HasExceptionText(const string & exception_text ) const
{
    CTempString sCleanedInputText = NStr::TruncateSpaces(exception_text);
    if( sCleanedInputText.empty() ) {
        // it's preferable for the caller not to give us an
        // empty string, but... just in case
        return false;
    }

    AutoPtr<TExceptionTextSet> p_exception_text_set = GetTempExceptionTextSet();

    // is it in the set?
    return ( p_exception_text_set->find(sCleanedInputText) != 
        p_exception_text_set->end() );
}

ISeq_feat::~ISeq_feat(void)
{
}

END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

/* Original file checksum: lines: 61, chars: 1885, CRC32: 417ca6e0 */
