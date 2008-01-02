#ifndef ALGO_GNOMON___GNOMON_MODEL__HPP
#define ALGO_GNOMON___GNOMON_MODEL__HPP

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
 * Authors:  Alexandre Souvorov
 *
 * File Description:
 *
 */

#include <corelib/ncbistd.hpp>
#include <corelib/ncbi_limits.hpp>
#include "set.hpp"

#include <set>
#include <vector>
#include <algorithm>
#include <math.h>

#include <objmgr/seq_vector_ci.hpp> // CSeqVectorTypes::TResidue
#include <util/range.hpp>           // TSignedSeqRange

BEGIN_NCBI_SCOPE
BEGIN_SCOPE(objects)
    class CSeq_align;
END_SCOPE(objects)
BEGIN_SCOPE(gnomon)

class CGnomonEngine;

// Making this a constant declaration (kBadScore) would be preferable,
// but backfires on WorkShop, where it is implicitly static and hence
// unavailable for use in inline functions.
inline
double BadScore() { return -numeric_limits<double>::max(); }

enum EStrand { ePlus, eMinus };

typedef objects::CSeqVectorTypes::TResidue TResidue; // unsigned char

typedef vector<TResidue> CResidueVec;

typedef vector<int> TIVec;

inline bool Precede(TSignedSeqRange l, TSignedSeqRange r) { return l.GetTo() < r.GetFrom(); }
inline bool Include(TSignedSeqRange big, TSignedSeqRange small) { return (big.GetFrom()<=small.GetFrom() && small.GetTo()<=big.GetTo()); }
inline bool Include(TSignedSeqRange r, TSignedSeqPos p) { return (r.GetFrom()<=p && p<=r.GetTo()); }
inline bool Enclosed(TSignedSeqRange big, TSignedSeqRange small) { return (big != small && Include(big, small)); }

typedef CConstRef<objects::CSeq_id> TConstSeqidRef;

class CSupportInfo
{
public:
    CSupportInfo(const TConstSeqidRef& s, bool core);

    TConstSeqidRef Seqid() const;
    void SetSeqid(const TConstSeqidRef sid);
    void SetCore(bool core) { m_core_align = core; }
    bool CoreAlignment() const { return m_core_align; }
    void SetType(int t) { m_type = t; }
    int Type() const { return m_type; }
    bool operator==(const CSupportInfo& s) const;
    bool operator<(const CSupportInfo& s) const;

private:
    int m_local_id;
    TConstSeqidRef m_seq_id;
    bool m_core_align;
    int m_type;
}; 

struct compare_seqid_refs : public binary_function<TConstSeqidRef,TConstSeqidRef,bool>
{
  bool operator()(const TConstSeqidRef& s1, const TConstSeqidRef& s2) const
  {
    return *s1 < *s2;
  }
};

typedef CVectorSet<CSupportInfo> CSupportInfoSet;


NCBI_XALGOGNOMON_EXPORT CRef<objects::CSeq_id> CreateSeqid(const string& name,int score_func(const CRef<objects::CSeq_id>&)=objects::CSeq_id::Score); // creates local id if not an accession
NCBI_XALGOGNOMON_EXPORT CRef<objects::CSeq_id> CreateSeqid(int local_id);

