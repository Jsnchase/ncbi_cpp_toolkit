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
 *   'seqloc.asn'.
 */

#ifndef OBJECTS_SEQLOC_SEQ_LOC_MIX_HPP
#define OBJECTS_SEQLOC_SEQ_LOC_MIX_HPP

// generated includes
#include <objects/seqloc/Seq_loc_mix_.hpp>

#include <objects/seqloc/Na_strand.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

class CSeq_id;


class NCBI_SEQLOC_EXPORT CSeq_loc_mix : public CSeq_loc_mix_Base
{
    typedef CSeq_loc_mix_Base Tparent;
public:
    // constructor
    CSeq_loc_mix(void);
    // destructor
    ~CSeq_loc_mix(void);
    
    //
    // See related function in util/sequence.hpp:
    //
    //   TSeqPos GetLength(const CSeq_loc_mix&, CScope*)
    //

    // check start or stop end of location for e_Lim fuzz
    bool IsPartialStart(ESeqLocExtremes ext) const;
    bool IsPartialStop (ESeqLocExtremes ext) const;

    // set / remove e_Lim fuzz on start or stop end
    void SetPartialStart(bool val, ESeqLocExtremes ext);
    void SetPartialStop (bool val, ESeqLocExtremes ext);

    /// check if parts of the seq-loc are missing
    bool IsTruncatedStart(ESeqLocExtremes ext) const;
    bool IsTruncatedStop (ESeqLocExtremes ext) const;

    /// set / remove e_Lim fuzz on start or stop
    /// (tl/tr - indicating removed parts of the seq-loc)
    void SetTruncatedStart(bool val, ESeqLocExtremes ext);
    void SetTruncatedStop (bool val, ESeqLocExtremes ext);

    // Add a Seq-loc to the mix.
    // NB: This is just a structural change, no guarantees as to the biological
    // validity of the data are made.
    // See sequence::MergeLocations(..) for context aware function.
    void AddSeqLoc(const CSeq_loc& other);
    void AddSeqLoc(CSeq_loc& other);
    // convenience function; automatically optimizes down points
    void AddInterval(const CSeq_id& id, TSeqPos from, TSeqPos to,
                     ENa_strand strand = eNa_strand_unknown);
        
    ENa_strand GetStrand(void) const;
    bool IsReverseStrand(void) const;
    TSeqPos GetStart(ESeqLocExtremes ext) const;
    TSeqPos GetStop (ESeqLocExtremes ext) const;

    /// Set the strand for all of the location's ranges.
    void SetStrand(ENa_strand strand);

    /// Reset the strand to 'not set'
    void ResetStrand(void);

    /// Flip the strand (e.g. plus to minus)
    void FlipStrand(void);

private:
    // Prohibit copy constructor & assignment operator
    CSeq_loc_mix(const CSeq_loc_mix&);
    CSeq_loc_mix& operator= (const CSeq_loc_mix&);

};



/////////////////// CSeq_loc_mix inline methods

inline
bool CSeq_loc_mix::IsReverseStrand(void) const
{
    return IsReverse(GetStrand());
}

/////////////////// end of CSeq_loc_mix inline methods


END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

#endif // OBJECTS_SEQLOC_SEQ_LOC_MIX_HPP
