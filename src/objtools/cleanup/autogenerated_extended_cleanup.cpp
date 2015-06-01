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
 */ 
/// This file was generated by application DATATOOL
///
/// ATTENTION:
///   Don't edit or commit this file into SVN as this file will
///   be overridden (by DATATOOL) without warning!

#include <ncbi_pch.hpp>
#include "autogenerated_extended_cleanup.hpp"
#include "cleanup_utils.hpp"
#include "autogenerated_cleanup_extra.hpp"
#include <objects/misc/sequence_macros.hpp>

BEGIN_SCOPE(ncbi)
BEGIN_SCOPE(objects)

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( std::string & arg0 )
{ // type Primitive
  m_NewCleanup.x_TrimInternalSemicolonsMarkChanged( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_E1798_ETC( COrgName & arg0 )
{ // type Sequence
  if( arg0.IsSetAttrib() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetAttrib() );
  }
  if( arg0.IsSetLineage() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetLineage() );
  }
  if( arg0.IsSetName() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name( arg0.SetName() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_E1798_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid_E( COrgName & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_E1798_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid_E

template< typename Tcontainer_ncbi_cref_corgname_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid( Tcontainer_ncbi_cref_corgname_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_corgname_, iter, arg0 ) { 
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid_E( **iter );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid( CMultiOrgName & arg0 )
{ // type Reference
  if( arg0.IsSet() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid_hybrid( arg0.Set() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name( COrgName::C_Name & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case COrgName::C_Name::e_Hybrid:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name_hybrid( arg0.SetHybrid() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname1799_ETC( COrgName & arg0 )
{ // type Sequence
  if( arg0.IsSetAttrib() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetAttrib() );
  }
  if( arg0.IsSetLineage() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetLineage() );
  }
  if( arg0.IsSetName() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_name( arg0.SetName() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname1799_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_ETC( COrgName & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname1799_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_ETC( COrg_ref & arg0 )
{ // type Sequence
  if( arg0.IsSetOrgname() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_ETC( arg0.SetOrgname() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( COrg_ref & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_ETC( CBioSource & arg0 )
{ // type Sequence
  m_NewCleanup.BioSourceEC( arg0 );
  if( arg0.IsSetOrg() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( arg0.SetOrg() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_ETC( CBioSource & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_data_data_gene_gene( CGene_ref & arg0 )
{ // type Sequence
  m_NewCleanup.x_RemoveRedundantComment( arg0, *m_LastArg_ExtendedCleanupSeqFeat );
  if( arg0.IsSetLocus_tag() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetLocus_tag() );
  }
  if( arg0.IsSetMaploc() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetMaploc() );
  }
} // end of x_ExtendedCleanupSeqFeat_data_data_gene_gene

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_data_data_gene( CGene_ref & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_data_data_gene_gene( arg0 );
} // end of x_ExtendedCleanupSeqFeat_data_data_gene

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_imp_ETC( CImp_feat & arg0 )
{ // type Sequence
  if( arg0.IsSetKey() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetKey() );
  }
  if( arg0.IsSetLoc() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetLoc() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_imp_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_ETC( CImp_feat & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_imp_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_E_ETC( CProt_ref & arg0 )
{ // type Sequence
  m_NewCleanup.x_RemoveProtDescThatDupsProtName( arg0 );
  m_NewCleanup.ProtRefEC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_E_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_ETC( CProt_ref & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_E_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_E_ETC( CGene_ref & arg0 )
{ // type Sequence
  if( arg0.IsSetLocus_tag() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetLocus_tag() );
  }
  if( arg0.IsSetMaploc() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetMaploc() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_E_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_ETC( CGene_ref & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_E_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_ETC

template< typename Tcontainer_ncbi_cref_cgene_ref_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_ETC( Tcontainer_ncbi_cref_cgene_ref_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cgene_ref_, iter, arg0 ) { 
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_ETC( **iter );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_ETC

template< typename Tcontainer_ncbi_cref_cprot_ref_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_ETC( Tcontainer_ncbi_cref_cprot_ref_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cprot_ref_, iter, arg0 ) { 
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_ETC( **iter );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_ETC( CTxinit & arg0 )
{ // type Sequence
  if( arg0.IsSetGene() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_ETC( arg0.SetGene() );
  }
  if( arg0.IsSetProtein() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_ETC( arg0.SetProtein() );
  }
  if( arg0.IsSetTxorg() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( arg0.SetTxorg() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_ETC( CTxinit & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_data_data( CSeqFeatData & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeqFeatData::e_Biosrc:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_ETC( arg0.SetBiosrc() );
    break;
  case CSeqFeatData::e_Gene:
    x_ExtendedCleanupSeqFeat_data_data_gene( arg0.SetGene() );
    break;
  case CSeqFeatData::e_Imp:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_ETC( arg0.SetImp() );
    break;
  case CSeqFeatData::e_Org:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( arg0.SetOrg() );
    break;
  case CSeqFeatData::e_Prot:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_ETC( arg0.SetProt() );
    break;
  case CSeqFeatData::e_Txinit:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_ETC( arg0.SetTxinit() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupSeqFeat_data_data

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_data( CSeqFeatData & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_data_data( arg0 );
} // end of x_ExtendedCleanupSeqFeat_data

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_data_ETC( CSeqFeatData & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeqFeatData::e_Biosrc:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_ETC( arg0.SetBiosrc() );
    break;
  case CSeqFeatData::e_Gene:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_gene_E_ETC( arg0.SetGene() );
    break;
  case CSeqFeatData::e_Imp:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_imp_ETC( arg0.SetImp() );
    break;
  case CSeqFeatData::e_Org:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( arg0.SetOrg() );
    break;
  case CSeqFeatData::e_Prot:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_txinit_protein_E_ETC( arg0.SetProt() );
    break;
  case CSeqFeatData::e_Txinit:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_txinit_ETC( arg0.SetTxinit() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_data_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_data_ETC( CSeqFeatData & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_data_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_E_ETC( CSeqFeatXref & arg0 )
{ // type Sequence
  if( arg0.IsSetData() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_ETC( arg0.SetData() );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_E_E_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_E_ETC( CSeqFeatXref & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqFeat_xref_E_E_ETC( arg0 );
} // end of x_ExtendedCleanupSeqFeat_xref_E_ETC

template< typename Tcontainer_ncbi_cref_cseqfeatxref_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqFeat_xref_ETC( Tcontainer_ncbi_cref_cseqfeatxref_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseqfeatxref_, iter, arg0 ) { 
    x_ExtendedCleanupSeqFeat_xref_E_ETC( **iter );
  }
} // end of x_ExtendedCleanupSeqFeat_xref_ETC

void CAutogeneratedExtendedCleanup::ExtendedCleanupSeqFeat( CSeq_feat & arg0_raw )
{ // type Sequence
  CRef<CSeq_feat> raw_ref( &arg0_raw );
  CSeq_feat_EditHandle efh;

  CRef<CSeq_feat> new_feat;

  try {
    // Try to use an edit handle so we can update the object manager
    efh = CSeq_feat_EditHandle( m_Scope.GetSeq_featHandle( arg0_raw ) );
    new_feat.Reset( new CSeq_feat );
    new_feat->Assign( arg0_raw );
  } catch(...) {
    new_feat.Reset( &arg0_raw );
  }

  CSeq_feat &arg0 = *new_feat;

  m_LastArg_ExtendedCleanupSeqFeat = &arg0;

  m_NewCleanup.CdRegionEC( arg0 );
  m_NewCleanup.x_MoveSeqfeatOrgToSourceOrg( arg0 );
  if( arg0.IsSetData() ) {
    x_ExtendedCleanupSeqFeat_data( arg0.SetData() );
  }
  if( arg0.IsSetTitle() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetTitle() );
  }
  if( arg0.IsSetXref() ) {
    x_ExtendedCleanupSeqFeat_xref_ETC( arg0.SetXref() );
  }

  m_LastArg_ExtendedCleanupSeqFeat = NULL;

  if( efh ) {
    efh.Replace(arg0);
    arg0_raw.Assign( arg0 );
  }

} // end of ExtendedCleanupSeqFeat

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqAnnot_data_ftable_E( CSeq_feat & arg0 )
{ // type Reference
    ExtendedCleanupSeqFeat( arg0 );
} // end of x_ExtendedCleanupSeqAnnot_data_ftable_E

template< typename Tcontainer_ncbi_cref_cseq_feat_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqAnnot_data_ftable( Tcontainer_ncbi_cref_cseq_feat_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_feat_, iter, arg0 ) { 
    x_ExtendedCleanupSeqAnnot_data_ftable_E( **iter );
  }
} // end of x_ExtendedCleanupSeqAnnot_data_ftable

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqAnnot_data( CSeq_annot::C_Data & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeq_annot::C_Data::e_Ftable:
    x_ExtendedCleanupSeqAnnot_data_ftable( arg0.SetFtable() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupSeqAnnot_data

void CAutogeneratedExtendedCleanup::ExtendedCleanupSeqAnnot( CSeq_annot & arg0 )
{ // type Sequence
  m_NewCleanup.x_RemoveEmptyFeatures( arg0 );
  if( arg0.IsSetData() ) {
    x_ExtendedCleanupSeqAnnot_data( arg0.SetData() );
  }
} // end of ExtendedCleanupSeqAnnot

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_annot_E( CSeq_annot & arg0 )
{ // type Reference
    ExtendedCleanupSeqAnnot( arg0 );
} // end of x_ExtendedCleanupBioseq_annot_E

template< typename Tcontainer_ncbi_cref_cseq_annot_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_annot( Tcontainer_ncbi_cref_cseq_annot_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_annot_, iter, arg0 ) { 
    x_ExtendedCleanupBioseq_annot_E( **iter );
  }
} // end of x_ExtendedCleanupBioseq_annot

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_genbank_ETC( CGB_block & arg0 )
{ // type Sequence
  if( arg0.IsSetOrigin() ) {
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_org_orgname_orgname_lineage_ETC( arg0.SetOrigin() );
  }
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_genbank_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_ETC( CGB_block & arg0 )
{ // type Reference
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_genbank_ETC( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_ETC( CSeqdesc & arg0 )
{ // type Choice
  m_NewCleanup.x_MoveSeqdescOrgToSourceOrg( arg0 );
  switch( arg0.Which() ) {
  case CSeqdesc::e_Genbank:
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_genbank_ETC( arg0.SetGenbank() );
    break;
  case CSeqdesc::e_Org:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_biosrc_org_ETC( arg0.SetOrg() );
    break;
  case CSeqdesc::e_Source:
    x_ExtendedCleanupSeqFeat_xref_E_E_data_data_biosrc_ETC( arg0.SetSource() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_ETC( CSeqdesc & arg0 )
{ // type Reference
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_E_ETC( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_ETC

template< typename Tcontainer_ncbi_cref_cseqdesc_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_ETC( Tcontainer_ncbi_cref_cseqdesc_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseqdesc_, iter, arg0 ) { 
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_E_ETC( **iter );
  }
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_ETC( CSeq_descr & arg0 )
{ // type Reference
  m_NewCleanup.x_RemoveEmptyUserObject( arg0 );
  if( arg0.IsSet() ) {
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_descr_ETC( arg0.Set() );
  }
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map_E( CSeq_feat & arg0 )
{ // type Reference
    ExtendedCleanupSeqFeat( arg0 );
} // end of x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map_E

template< typename Tcontainer_ncbi_cref_cseq_feat_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map( Tcontainer_ncbi_cref_cseq_feat_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_feat_, iter, arg0 ) { 
    x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map_E( **iter );
  }
} // end of x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst_ext_ext_map( CMap_ext & arg0 )
{ // type Reference
  if( arg0.IsSet() ) {
    x_ExtendedCleanupBioseq_inst_inst_ext_ext_map_map( arg0.Set() );
  }
} // end of x_ExtendedCleanupBioseq_inst_inst_ext_ext_map

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst_ext_ext( CSeq_ext & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeq_ext::e_Map:
    x_ExtendedCleanupBioseq_inst_inst_ext_ext_map( arg0.SetMap() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupBioseq_inst_inst_ext_ext

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst_ext( CSeq_ext & arg0 )
{ // type Reference
    x_ExtendedCleanupBioseq_inst_inst_ext_ext( arg0 );
} // end of x_ExtendedCleanupBioseq_inst_inst_ext

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst_inst( CSeq_inst & arg0 )
{ // type Sequence
  if( arg0.IsSetExt() ) {
    x_ExtendedCleanupBioseq_inst_inst_ext( arg0.SetExt() );
  }
} // end of x_ExtendedCleanupBioseq_inst_inst

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseq_inst( CSeq_inst & arg0 )
{ // type Reference
    x_ExtendedCleanupBioseq_inst_inst( arg0 );
} // end of x_ExtendedCleanupBioseq_inst

void CAutogeneratedExtendedCleanup::ExtendedCleanupBioseq( CBioseq & arg0 )
{ // type Sequence
  m_NewCleanup.x_RemoveDupBioSource( arg0 );
  m_NewCleanup.x_FixStructuredCommentKeywords( arg0 );
  m_NewCleanup.CreateMissingMolInfo( arg0 );
  m_NewCleanup.AddProteinTitles( arg0 );
  m_NewCleanup.x_RemoveEmptyFeatureTables( arg0 );
  if( arg0.IsSetAnnot() ) {
    x_ExtendedCleanupBioseq_annot( arg0.SetAnnot() );
  }
  if( arg0.IsSetDescr() ) {
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_ETC( arg0.SetDescr() );
  }
  if( arg0.IsSetInst() ) {
    x_ExtendedCleanupBioseq_inst( arg0.SetInst() );
  }
  m_NewCleanup.x_ClearEmptyDescr( arg0 );
} // end of ExtendedCleanupBioseq

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_seq( CBioseq & arg0 )
{ // type Reference
    ExtendedCleanupBioseq( arg0 );
} // end of x_ExtendedCleanupSeqEntry_seq

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set_set_annot_E( CSeq_annot & arg0 )
{ // type Reference
    ExtendedCleanupSeqAnnot( arg0 );
} // end of x_ExtendedCleanupSeqEntry_set_set_annot_E

template< typename Tcontainer_ncbi_cref_cseq_annot_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set_set_annot( Tcontainer_ncbi_cref_cseq_annot_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_annot_, iter, arg0 ) { 
    x_ExtendedCleanupSeqEntry_set_set_annot_E( **iter );
  }
} // end of x_ExtendedCleanupSeqEntry_set_set_annot

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set_set_seq_set_E( CSeq_entry & arg0 )
{ // type Reference
    ExtendedCleanupSeqEntry( arg0 );
} // end of x_ExtendedCleanupSeqEntry_set_set_seq_set_E

template< typename Tcontainer_ncbi_cref_cseq_entry_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set_set_seq_set( Tcontainer_ncbi_cref_cseq_entry_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_entry_, iter, arg0 ) { 
    x_ExtendedCleanupSeqEntry_set_set_seq_set_E( **iter );
  }
} // end of x_ExtendedCleanupSeqEntry_set_set_seq_set

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set_set_ETC( CBioseq_set & arg0 )
{ // type Sequence
  m_NewCleanup.x_BioseqSetEC( arg0 );
  m_NewCleanup.x_RemoveDupBioSource( arg0 );
  m_NewCleanup.x_MoveCDSFromNucAnnotToSetAnnot( arg0 );
  m_NewCleanup.x_RemoveEmptyFeatureTables( arg0 );
  if( arg0.IsSetAnnot() ) {
    x_ExtendedCleanupSeqEntry_set_set_annot( arg0.SetAnnot() );
  }
  if( arg0.IsSetDescr() ) {
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_ETC( arg0.SetDescr() );
  }
  if( arg0.IsSetSeq_set() ) {
    x_ExtendedCleanupSeqEntry_set_set_seq_set( arg0.SetSeq_set() );
  }
  m_NewCleanup.x_ClearEmptyDescr( arg0 );
} // end of x_ExtendedCleanupSeqEntry_set_set_ETC

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqEntry_set( CBioseq_set & arg0 )
{ // type Reference
    x_ExtendedCleanupSeqEntry_set_set_ETC( arg0 );
} // end of x_ExtendedCleanupSeqEntry_set

void CAutogeneratedExtendedCleanup::ExtendedCleanupSeqEntry( CSeq_entry & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeq_entry::e_Seq:
    x_ExtendedCleanupSeqEntry_seq( arg0.SetSeq() );
    break;
  case CSeq_entry::e_Set:
    x_ExtendedCleanupSeqEntry_set( arg0.SetSet() );
    break;
  default:
    break;
  }
  m_NewCleanup.x_SortSeqDescs( arg0 );
} // end of ExtendedCleanupSeqEntry

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqSubmit_data_annots_E( CSeq_annot & arg0 )
{ // type Reference
    ExtendedCleanupSeqAnnot( arg0 );
} // end of x_ExtendedCleanupSeqSubmit_data_annots_E

template< typename Tcontainer_ncbi_cref_cseq_annot_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqSubmit_data_annots( Tcontainer_ncbi_cref_cseq_annot_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_annot_, iter, arg0 ) { 
    x_ExtendedCleanupSeqSubmit_data_annots_E( **iter );
  }
} // end of x_ExtendedCleanupSeqSubmit_data_annots

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqSubmit_data_entrys_E( CSeq_entry & arg0 )
{ // type Reference
    ExtendedCleanupSeqEntry( arg0 );
} // end of x_ExtendedCleanupSeqSubmit_data_entrys_E

template< typename Tcontainer_ncbi_cref_cseq_entry_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqSubmit_data_entrys( Tcontainer_ncbi_cref_cseq_entry_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_entry_, iter, arg0 ) { 
    x_ExtendedCleanupSeqSubmit_data_entrys_E( **iter );
  }
} // end of x_ExtendedCleanupSeqSubmit_data_entrys

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupSeqSubmit_data( CSeq_submit::C_Data & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeq_submit::C_Data::e_Annots:
    x_ExtendedCleanupSeqSubmit_data_annots( arg0.SetAnnots() );
    break;
  case CSeq_submit::C_Data::e_Entrys:
    x_ExtendedCleanupSeqSubmit_data_entrys( arg0.SetEntrys() );
    break;
  default:
    break;
  }
} // end of x_ExtendedCleanupSeqSubmit_data

void CAutogeneratedExtendedCleanup::ExtendedCleanupSeqSubmit( CSeq_submit & arg0 )
{ // type Sequence
  if( arg0.IsSetData() ) {
    x_ExtendedCleanupSeqSubmit_data( arg0.SetData() );
  }
} // end of ExtendedCleanupSeqSubmit

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_annot_E( CSeq_annot & arg0 )
{ // type Reference
    ExtendedCleanupSeqAnnot( arg0 );
} // end of x_ExtendedCleanupBioseqSet_annot_E

template< typename Tcontainer_ncbi_cref_cseq_annot_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_annot( Tcontainer_ncbi_cref_cseq_annot_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_annot_, iter, arg0 ) { 
    x_ExtendedCleanupBioseqSet_annot_E( **iter );
  }
} // end of x_ExtendedCleanupBioseqSet_annot

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_seq( CBioseq & arg0 )
{ // type Reference
    ExtendedCleanupBioseq( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_seq

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E_set( CBioseq_set & arg0 )
{ // type Reference
    ExtendedCleanupBioseqSet( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E_set

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E_E( CSeq_entry & arg0 )
{ // type Choice
  switch( arg0.Which() ) {
  case CSeq_entry::e_Seq:
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq( arg0.SetSeq() );
    break;
  case CSeq_entry::e_Set:
    x_ExtendedCleanupBioseqSet_seq_set_E_E_set( arg0.SetSet() );
    break;
  default:
    break;
  }
  m_NewCleanup.x_SortSeqDescs( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E_E

void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set_E( CSeq_entry & arg0 )
{ // type Reference
    x_ExtendedCleanupBioseqSet_seq_set_E_E( arg0 );
} // end of x_ExtendedCleanupBioseqSet_seq_set_E

template< typename Tcontainer_ncbi_cref_cseq_entry_ >
void CAutogeneratedExtendedCleanup::x_ExtendedCleanupBioseqSet_seq_set( Tcontainer_ncbi_cref_cseq_entry_ & arg0 )
{ // type UniSequence
  NON_CONST_ITERATE( typename Tcontainer_ncbi_cref_cseq_entry_, iter, arg0 ) { 
    x_ExtendedCleanupBioseqSet_seq_set_E( **iter );
  }
} // end of x_ExtendedCleanupBioseqSet_seq_set

void CAutogeneratedExtendedCleanup::ExtendedCleanupBioseqSet( CBioseq_set & arg0 )
{ // type Sequence
  m_NewCleanup.x_BioseqSetEC( arg0 );
  m_NewCleanup.x_RemoveDupBioSource( arg0 );
  m_NewCleanup.x_MoveCDSFromNucAnnotToSetAnnot( arg0 );
  m_NewCleanup.x_RemoveEmptyFeatureTables( arg0 );
  if( arg0.IsSetAnnot() ) {
    x_ExtendedCleanupBioseqSet_annot( arg0.SetAnnot() );
  }
  if( arg0.IsSetDescr() ) {
    x_ExtendedCleanupBioseqSet_seq_set_E_E_seq_seq_descr_ETC( arg0.SetDescr() );
  }
  if( arg0.IsSetSeq_set() ) {
    x_ExtendedCleanupBioseqSet_seq_set( arg0.SetSeq_set() );
  }
  m_NewCleanup.x_ClearEmptyDescr( arg0 );
} // end of ExtendedCleanupBioseqSet


END_SCOPE(objects)
END_SCOPE(ncbi)