class NCBI_XALGOGNOMON_EXPORT CFrameShiftInfo
{
public:
    CFrameShiftInfo(TSignedSeqPos l = 0, int len = 0, bool is_i = true, const string& d_v = kEmptyStr) :
        m_loc(l), m_len(len), m_is_insert(is_i),
        m_delet_value((!is_i && d_v == kEmptyStr)?string(len,'N'):d_v), m_old_loc(-1)
    {}
    TSignedSeqPos Loc() const { return m_loc; }
    int Len() const { return m_len; }
    bool IsInsertion() const { return m_is_insert; }
    bool IsDeletion() const { return !m_is_insert; }
    const string& DeletedValue() const { return m_delet_value; }
    bool IntersectingWith(TSignedSeqPos a, TSignedSeqPos b) const
    {
        return IsDeletion() && Loc() >= a && Loc() <= b+1 ||
            IsInsertion() && Loc() <= b && a <= Loc()+Len()-1;
    }
    bool operator<(const CFrameShiftInfo& fsi) const
    { return ((m_loc == fsi.m_loc && IsDeletion() != fsi.IsDeletion()) ? IsDeletion() : m_loc < fsi.m_loc); } // if location is same deletion first
    bool operator==(const CFrameShiftInfo& fsi) const 
    { return (m_loc == fsi.m_loc) && (m_len == fsi.m_len) && (m_is_insert == fsi.m_is_insert) && (m_delet_value == fsi.m_delet_value); }
    bool operator!=(const CFrameShiftInfo& fsi) const
    { return !(*this == fsi); }
    void ReplaceWithInsertion(TSignedSeqPos l, int len) // should be deletion, and old len + new len == 3
    { _ASSERT( IsDeletion() && m_old_loc == -1 && m_len+len==3 ); m_old_loc = m_loc; m_loc = l; m_len = len; m_is_insert = true; }
    void RestoreIfReplaced() { if (IsInsertion() && m_old_loc != -1) { m_len = 3 - m_len; m_is_insert = false; swap (m_old_loc, m_loc); } }
    TSignedSeqPos ReplacementLoc() const { return m_old_loc; }
private:
    TSignedSeqPos m_loc;  // left location for insertion, deletion is before m_loc
                          // insertion - when there are extra bases in the genome
    int m_len;
    bool m_is_insert;
    string m_delet_value;
    TSignedSeqPos m_old_loc; // for replaced deletion
};

typedef vector<CFrameShiftInfo> TFrameShifts;

template <class Res>
bool IsStartCodon(const Res * seq, int strand = ePlus);   // seq points to the first base in biological order
template <class Res>
bool IsStopCodon(const Res * seq, int strand = ePlus);   // seq points to the first base in biological order

class CFrameShiftedSeqMap;

template <class Vec>
bool IsProperStart(size_t pos, const Vec& mrna, const CFrameShiftedSeqMap& mrnamap);
template <class Vec>
bool IsProperStop(size_t pos, const Vec& mrna, const CFrameShiftedSeqMap& mrnamap);

class CRangeMapper {
public:
    virtual ~CRangeMapper() {}
    virtual TSignedSeqRange operator()(TSignedSeqRange r, bool withextras = true) const = 0;
};

class NCBI_XALGOGNOMON_EXPORT CAlignExon {
public:
    CAlignExon(TSignedSeqPos f = 0, TSignedSeqPos s = 0, bool fs = false, bool ss = false) : m_fsplice(fs), m_ssplice(ss), m_range(f,s) {};

    bool operator==(const CAlignExon& p) const 
    { 
        return (m_range==p.m_range && m_fsplice == p.m_fsplice && m_ssplice == p.m_ssplice); 
    }
    bool operator!=(const CAlignExon& p) const
    {
        return !(*this == p);
    }
    bool operator<(const CAlignExon& p) const { return Precede(Limits(),p.Limits()); }
    
    operator TSignedSeqRange() const { return m_range; }
    const TSignedSeqRange& Limits() const { return m_range; }
          TSignedSeqRange& Limits()       { return m_range; }
    TSignedSeqPos GetFrom() const { return m_range.GetFrom(); }
    TSignedSeqPos GetTo() const { return m_range.GetTo(); }
    void Extend(const CAlignExon& e);
    void AddFrom(int d) { m_range.SetFrom( m_range.GetFrom() +d ); }
    void AddTo(int d) { m_range.SetTo( m_range.GetTo() +d ); }

    bool m_fsplice, m_ssplice;

    void Remap(const CRangeMapper& mapper) { m_range = mapper(m_range); }
private:
    TSignedSeqRange m_range;
};

class CCDSInfo {
public:
    CCDSInfo(): m_confirmed_start(false),m_open(false),
                m_score(BadScore()) {}

    bool operator== (const CCDSInfo& another) const;

    TSignedSeqRange ReadingFrame() const { return m_reading_frame; }
    TSignedSeqRange ProtReadingFrame() const { return m_reading_frame_from_proteins; }
    TSignedSeqRange Cds() const { return Start()+ReadingFrame()+Stop(); }
    TSignedSeqRange MaxCdsLimits() const { return m_max_cds_limits; }

