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
* Author:  Aaron Ucko, Wratko Hlavina
*
* File Description:
*   Flat formatter for Generic Feature Format (incl. Gene Transfer Format)
*
* ===========================================================================
*/

#include <objtools/flat/flat_gff_formatter.hpp>
#include <objtools/flat/flat_head.hpp>
#include <objtools/flat/flat_items.hpp>

#include <objects/general/Dbtag.hpp>
#include <objects/general/Int_fuzz.hpp>
#include <objects/seqfeat/Cdregion.hpp>
#include <objects/seqfeat/Genetic_code_table.hpp>

#include <objmgr/seq_vector.hpp>
#include <objmgr/util/sequence.hpp>

#include <algorithm>

BEGIN_NCBI_SCOPE
BEGIN_SCOPE(objects)


CFlatGFFFormatter::CFlatGFFFormatter(IFlatTextOStream& stream, CScope& scope,
                                     EMode mode, TGFFFlags gff_flags,
                                     EStyle style, TFlags flags)
    : IFlatFormatter(scope, mode, style, flags),
      m_GFFFlags(gff_flags), m_Stream(&stream)
{
    list<string> header;
    header.push_back("##gff-version 2");
    header.push_back("##source-version NCBI C++ formatter 0.1");
    header.push_back("##date " + CurrentTime().AsString("Y-M-D"));
    stream.AddParagraph(header);
}


void CFlatGFFFormatter::FormatHead(const CFlatHead& head)
{
    m_Stream->NewSequence();
    list<string> l;

    switch (m_Context->GetMol()) {
    case CSeq_inst::eMol_dna:  m_SeqType = "DNA";      break;
    case CSeq_inst::eMol_rna:  m_SeqType = "RNA";      break;
    case CSeq_inst::eMol_aa:   m_SeqType = "Protein";  break;
    default:                   m_SeqType.erase();      break;
    }
    if ( !m_SeqType.empty() ) {
        l.push_back("##Type " + m_SeqType + ' '
                    + m_Context->GetAccession());
    }

    m_Date.erase();
    head.GetUpdateDate().GetDate(&m_Date, "%4Y-%{%2M%|??%}-%{%2D%|??%}");
    m_Strandedness = head.GetStrandedness();

    m_EndSequence.erase();
    m_Stream->AddParagraph(l, &head);
}


