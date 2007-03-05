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

#ifndef OBJECTS_SEQFEAT_SEQ_FEAT_HPP
#define OBJECTS_SEQFEAT_SEQ_FEAT_HPP


// generated includes
#include <objects/seqfeat/Seq_feat_.hpp>
#include <objects/seqfeat/Gene_ref.hpp>
#include <objects/seqfeat/Prot_ref.hpp>
#include <objects/seqfeat/SeqFeatData.hpp>
#include <objects/seqloc/Seq_loc.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE /// namespace ncbi::objects::

class NCBI_SEQFEAT_EXPORT CSeq_feat : public CSeq_feat_Base
{
    typedef CSeq_feat_Base Tparent;
public:
    /// constructor
    CSeq_feat(void);
    /// destructor
    ~CSeq_feat(void);
    
    //
    /// See related function in util/feature.hpp
    //
    ///   void GetLabel (const CSeq_feat&, string*, ELabelType, CScope*)
    //

    /// get gene (if present) from Seq-feat.xref list
    const CGene_ref* GetGeneXref(void) const;
    void SetGeneXref(CGene_ref& value);
    CGene_ref& SetGeneXref(void);

    /// get protein (if present) from Seq-feat.xref list
    const CProt_ref* GetProtXref(void) const;
    void SetProtXref(CProt_ref& value);
    CProt_ref& SetProtXref(void);

    /// Add a qualifier to this feature
    void AddQualifier(const string& qual_name, const string& qual_val);

    /// add a DB xref to this feature
    void AddDbxref(const string& db_name, const string& db_key);
    void AddDbxref(const string& db_name, int db_key);

    /// Return a specified DB xref.  This will find the *first* item in the
    /// given referenced database.  If no item is found, an empty CConstRef<>
    /// is returned.
    CConstRef<CDbtag> GetNamedDbxref(const string& db) const;

    /// Return a named qualifier.  This will return the first item matching the
    /// qualifier name.  If no such qualifier is found, an empty string is
    /// returned.
    const string& GetNamedQual(const string& qual_name) const;

    /// Optional locations are used for features with locations
    /// re-mapped to a master sequence
    int CompareNonLocation(const CSeq_feat& f2,
                           const CSeq_loc& loc1, const CSeq_loc& loc2) const;
    int GetTypeSortingOrder(void) const;
    static int GetTypeSortingOrder(CSeqFeatData::E_Choice type);

    int Compare(const CSeq_feat& f2) const;
    int Compare(const CSeq_feat& f2,
                const CSeq_loc& mapped1, const CSeq_loc& mapped2) const;

private:

    /// Prohibit copy constructor and assignment operator
    CSeq_feat(const CSeq_feat& value);
    CSeq_feat& operator=(const CSeq_feat& value);        
};


/////////////////// CSeq_feat inline methods

// constructor
inline
CSeq_feat::CSeq_feat(void)
{
}


// Corresponds to SortFeatItemListByPos from the C toolkit
inline
int CSeq_feat::Compare(const CSeq_feat& f2,
                       const CSeq_loc& loc1, const CSeq_loc& loc2) const
{
    int diff = loc1.Compare(loc2);
    return diff != 0? diff: CompareNonLocation(f2, loc1, loc2);
}


// Corresponds to SortFeatItemListByPos from the C toolkit
inline
int CSeq_feat::Compare(const CSeq_feat& f2) const
{
    return Compare(f2, GetLocation(), f2.GetLocation());
}


inline
int CSeq_feat::GetTypeSortingOrder(void) const
{
    return GetTypeSortingOrder(GetData().Which());
}


NCBI_SEQFEAT_EXPORT
inline
bool operator< (const CSeq_feat& f1, const CSeq_feat& f2)
{
    return f1.Compare(f2) < 0;
}

/////////////////// end of CSeq_feat inline methods


END_objects_SCOPE /// namespace ncbi::objects::

END_NCBI_SCOPE


#endif /// OBJECTS_SEQFEAT_SEQ_FEAT_HPP
/* Original file checksum: lines: 90, chars: 2388, CRC32: c285198b */