    TSignedSeqRange Start() const {return m_start; }
    TSignedSeqRange Stop() const {return m_stop; }
    bool HasStart() const { return Start().NotEmpty(); }
    bool HasStop () const { return Stop().NotEmpty();  }
    bool ConfirmedStart() const { return m_confirmed_start; }  // start is confirmed by protein alignment
    typedef vector<TSignedSeqRange> TPStops;
    const TPStops& PStops() const { return m_p_stops; }
    bool PStop() const { return !m_p_stops.empty(); }  // has premature stop(s)
    
    bool OpenCds() const { return m_open; }  // "optimal" CDS is not internal
    double Score() const { return m_score; }

    void SetReadingFrame(TSignedSeqRange r, bool protein = false);
    void SetStart(TSignedSeqRange r, bool confirmed = false);
    void SetStop(TSignedSeqRange r);
    void Set5PrimeCdsLimit(TSignedSeqPos p);
    void Clear5PrimeCdsLimit();
    void AddPStop(TSignedSeqRange r);
    void SetScore(double score, bool open=false);

    void CombineWith(const CCDSInfo& another_cds_info);
    void Remap(const CRangeMapper& mapper);
    void Clip(TSignedSeqRange limits);
    void Clear();
    void ClearPStops() { m_p_stops.clear(); }

    int Strand() const; // -1 (minus), 0 (unknown), 1 (plus)

    bool Invariant() const
    {
        if (ReadingFrame().Empty()) {
            _ASSERT( !HasStop() && !HasStart() );
            _ASSERT( ProtReadingFrame().Empty() && MaxCdsLimits().Empty() );
            _ASSERT( !ConfirmedStart() );
            _ASSERT( !PStop() );
            _ASSERT( !OpenCds() );
            _ASSERT( Score()==BadScore() );
            return true;
        }

        _ASSERT( !Start().IntersectingWith(ReadingFrame()) );
        _ASSERT( !Stop().IntersectingWith(ReadingFrame()) );
        _ASSERT( ProtReadingFrame().Empty() || Include(ReadingFrame(), ProtReadingFrame()) );
        _ASSERT( Include( MaxCdsLimits(), Cds() ) );

        if (!HasStop() && !HasStart()) {
            _ASSERT( MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom() ||
                     MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo() );
        } else if (HasStart() && !HasStop()) {
            if (Precede(Start(), ReadingFrame())) {
                _ASSERT( MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo() );
            } else {
                _ASSERT( MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom() );
            }
        }
        else if (HasStart() && HasStop()) {
            _ASSERT( Include(Start()+Stop(),ReadingFrame()) );
        }
        if (HasStop()) {
            if (Precede(ReadingFrame(),Stop())) {
                _ASSERT( MaxCdsLimits().GetTo()==Stop().GetTo() );
            } else {
                _ASSERT( MaxCdsLimits().GetFrom()==Stop().GetFrom() );
            } 
        }

        if (ConfirmedStart()) {
            _ASSERT( HasStart() );
            if (Precede(Start(), ReadingFrame())) {
                 _ASSERT( ProtReadingFrame().GetFrom()==ReadingFrame().GetFrom() );
            } else {
                 _ASSERT( ProtReadingFrame().GetTo()==ReadingFrame().GetTo() );
            }
        }

        ITERATE( vector<TSignedSeqRange>, s, PStops())
            _ASSERT( Include(ReadingFrame(), *s) );

        if (OpenCds()) {
            if (HasStart()) {
                if (Precede(Start(), ReadingFrame())) {
                    _ASSERT( MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom() );
                } else {
                    _ASSERT( MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo() );
                }
            } else if (HasStop()) {
                if (Precede(ReadingFrame(), Stop())) {
                    _ASSERT( MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom() );
                } else {
                    _ASSERT( MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo() );
                }
            } else {
                _ASSERT( MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom() );
                _ASSERT( MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo() );
            }
        }

        return true;
    }

private:
    TSignedSeqRange m_start, m_stop;
    TSignedSeqRange m_reading_frame;
    TSignedSeqRange m_reading_frame_from_proteins;

    TSignedSeqRange m_max_cds_limits;

    bool m_confirmed_start;
    TPStops m_p_stops;

    bool m_open;
    double m_score;
};