void CFlatGFFFormatter::FormatFeature(const IFlattishFeature& f)
{
    string   key(f.GetKey());
    bool     gtf      = false;
    CSeq_loc tentative_stop;

    if ((m_GFFFlags & fGTFCompat)  &&  !m_Context->IsProt()
        &&  (key == "CDS"  ||  key == "exon")) {
        gtf = true;
    } else if ((m_GFFFlags & fGTFOnly) == fGTFOnly) {
        return;
    }

    CFlatFeature&    feat    = *f.Format();
    const CSeq_feat& seqfeat = feat.GetFeat();
    list<string>     l;
    list<string>     attr_list;

    ITERATE (CFlatFeature::TQuals, it, feat.GetQuals()) {
        const string& name = (*it)->GetName();
        if (name == "codon_start"  ||  name == "translation"
            ||  name == "transcription") {
            continue; // suppressed to reduce verbosity
        } else if ((m_GFFFlags & fGTFCompat)  &&  !m_Context->IsProt()
                   &&  name == "gene") {
            string gene_id = x_GetGeneID(feat, (*it)->GetValue());
            attr_list.push_front
                ("transcript_id \"" + gene_id + '.' + m_Date + "\";");
            attr_list.push_front("gene_id \"" + gene_id + "\";");
            continue;
        }
        string value0((*it)->GetValue()), value;
        ITERATE (string, c, value0) {
            switch (*c) {
            case '\\': value += "\\\\";  break;
            case '\"': value += "\\x22"; break;
            case ' ':  value += "\\x20"; break;
            default:   value += *c;
            }
        }
        attr_list.push_back(name + " \"" + value + "\";");
    }
    string attrs(NStr::Join(attr_list, " "));

    string source = x_GetSourceName(f);

    int frame = -1;
    if (seqfeat.GetData().IsCdregion()  &&  !m_Context->IsProt() ) {
        const CCdregion& cds = seqfeat.GetData().GetCdregion();
        frame = max(cds.GetFrame() - 1, 0);
    }
    tentative_stop.SetNull();
    ITERATE (CFlatLoc::TIntervals, it, feat.GetLoc().GetIntervals()) {
        if (it->m_Accession != m_Context->GetAccession()) {
            continue;
        }

        TSeqPos from = it->m_Range.GetFrom(), to = it->m_Range.GetTo();
        if (gtf  &&  &*it == &feat.GetLoc().GetIntervals().back()
            &&  key == "CDS"  &&  !seqfeat.GetLocation().IsPartialRight() ) {
            CConstRef<CSeq_feat> seqfeat2(&seqfeat);
            if ( !seqfeat.GetLocation().Equals(f.GetLoc()) ) {
                CRef<CSeq_feat> new_feat(new CSeq_feat);
                new_feat->Assign(seqfeat);
                new_feat->SetLocation(const_cast<CSeq_loc&>(f.GetLoc()));
                seqfeat2.Reset(new_feat);
            }
            CRef<CSeq_loc> src_nostop =
                sequence::ProductToSource(*seqfeat2, seqfeat.GetProduct(),
                                          0, m_Scope);
            CSeq_interval&  si = tentative_stop.SetInt();
            CSeq_loc_CI     it2(*src_nostop);
            CRange<TSeqPos> range;
            do {
                range = it2.GetRange();
                si.SetId(const_cast<CSeq_id&>(it2.GetSeq_id()));
            } while (++it2);

            // We need to introduce fuzz so that our potential stop codon
            // won't also have to be a start codon....
            if (it->IsReversed()) {
                from = range.GetFrom() + 1;
                if (from > 3) {
                    si.SetFrom(from - 4);
                    si.SetTo(from - 2);
                    si.SetFuzz_to().SetLim(CInt_fuzz::eLim_gt);
                    si.SetStrand(eNa_strand_minus);
                } else {
                    tentative_stop.SetNull();
                }
            } else if (to + 3 <= sequence::GetLength(si.GetId(), m_Scope)) {
                to = range.GetTo() + 1;
                si.SetFrom(to);
                si.SetTo(to + 2);
                si.SetFuzz_from().SetLim(CInt_fuzz::eLim_lt);
                si.SetStrand(eNa_strand_plus);
            } else {
                tentative_stop.SetNull();
            }
        }
        char strand = '+';
        if (it->IsReversed()) {
            strand = '-';
        } else if (f.GetLoc().IsWhole()
                   ||  (m_Strandedness <= CSeq_inst::eStrand_ss
                        &&  m_Context->GetMol() != CSeq_inst::eMol_dna)) {
            strand = '.'; // N/A
        }
        l.push_back(m_Context->GetAccession() + '\t'
                    + source + '\t'
                    + key + '\t'
                    + NStr::IntToString(from) + '\t'
                    + NStr::IntToString(to) + '\t'
                    + ".\t" // score -- supply if known
                    + strand + '\t'
                    + (frame >= 0 ? char(frame + '0') : '.') + "\t"
                    + attrs);
        if (frame >= 0) {
            frame = (frame + to - from + 1) % 3;
        }
    }

    if (gtf  &&  seqfeat.GetData().IsCdregion()) {
        const CCdregion& cds = seqfeat.GetData().GetCdregion();
        if ( !seqfeat.GetLocation().IsPartialLeft() ) {
            CRef<CSeq_loc> tentative_start;
            {{
                CRef<SRelLoc::TRange> range(new SRelLoc::TRange);
                SRelLoc::TRanges      ranges;
                range->SetFrom(max(cds.GetFrame() - 1, 0));
                range->SetTo(range->GetFrom() + 2);
                ranges.push_back(range);
                tentative_start
                    = SRelLoc(seqfeat.GetLocation(), ranges).Resolve(m_Scope);
            }}

            string s;
            m_Context->GetHandle().GetSequenceView
                (*tentative_start, CBioseq_Handle::eViewConstructed)
                .GetSeqData(0, 3, s);
            const CTrans_table* tt;
            if (cds.IsSetCode()) {
                tt = &CGen_code_table::GetTransTable(cds.GetCode());
            } else {
                tt = &CGen_code_table::GetTransTable(1);
            }
            if (tt->IsAnyStart(tt->SetCodonState(s[0], s[1], s[2]))
                &&  tentative_start->IsInt()
                &&  sequence::IsSameBioseq(tentative_start->GetInt().GetId(),
                                           m_Context->GetPrimaryID(),
                                           m_Scope)) {
                const CSeq_interval& si = tentative_start->GetInt();
                l.push_back(m_Context->GetAccession() + '\t'
                            + source + '\t'
                            + "start_codon\t"
                            + NStr::IntToString(si.GetFrom() + 1) + '\t'
                            + NStr::IntToString(si.GetTo() + 1) + '\t'
                            + ".\t" // score
                            + (IsReverse(si.GetStrand()) ? '-' : '+') + '\t'
                            + "0\t" // frame -- better be 0!
                            + attrs);                
            }
        }
        if ( !tentative_stop.IsNull() ) {
            string s;
            CCdregion_translate::TranslateCdregion
                (s, m_Context->GetHandle(), tentative_stop, cds);
            if (s == "*") {
                const CSeq_interval& si = tentative_stop.GetInt();
                l.push_back(m_Context->GetAccession() + '\t'
                            + source + '\t'
                            + "stop_codon\t"
                            + NStr::IntToString(si.GetFrom() + 1) + '\t'
                            + NStr::IntToString(si.GetTo() + 1) + '\t'
                            + ".\t" // score
                            + (IsReverse(si.GetStrand()) ? '-' : '+') + '\t'
                            + "0\t" // frame -- better be 0!
                            + attrs);
            }
        }
    }

    m_Stream->AddParagraph(l, &f, &seqfeat);
}


