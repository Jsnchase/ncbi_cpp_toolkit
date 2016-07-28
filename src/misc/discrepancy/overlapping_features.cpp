/*  $Id$
 * =========================================================================
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
 * =========================================================================
 *
 * Authors: Sema Kachalo
 *
 */

#include <ncbi_pch.hpp>
#include "discrepancy_core.hpp"
#include <objmgr/util/sequence.hpp>

BEGIN_NCBI_SCOPE
BEGIN_SCOPE(NDiscrepancy)
USING_SCOPE(objects);

DISCREPANCY_MODULE(overlapping_features);

// CDS_TRNA_OVERLAP

DISCREPANCY_CASE(CDS_TRNA_OVERLAP, COverlappingFeatures, eDisc | eSubmitter | eSmart, "CDS tRNA Overlap")
{
    const vector<CConstRef<CSeq_feat> >& cds = context.FeatCDS();
    const vector<CConstRef<CSeq_feat> >& trnas = context.FeatTRNAs();
    bool increase_count = false;
    for (size_t i = 0; i < cds.size(); i++) {
        const CSeq_loc& loc_i = cds[i]->GetLocation();
        for (size_t j = 0; j < trnas.size(); j++) {
            const CSeq_loc& loc_j = trnas[j]->GetLocation();
            sequence::ECompare ovlp = sequence::Compare(loc_i, loc_j, &context.GetScope(), sequence::fCompareOverlapping);
            if (ovlp != sequence::eNoOverlap) {
                increase_count = true;
                CReportNode& out = m_Objs["[n] Bioseq[s] [has] coding regions that overlap tRNAs"];
                out["[n] coding region[s] [has] overlapping tRNAs"].Incr().Ext().Add(*context.NewDiscObj(cds[i])).Add(*context.NewDiscObj(trnas[j]));
                break;
            }
        }
    }
    if (increase_count) {
        m_Objs["[n] Bioseq[s] [has] coding regions that overlap tRNAs"].Incr();
    }
}


DISCREPANCY_SUMMARIZE(CDS_TRNA_OVERLAP)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


// RNA_CDS_OVERLAP

typedef pair<size_t, bool> TRNALength;
typedef map<string, TRNALength > TRNALengthMap;

static const TRNALengthMap kTrnaLengthMap{
    { "16S", { 1000, false } },
    { "18S", { 1000, false } },
    { "23S", { 2000, false } },
    { "25S", { 1000, false } },
    { "26S", { 1000, false } },
    { "28S", { 1000, false } },
    { "28S", { 3300, false } },
    { "small", { 1000, false } },
    { "large", { 1000, false } },
    { "5.8S", { 130, true } },
    { "5S", { 90, true } } 
};


bool IsShortrRNA(const CSeq_feat& f, CScope* scope) // used in feature_tests.cpp
{
    if (f.GetData().GetSubtype() != CSeqFeatData::eSubtype_rRNA) {
        return false;
    }

    bool is_bad = false;

    size_t len = sequence::GetLength(f.GetLocation(), scope);

    string rrna_name = f.GetData().GetRna().GetRnaProductName();

    ITERATE (TRNALengthMap, it, kTrnaLengthMap) {
        if (NStr::FindNoCase(rrna_name, it->first) != string::npos &&
            len < it->second.first &&
            (!it->second.second || (f.IsSetPartial() && f.GetPartial())) ) {
            is_bad = true;
            break;
        }
    }

    return is_bad;
}


const string kCDSRNAAnyOverlap = "[n/2] coding region[s] overlap RNA feature[s]";
const string kCDSRNAExactMatch = "[n/2] coding region location[s] exactly match an RNA location";
const string kCDSRNAContainedIn = "[n/2] coding region[s] [is] completely contained in RNA[s]";
const string kCDSRNAContains = "[n/2] coding region[s] completely contain RNA[s]";
const string kCDSRNAContainstRNA = "[n/2] coding region[s] completely contain tRNA[s]";
const string kCDSRNAOverlapNoContain = "[n/2] coding regions overlap RNA[s] (no containment)";
const string kCDSRNAOverlapNoContainSameStrand = "[n/2] coding region[s] overlap RNA[s] on the same strand (no containment)";
const string kCDSRNAOverlapNoContainOppStrand = "[n/2] coding region[s] overlap RNA[s] on the opposite strand (no containment)";