class NCBI_XALGOGNOMON_EXPORT CGeneModel
{
public:
    enum EType {
        eWall = 1,
        eNested = 2,
        eEST = 4,
        emRNA = 8,
        eProt = 16,
        eChain = 32,
        eGnomon = 64
    };
    static string TypeToString(int type);

    enum EStatus {
        eSkipped = 4,
        eLeftTrimmed = 8,
        eRightTrimmed = 16,
        eFullSupCDS = 32,
        ePseudo = 64
    };

    CGeneModel(EStrand s = ePlus, int id = 0, int type = 0) :
        m_type(type), m_id(id), m_status(0), m_expecting_hole(false), m_strand(s), m_geneid(0) {}
    CGeneModel(const objects::CSeq_align& seq_align);
    virtual ~CGeneModel() {}

    void AddExon(TSignedSeqRange exon);
    void AddHole(); // between model and next exons

    typedef vector<CAlignExon> TExons;
    const TExons& Exons() const { return m_exons; }

    void Remap(const CRangeMapper& mapper);
    enum EClipMode { eRemoveExons, eDontRemoveExons };
    void Clip(TSignedSeqRange limits, EClipMode mode, bool ensure_cds_invariant = true);
    void CutExons(TSignedSeqRange hole); // clip or remove exons, dangerous, should be completely in or outside the cds, should not cut an exon in two 
    void ExtendLeft(int amount);
    void ExtendRight(int amount);
    void Extend(const CGeneModel& a, bool ensure_cds_invariant = true);
    void TrimEdgesToFrameInOtherAlignGaps(const TExons& exons_with_gaps, bool ensure_cds_invariant = true);
   
    TSignedSeqRange Limits() const { return m_range; }
    int AlignLen() const ;
    void RecalculateLimits()
    {
        if (Exons().empty()) {
            m_range = TSignedSeqRange::GetEmpty(); 
        } else {
            m_range = TSignedSeqRange(Exons().front().GetFrom(),Exons().back().GetTo());
        }
    }

    // ReadingFrame doesn't include start/stop. It's always on codon boundaries
    TSignedSeqRange ReadingFrame() const { return m_cds_info.ReadingFrame(); }
    // CdsLimits include start/stop if any, goes to model limit if no start/stop
    TSignedSeqRange RealCdsLimits() const;
    int RealCdsLen() const ;              // %3!=0 is possible 
    // MaxCdsLimits - longest cds. include start/stop if any,
    // goes to 5' limit if no upstream stop, goes to 3' limit if no stop
    TSignedSeqRange MaxCdsLimits() const;

    const CCDSInfo& GetCdsInfo() const { return m_cds_info; }
    void SetCdsInfo(const CCDSInfo& cds_info);
    void SetCdsInfo(const CGeneModel& a);
    void CombineCdsInfo(const CGeneModel& a, bool ensure_cds_invariant = true);
    void CombineCdsInfo(const CCDSInfo& cds_info, bool ensure_cds_invariant = true);

    bool IntersectingWith(const CGeneModel& a) const 
    {
        return Limits().IntersectingWith(a.Limits()); 
    }
    void SetStrand(EStrand s) { m_strand = s; }
    EStrand Strand() const { return m_strand; }
    void SetType(int t) { m_type = t; }
    int Type() const { return m_type; }
    int GeneID() const { return m_geneid; }
    void SetGeneID(int id) { m_geneid = id; }
    int ID() const { return m_id; }
    void SetID(int id) { m_id = id; }
    TConstSeqidRef Seqid() const;
    const CSupportInfoSet& Support() const { return m_support; }
          CSupportInfoSet& Support()       { return m_support; }
    const string& ProteinHit() const { return  m_protein_hit; }
          string& ProteinHit()       { return  m_protein_hit; }

    unsigned int& Status() { return m_status; }
    const unsigned int& Status() const { return m_status; }
    void ClearStatus() { m_status = 0; }

    const string& GetComment() const { return m_comment; }
    void SetComment(const string& comment) { m_comment = comment; }

    bool operator<(const CGeneModel& a) const { return Precede(Limits(),a.Limits()); }

    double Score() const { return m_cds_info.Score(); }