void CFlatGFFFormatter::FormatDataHeader(const CFlatDataHeader& dh)
{
    if ( !(m_GFFFlags & fShowSeq) )
        return;

    list<string> l;
    l.push_back("##" + m_SeqType + ' ' + m_Context->GetAccession());
    m_Stream->AddParagraph(l, &dh);
    m_EndSequence = "##end-" + m_SeqType;
}


void CFlatGFFFormatter::FormatData(const CFlatData& data)
{
    if ( !(m_GFFFlags & fShowSeq) )
        return;

    list<string> l;
    CSeqVector v = m_Context->GetHandle().GetSequenceView
        (data.GetLoc(), CBioseq_Handle::eViewConstructed,
         CBioseq_Handle::eCoding_Iupac);
    CSeqVector_CI vi(v);
    while (vi) {
        string s;
        vi.GetSeqData(s, 70);
        l.push_back("##" + s);
    }
    m_Stream->AddParagraph(l, &data, &data.GetLoc());
}


void CFlatGFFFormatter::EndSequence(void)
{
    if ( !m_EndSequence.empty() ) {
        list<string> l;
        l.push_back(m_EndSequence);
        m_Stream->AddParagraph(l);
    }
}


string CFlatGFFFormatter::x_GetGeneID(const CFlatFeature& feat,
                                      const string& gene)
{
    const CSeq_feat& seqfeat = feat.GetFeat();

    string               main_acc;
    if (m_Context->InSegSet()) {
        const CSeq_id& id = *m_Context->GetSegMaster()->GetId().front();
        main_acc = m_Context->GetPreferredSynonym(id).GetSeqIdString(true);
    } else {
        main_acc = m_Context->GetAccession();
    }

    string               gene_id   = main_acc + ':' + gene;
    CConstRef<CSeq_feat> gene_feat = sequence::GetBestOverlappingFeat
        (seqfeat.GetLocation(), CSeqFeatData::e_Gene,
         sequence::eOverlap_Interval, *m_Scope);
    
    TFeatVec&                v  = m_Genes[gene_id];
    TFeatVec::const_iterator it = find(v.begin(), v.end(), gene_feat);
    int                      n;
    if (it == v.end()) {
        n = v.size();
        v.push_back(gene_feat);
    } else {
        n = it - v.begin();
    }
    if (n > 0) {
        gene_id += '.' + NStr::IntToString(n + 1);
    }

    return gene_id;
}


string CFlatGFFFormatter::x_GetSourceName(const IFlattishFeature&)
{
    // XXX - get from annot name (not presently available from IFF)?
    switch (m_Context->GetPrimaryID().Which()) {
    case CSeq_id::e_Local:                           return "Local";
    case CSeq_id::e_Gibbsq: case CSeq_id::e_Gibbmt:
    case CSeq_id::e_Giim:   case CSeq_id::e_Gi:      return "GenInfo";
    case CSeq_id::e_Genbank:                         return "Genbank";
    case CSeq_id::e_Swissprot:                       return "SwissProt";
    case CSeq_id::e_Patent:                          return "Patent";
    case CSeq_id::e_Other:                           return "RefSeq";
    case CSeq_id::e_General:
        return m_Context->GetPrimaryID().GetGeneral().GetDb();
    default:
    {
        string source
            (CSeq_id::SelectionName(m_Context->GetPrimaryID().Which()));
        return NStr::ToUpper(source);
    }
    }
}


END_SCOPE(objects)
END_NCBI_SCOPE

/*
* ===========================================================================
*
* $Log$
* Revision 1.1  2003/10/08 21:11:45  ucko
* New GFF/GTF formatter
*
*
* ===========================================================================
*/