DISCREPANCY_CASE(RNA_CDS_OVERLAP, COverlappingFeatures, eDisc | eSubmitter | eSmart, "CDS RNA Overlap")
{
    const vector<CConstRef<CSeq_feat> >& cds = context.FeatCDS();
    const vector<CConstRef<CSeq_feat> >& rnas = context.Feat_RNAs();
    for (size_t i = 0; i < rnas.size(); i++) {
        const CSeq_loc& loc_i = rnas[i]->GetLocation();
        CSeqFeatData::ESubtype subtype = rnas[i]->GetData().GetSubtype();
        if (subtype == CSeqFeatData::eSubtype_tRNA) {
            if (context.IsEukaryotic()) {
                continue;
            }
        }
        else if (subtype == CSeqFeatData::eSubtype_mRNA || subtype == CSeqFeatData::eSubtype_ncRNA) {
            continue;
        }
        else if (subtype == CSeqFeatData::eSubtype_rRNA) {
            size_t len = sequence::GetLength(loc_i, &context.GetScope());
            string rrna_name = rnas[i]->GetData().GetRna().GetRnaProductName();
            bool is_bad = false;
            ITERATE (TRNALengthMap, it, kTrnaLengthMap) {
                if (NStr::FindNoCase(rrna_name, it->first) != string::npos &&
                        len < it->second.first &&
                        (!it->second.second || (rnas[i]->IsSetPartial() && rnas[i]->GetPartial())) ) {
                    is_bad = true;
                    break;
                }
            }
            if (is_bad) {
                continue;
            }
        }
        for (size_t j = 0; j < cds.size(); j++) {
            const CSeq_loc& loc_j = cds[j]->GetLocation();
            sequence::ECompare compare = sequence::Compare(loc_j, loc_i, &context.GetScope(), sequence::fCompareOverlapping);
            if (compare == sequence::eSame) {
                m_Objs[kCDSRNAAnyOverlap][kCDSRNAExactMatch].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
            }
            else if (compare == sequence::eContained) {
                m_Objs[kCDSRNAAnyOverlap][kCDSRNAContainedIn].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
            }
            else if (compare == sequence::eContains) {
                if (rnas[i]->GetData().GetSubtype() == CSeqFeatData::eSubtype_tRNA) {
                    m_Objs[kCDSRNAAnyOverlap][kCDSRNAContainstRNA].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
                }
                else {
                    m_Objs[kCDSRNAAnyOverlap][kCDSRNAContains].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
                }
            }
            else if (compare != sequence::eNoOverlap) {
                ENa_strand cds_strand = cds[j]->GetLocation().GetStrand();
                ENa_strand rna_strand = rnas[i]->GetLocation().GetStrand();
                if (cds_strand == eNa_strand_minus && rna_strand != eNa_strand_minus) {
                    m_Objs[kCDSRNAAnyOverlap][kCDSRNAOverlapNoContain].Ext()[kCDSRNAOverlapNoContainOppStrand].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
                }
                else if (cds_strand != eNa_strand_minus && rna_strand == eNa_strand_minus) {
                    m_Objs[kCDSRNAAnyOverlap][kCDSRNAOverlapNoContain].Ext()[kCDSRNAOverlapNoContainOppStrand].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
                }
                else {
                    m_Objs[kCDSRNAAnyOverlap][kCDSRNAOverlapNoContain].Ext()[kCDSRNAOverlapNoContainSameStrand].Ext().Add(*context.NewDiscObj(rnas[i]), false).Add(*context.NewDiscObj(cds[j]), false).Fatal();
                }
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(RNA_CDS_OVERLAP)
{
    m_ReportItems = m_Objs.Export(*this, false)->GetSubitems();
}


DISCREPANCY_CASE(OVERLAPPING_RRNAS, COverlappingFeatures, eDisc | eSubmitter | eSmart, "Overlapping rRNAs")
{
    const vector<CConstRef<CSeq_feat> >& rrnas = context.FeatRRNAs();
    for (size_t i = 0; i < rrnas.size(); i++) {
        const CSeq_loc& loc_i = rrnas[i]->GetLocation();
        for (size_t j = i + 1; j < rrnas.size(); j++) {
            const CSeq_loc& loc_j = rrnas[j]->GetLocation();
            if (sequence::Compare(loc_j, loc_i, &context.GetScope(), sequence::fCompareOverlapping) != sequence::eNoOverlap) {
                m_Objs["[n] rRNA feature[s] overlap[S] another rRNA feature."].Add(*context.NewDiscObj(rrnas[i])).Add(*context.NewDiscObj(rrnas[j])).Fatal();
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(OVERLAPPING_RRNAS)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


// OVERLAPPING_GENES

DISCREPANCY_CASE(OVERLAPPING_GENES, COverlappingFeatures, eDisc, "Overlapping Genes")
{
    const vector<CConstRef<CSeq_feat> >& genes = context.FeatGenes();
    for (size_t i = 0; i < genes.size(); i++) {
        const CSeq_loc& loc_i = genes[i]->GetLocation();
        for (size_t j = i + 1; j < genes.size(); j++) {
            const CSeq_loc& loc_j = genes[j]->GetLocation();
            if (loc_j.GetStrand() == loc_i.GetStrand() && sequence::Compare(loc_j, loc_i, &context.GetScope(), sequence::fCompareOverlapping) != sequence::eNoOverlap) {
                m_Objs["[n] gene[s] overlap[S] another gene on the same strand."].Add(*context.NewDiscObj(genes[i])).Add(*context.NewDiscObj(genes[j]));
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(OVERLAPPING_GENES)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


// FIND_OVERLAPPED_GENES

DISCREPANCY_CASE(FIND_OVERLAPPED_GENES, COverlappingFeatures, eDisc | eSmart, "Genes completely contained by another gene on the same strand")
{
    const vector<CConstRef<CSeq_feat> >& genes = context.FeatGenes();
    for (size_t i = 0; i < genes.size(); i++) {
        const CSeq_loc& loc_i = genes[i]->GetLocation();
        for (size_t j = i + 1; j < genes.size(); j++) {
            const CSeq_loc& loc_j = genes[j]->GetLocation();
            sequence::ECompare ovlp = sequence::Compare(loc_i, loc_j, &context.GetScope(), sequence::fCompareOverlapping);
            if (ovlp == sequence::eContained || ovlp == sequence::eSame) {
                m_Objs["[n] gene[s] completely overlapped by other genes"].Add(*context.NewDiscObj(genes[i]));
            }
            else if (ovlp == sequence::eContains) {
                m_Objs["[n] gene[s] completely overlapped by other genes"].Add(*context.NewDiscObj(genes[j]));
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(FIND_OVERLAPPED_GENES)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


// DUP_GENES_OPPOSITE_STRANDS

DISCREPANCY_CASE(DUP_GENES_OPPOSITE_STRANDS, COverlappingFeatures, eDisc | eOncaller | eSubmitter | eSmart, "Genes match other genes in the same location, but on the opposite strand")
{
    const vector<CConstRef<CSeq_feat> >& genes = context.FeatGenes();
    for (size_t i = 0; i < genes.size(); i++) {
        const CSeq_loc& loc_i = genes[i]->GetLocation();
        for (size_t j = i + 1; j < genes.size(); j++) {
            const CSeq_loc& loc_j = genes[j]->GetLocation();
            if (loc_i.GetStrand() == loc_j.GetStrand()) {
                continue;
            }
            sequence::ECompare ovlp = sequence::Compare(loc_i, loc_j, &context.GetScope(), sequence::fCompareOverlapping);
            if (ovlp == sequence::eSame) {
                m_Objs["[n] genes match other genes in the same location, but on the opposite strand"].Add(*context.NewDiscObj(genes[i])).Add(*context.NewDiscObj(genes[j]));
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(DUP_GENES_OPPOSITE_STRANDS)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


// MRNA_OVERLAPPING_PSEUDO_GENE

DISCREPANCY_CASE(MRNA_OVERLAPPING_PSEUDO_GENE, COverlappingFeatures, eOncaller, "mRNA overlapping pseudo gene")
{
    const vector<CConstRef<CSeq_feat> >& pseudo = context.FeatPseudo();
    const vector<CConstRef<CSeq_feat> >& mrnas = context.FeatMRNAs();
    for (size_t i = 0; i < mrnas.size(); i++) {
        const CSeq_loc& loc_i = mrnas[i]->GetLocation();
        for (size_t j = 0; j < pseudo.size(); j++) {
            const CSeq_loc& loc_j = pseudo[j]->GetLocation();
            sequence::ECompare ovlp = sequence::Compare(loc_i, loc_j, &context.GetScope(), sequence::fCompareOverlapping);
            if (ovlp != sequence::eNoOverlap) {
                m_Objs["[n] Pseudogene[s] [has] overlapping mRNA[s]."].Add(*context.NewDiscObj(mrnas[i]));  // should say "n mRNAs overlapping pseudogenes", but C Toolkit reports this way.
                break;
            }
        }
    }
}


DISCREPANCY_SUMMARIZE(MRNA_OVERLAPPING_PSEUDO_GENE)
{
    m_ReportItems = m_Objs.Export(*this)->GetSubitems();
}


END_SCOPE(NDiscrepancy)
END_NCBI_SCOPE