    bool Continuous() const           //  no "holes" in alignment
    {
        for(unsigned int i = 1; i < Exons().size(); ++i)
            if (!Exons()[i-1].m_ssplice || !Exons()[i].m_fsplice)
                return false;
        return true;
    }
    bool HasStart() const { return m_cds_info.HasStart(); }
    bool HasStop () const { return m_cds_info.HasStop ();  }
    bool LeftComplete()  const { return Strand() == ePlus ? HasStart() : HasStop();  }
    bool RightComplete() const { return Strand() == ePlus ? HasStop()  : HasStart(); }
    bool FullCds() const { return HasStart() && HasStop() && Continuous(); }
    bool CompleteCds() const { return FullCds() && (!Open5primeEnd() || ConfirmedStart()); }

    bool GoodEnoughToBeAnnotation(int minCdsLen) const
    {
        return !OpenCds() && FullCds() && RealCdsLen() >= minCdsLen;
    }
    bool GoodEnoughToBeAlternative(int minCdsLen, int maxcomposite) const;

    bool Open5primeEnd() const
    {
        return (Strand() == ePlus ? OpenLeftEnd() : OpenRightEnd());
    }
    bool OpenLeftEnd() const
    {
        return ReadingFrame().NotEmpty() && GetCdsInfo().MaxCdsLimits().GetFrom()==TSignedSeqRange::GetWholeFrom();
    }
    bool OpenRightEnd() const
    {
        return ReadingFrame().NotEmpty() && GetCdsInfo().MaxCdsLimits().GetTo()==TSignedSeqRange::GetWholeTo();
    }

    bool OpenCds() const { return m_cds_info.OpenCds(); }  // "optimal" CDS is not internal
    bool PStop() const { return m_cds_info.PStop(); }  // has premature stop(s)
    
    bool ConfirmedStart() const { return m_cds_info.ConfirmedStart(); }  // start is confirmed by protein alignment

    bool isNMD() const
    {
        if (ReadingFrame().Empty() || Exons().size() <= 1)
            return false;
        if (Strand() == ePlus) {
            return RealCdsLimits().GetTo() < Exons().back().GetFrom();
        } else {
            return RealCdsLimits().GetFrom() > Exons().front().GetTo();
        }
    }

    TFrameShifts& FrameShifts() { return m_fshifts; }
    const TFrameShifts& FrameShifts() const { return m_fshifts; }
    TFrameShifts FrameShifts(TSignedSeqPos a, TSignedSeqPos b) const;
    void RemoveExtraFShifts();

    int FShiftedLen(TSignedSeqPos a, TSignedSeqPos, bool withextras = true) const;
    int FShiftedLen(TSignedSeqRange ab, bool withextras = true) const;

    // move along mrna skipping introns
    TSignedSeqPos FShiftedMove(TSignedSeqPos pos, int len) const;
    
    /*
    template <class Vec>
    void GetSequence(const Vec& seq, Vec& mrna, TIVec* mrnamap = 0, bool cdsonly = false) const;                     
    */

    string GetProtein (const CResidueVec& contig_sequence) const;
    
    // Below comparisons ignore CDS completely, first 3 assume that alignments are the same strand
    
    int isCompatible(const CGeneModel& a) const;  // returns 0 for notcompatible or (number of common splices)+1
    bool IsSubAlignOf(const CGeneModel& a) const { return Include(a.Limits(),Limits()) && isCompatible(a); }
    int MutualExtension(const CGeneModel& a) const;  // returns 0 for notcompatible or (number of introns) + 1
    
    bool IdenticalAlign(const CGeneModel& a) const
    { return Strand()==a.Strand() && Limits()==a.Limits() && Exons() == a.Exons() && FrameShifts()==a.FrameShifts() && 
            GetCdsInfo().PStops() == a.GetCdsInfo().PStops() && Type() == a.Type(); }
    bool operator==(const CGeneModel& a) const
    {
        return IdenticalAlign(a) && Type()==a.Type() && m_id==a.m_id && m_support==a.m_support;
    }

#ifdef _DEBUG
    int oid;
#endif

private:
    int m_type;
    int m_id;
    unsigned int m_status;

    TExons m_exons;
    TExons& MyExons() { return m_exons; }
    bool m_expecting_hole;

    TSignedSeqRange m_range;
    EStrand m_strand;
    TFrameShifts m_fshifts;

    CCDSInfo m_cds_info;
    bool CdsInvariant(bool check_start_stop = true) const;

    int m_geneid;
    CSupportInfoSet m_support;
    string m_protein_hit;
    string m_comment;
};


class CFrameShiftedSeqMap {
public:
    CFrameShiftedSeqMap(TSignedSeqPos orig_a, TSignedSeqPos orig_b) : m_strand(ePlus) {
        m_orig_ranges.push_back(SMapRange(SMapRangeEdge(orig_a), SMapRangeEdge(orig_b)));
        m_edited_ranges = m_orig_ranges;
}
    CFrameShiftedSeqMap(TSignedSeqPos orig_a, TSignedSeqPos orig_b, TFrameShifts::const_iterator fsi_begin, const TFrameShifts::const_iterator fsi_end) : m_strand(ePlus) { InsertIndelRangesForInterval(orig_a, orig_b, 0, fsi_begin, fsi_end); }
    enum EMapLimit{ eNoLimit, eCdsOnly, eRealCdsOnly };
    CFrameShiftedSeqMap(const CGeneModel& align, EMapLimit limit = eNoLimit);
    TSignedSeqPos MapOrigToEdited(TSignedSeqPos orig_pos) const;
    TSignedSeqPos MapEditedToOrig(TSignedSeqPos edited_pos) const;
    TSignedSeqRange MapRangeOrigToEdited(TSignedSeqRange orig_range, bool withextras = true) const;
    TSignedSeqRange MapRangeEditedTodOrig(TSignedSeqRange edited_range, bool withextras = true) const;
    template <class Vec>
    void EditedSequence(const Vec& original_sequence, Vec& edited_sequence) const;
    bool EditedRangeIncludesTransformedDeletion(TSignedSeqRange edited_range) const;
    int FShiftedLen(TSignedSeqRange ab, bool withextras = true) const { return MapRangeOrigToEdited(ab, withextras).GetLength(); }
    int FShiftedLen(TSignedSeqPos a, TSignedSeqPos b, bool withextras = true) const { return FShiftedLen(TSignedSeqRange(a,b), withextras); }
    TSignedSeqRange ShrinkToRealPoints(TSignedSeqRange orig_range) const;
    TSignedSeqPos FShiftedMove(TSignedSeqPos orig_pos, int len) const;
// private: // breaks SMapRange on WorkShop. :-/
    struct SMapRangeEdge {
        SMapRangeEdge(TSignedSeqPos p, TSignedSeqPos e = 0, bool d = false) : m_pos(p), m_extra(e), m_was_deletion(d) {}
        bool operator<(const SMapRangeEdge& mre) const { return m_pos < mre.m_pos; }
        bool operator==(const SMapRangeEdge& mre) const { return m_pos == mre.m_pos; }

        TSignedSeqPos m_pos, m_extra;
        bool m_was_deletion;
    };

private:
    class SMapRange {
    public:
        SMapRange(SMapRangeEdge from, SMapRangeEdge to) : m_from(from), m_to(to) {}
        TSignedSeqPos GetFrom() const { return m_from.m_pos; }
        TSignedSeqPos GetTo() const { return m_to.m_pos; }
        TSignedSeqPos GetExtendedFrom() const { return m_from.m_pos - m_from.m_extra; }
        TSignedSeqPos GetExtendedTo() const { return m_to.m_pos+m_to.m_extra; }
        TSignedSeqPos GetExtraFrom() const { return m_from.m_extra; }
        TSignedSeqPos GetExtraTo() const { return m_to.m_extra; }
        bool LeftEndIsTransformedDeletion() const { return m_from.m_was_deletion; }
        bool RightEndIsTransformedDeletion() const { return m_to.m_was_deletion; }
        bool operator<(const SMapRange& mr) const {
            if(m_from.m_pos == mr.m_from.m_pos) return m_to.m_pos < mr.m_to.m_pos;
            else return m_from.m_pos < mr.m_from.m_pos;
        }
    
    private:
        SMapRangeEdge m_from, m_to;
    };


    enum ERangeEnd{ eLeftEnd, eRightEnd, eSinglePoint };

    static TSignedSeqPos MapAtoB(const vector<CFrameShiftedSeqMap::SMapRange>& a, const vector<CFrameShiftedSeqMap::SMapRange>& b, TSignedSeqPos p, ERangeEnd move_mode);
    static TSignedSeqRange MapRangeAtoB(const vector<CFrameShiftedSeqMap::SMapRange>& a, const vector<CFrameShiftedSeqMap::SMapRange>& b, TSignedSeqRange r, bool withextras );
    static int FindLowerRange(const vector<CFrameShiftedSeqMap::SMapRange>& a,  TSignedSeqPos p);

    void InsertOneToOneRange(TSignedSeqPos orig_start, TSignedSeqPos edited_start, int len, const CFrameShiftInfo* left_fs = 0, const CFrameShiftInfo* right_fs = 0);
    TSignedSeqPos InsertIndelRangesForInterval(TSignedSeqPos orig_a, TSignedSeqPos orig_b, TSignedSeqPos edit_a, TFrameShifts::const_iterator fsi_begin, TFrameShifts::const_iterator fsi_end);

    vector<SMapRange> m_orig_ranges, m_edited_ranges;
    EStrand m_strand;
};

TSignedSeqRange MapRangeToOrig(TSignedSeqPos start, TSignedSeqPos stop, const CFrameShiftedSeqMap& mrnamap);


struct NCBI_XALGOGNOMON_EXPORT setcontig {
    const string& m_contig;
    explicit setcontig(const string& cntg) : m_contig(cntg) {}
};
struct NCBI_XALGOGNOMON_EXPORT getcontig {
    string& m_contig;
    explicit getcontig(string& cntg) : m_contig(cntg) {}
};
NCBI_XALGOGNOMON_EXPORT CNcbiOstream& operator<<(CNcbiOstream& s, const setcontig& c);
NCBI_XALGOGNOMON_EXPORT CNcbiIstream& operator>>(CNcbiIstream& s, const getcontig& c);

NCBI_XALGOGNOMON_EXPORT CNcbiIstream& operator>>(CNcbiIstream& s, CGeneModel& a);
NCBI_XALGOGNOMON_EXPORT CNcbiOstream& operator<<(CNcbiOstream& s, const CGeneModel& a);

typedef list<CGeneModel> TAlignList;

class NCBI_XALGOGNOMON_EXPORT CCluster : public TAlignList
{
 public:
    CCluster(int f = numeric_limits<int>::max(), int s = 0) : m_limits(f,s) {}
    CCluster(TSignedSeqRange limits) : m_limits(limits) {}
    void Insert(const CGeneModel& a);
    void Splice(CCluster& c); // elements removed from c and inserted into *this
    TSignedSeqRange Limits() const { return m_limits; }
    bool operator<(const CCluster& c) const { return Precede(m_limits, c.m_limits); }
    void Init(TSignedSeqPos first, TSignedSeqPos second);

    //    CNcbiOstream& print(CNcbiOstream& s) const;

 private:
    TSignedSeqRange m_limits;
};

class NCBI_XALGOGNOMON_EXPORT CClusterSet : public set<CCluster>
{
 public:
    CClusterSet() {}
    void Insert(const CGeneModel& a);
};

END_SCOPE(gnomon)
END_NCBI_SCOPE


/*
 * ===========================================================================
 * $Log$
 * Revision 1.16.2.8  2007/01/11 16:15:02  souvorov
 * Implementation of the composite measure
 *
 * Revision 1.16.2.7  2006/12/21 15:44:22  souvorov
 *  CFrameShiftedSeqMap introduction
 *
 * Revision 1.16.2.6  2006/11/30 20:10:02  souvorov
 * Implementation of proper mapping for prediction
 *
 * Revision 1.16.2.5  2006/11/28 19:48:40  souvorov
 * Introduction of CFrameShiftedSeqMap
 *
 * Revision 1.16.2.4  2006/11/03 20:24:58  chetvern
 * Added start/stop codon test functions
 *
 * Revision 1.16.2.3  2006/10/26 21:18:22  chetvern
 * Convert Deletions into Insertions
 *
 * Revision 1.16.2.2  2006/10/24 19:42:39  souvorov
 * Modification for open alignments
 *
 * Revision 1.16.2.1  2006/10/06 14:17:56  chetvern
 * Major overhaul. Single format for intermediate files.
 *
 * Revision 1.20  2006/06/29 19:19:22  souvorov
 * Confirmed start implementation
 *
 * Revision 1.19  2006/05/11 19:25:44  souvorov
 * ConfirmedStart added
 *
 * Revision 1.18  2006/05/11 17:40:07  souvorov
 * NMD test and GeneID for CGeneModel
 *
 * Revision 1.17  2006/04/24 13:53:47  dicuccio
 * FIx compiler warning on MSVC
 *
 * Revision 1.16  2006/02/03 20:25:46  souvorov
 * Use flags for CGeneModel properties
 *
 * Revision 1.15  2005/12/16 14:29:13  chetvern
 * remove extra method declaration
 *
 * Revision 1.14  2005/11/21 21:28:38  chetvern
 * Small changes in CAlignExon and CClusterSet interfaces
 *
 * Revision 1.13  2005/10/21 15:21:46  souvorov
 * CGeneModel::SetLimits deleted
 *
 * Revision 1.12  2005/10/21 13:35:19  chetvern
 * Implemented CGeneModel::operator==
 *
 * Revision 1.11  2005/10/20 19:24:50  souvorov
 * SetLimits for CGeneModel
 *
 * Revision 1.10  2005/10/20 18:10:02  chetvern
 * Fixed CGeneModel::IdenticalAlign method
 *
 * Revision 1.9  2005/10/14 20:09:53  chetvern
 * added CGeneModel::SetCdsInfo method
 *
 * Revision 1.8  2005/10/13 19:04:37  chetvern
 * added CCluster::Splice method
 *
 * Revision 1.7  2005/10/06 18:08:39  ucko
 * Tweak RecalculateLimits, as some compiler versions took issue with
 * its use of ?:.
 *
 * Revision 1.6  2005/10/06 15:49:21  chetvern
 * added precomputed limits to CGeneModel
 *
 * Revision 1.5  2005/10/06 14:36:03  souvorov
 * Editorial corrections
 *
 * Revision 1.4  2005/09/30 18:57:53  chetvern
 * added m_status and m_name to CGeneModel
 * removed m_contig from CClusterSet
 * moved frameshifts from CExonData to CGene
 *
 * Revision 1.3  2005/09/16 20:22:28  ucko
 * Whoops, MIPSpro also needs operator!= for CFrameShiftInfo.
 *
 * Revision 1.2  2005/09/16 18:02:31  ucko
 * Formal portability fixes:
 * - Replace kBadScore with an inline BadScore function that always
 *   returns the same value to avoid lossage in optimized WorkShop builds.
 * - Use <corelib/ncbi_limits.hpp> rather than <limits> for GCC 2.95.
 * - Supply CAlignExon::operator!= to satisfy SGI's MIPSpro compiler.
 * Don't bother explicitly including STL headers already pulled in by
 * ncbistd.hpp.
 *
 * Revision 1.1  2005/09/15 21:16:01  chetvern
 * redesigned API
 *
 * Revision 1.6  2005/06/03 18:17:03  souvorov
 * Change to indels mapping
 *
 * Revision 1.5  2005/04/13 19:02:37  souvorov
 * Score output for exons
 *
 * Revision 1.4  2005/04/07 15:25:30  souvorov
 * Output for supported length
 *
 * Revision 1.3  2005/03/23 20:50:02  souvorov
 * Mulpiprot penalty introduction
 *
 * Revision 1.2  2005/03/21 16:27:39  souvorov
 * Added check for holes in alignments
 *
 * Revision 1.1  2005/02/18 15:18:31  souvorov
 * First commit
 *
 * Revision 1.3  2004/07/28 12:33:18  dicuccio
 * Sync with Sasha's working tree
 *
 * Revision 1.2  2003/11/06 15:02:21  ucko
 * Use iostream interface from ncbistre.hpp for GCC 2.95 compatibility.
 *
 * Revision 1.1  2003/10/24 15:07:25  dicuccio
 * Initial revision
 *
 * ===========================================================================
 */

#endif  // ALGO_GNOMON___GNOMON_MODEL__HPP
