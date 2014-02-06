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
* Author:  Pavel Ivanov, NCBI
*
* File Description:
*   Sample unit tests file for the mainstream test developing.
*
* This file represents basic most common usage of Ncbi.Test framework (which
* is based on Boost.Test framework. For more advanced techniques look into
* another sample - unit_test_alt_sample.cpp.
*
* ===========================================================================
*/

#include <ncbi_pch.hpp>

#include <corelib/ncbi_system.hpp>
#include <corelib/ncbiapp.hpp>

// This header must be included before all Boost.Test headers if there are any
#include <corelib/test_boost.hpp>

#include <corelib/test_boost.hpp>

#include <objtools/unit_test_util/unit_test_util.hpp>

#include <objtools/discrepancy_report/hDiscRep_tests.hpp>
#include <objtools/discrepancy_report/hDiscRep_output.hpp>
#include <objtools/discrepancy_report/hDiscRep_config.hpp>
#include <objtools/discrepancy_report/hUtilib.hpp>


const char* sc_TestEntryCollidingLocusTags ="Seq-entry ::= seq {\
     id {\
       local str \"LocusCollidesWithLocusTag\" } ,\
     inst {\
       repr raw ,\
       mol dna ,\
       length 24 ,\
       seq-data\
         iupacna \"AATTGGCCAANNAATTGGCCAANN\" } ,\
     annot {\
       {\
         data\
           ftable {\
             {\
               data\
                 gene {\
                   locus \"foo\" ,\
                   locus-tag \"foo\" } ,\
               location\
                 int {\
                   from 0 ,\
                   to 4 ,\
                   strand plus ,\
                   id\
                     local str \"LocusCollidesWithLocusTag\" } } ,\
             {\
               data\
                 gene {\
                   locus \"bar\" ,\
                   locus-tag \"foo\" } ,\
               location\
                 int {\
                   from 5 ,\
                   to 9 ,\
                   strand plus ,\
                   id\
                     local str \"LocusCollidesWithLocusTag\" } } ,\
             {\
               data\
                 gene {\
                   locus \"bar\" ,\
                   locus-tag \"baz\" } ,\
               location\
                 int {\
                   from 10 ,\
                   to 14 ,\
                   strand plus ,\
                   id\
                     local str \"LocusCollidesWithLocusTag\" } } ,\
             {\
               data\
                 gene {\
                   locus \"quux\" ,\
                   locus-tag \"baz\" } ,\
               location\
                 int {\
                   from 15 ,\
                   to 19 ,\
                   strand plus ,\
                   id\
                     local str \"LocusCollidesWithLocusTag\" } } } } } }\
 ";


BEGIN_NCBI_SCOPE
USING_NCBI_SCOPE;
USING_SCOPE(objects);
USING_SCOPE(DiscRepNmSpc);
USING_SCOPE(unit_test_util);

static CRef <CRepConfig> config(new CRepConfig);
NCBITEST_AUTO_INIT()
{
    // Your application initialization code here (optional)
    cout << "Initialization function executed" << endl;
    config->InitParams(0);
}


NCBITEST_INIT_CMDLINE(arg_desc)
{
    // Describe command line parameters that we are going to use
    arg_desc->AddFlag
        ("enable_TestTimeout",
         "Run TestTimeout test, which is artificially disabled by default in"
         "order to avoid unwanted failure during the daily automated builds.");
}


NCBITEST_AUTO_FINI()
{
    // Your application finalization code here (optional)
    cout << "Finalization function executed" << endl;
}

void RunTest(CRef <CRepConfig>& config, CRef <CClickableItem>& c_item)
{
   config->CollectTests();
   config->Run();
   CDiscRepOutput output_obj;
   output_obj.Export(c_item);
};

void CheckReport(CRef <CClickableItem>& c_item, const string& msg)
{
   if (c_item.Empty() || (c_item->description).empty()) {
      NCBITEST_CHECK_MESSAGE(!c_item.Empty(), "no report");
   }
   else {
/*
     NCBITEST_CHECK_MESSAGE(c_item->item_list.size() == c_item->obj_list.size(),
              "The sizes of item_list and obj_list are not equal");
*/
     NCBITEST_CHECK_MESSAGE(c_item->description == msg,
              "Test report is incorrect: " + c_item->description);
   }
};

BOOST_AUTO_TEST_CASE(DISC_FLATFILE_FIND_ONCALLER)
{
  CRef <CSeq_entry> entry = unit_test_util::BuildGoodNucProtSet();

   CRef<CObjectManager> objmgr = CObjectManager::GetInstance();
   CRef <CScope> scope(new CScope(*objmgr));
   CSeq_entry_Handle seh = scope->AddTopLevelSeqEntry(*entry);
   config->SetTopLevelSeqEntry(&seh);
   
   NON_CONST_ITERATE (list <CRef <CSeq_entry> >, it, entry->SetSet().SetSeq_set()){
      if ( (*it)->SetSeq().IsAa()) {
         (*it)->SetSeq().SetInst().SetSeq_data().SetIupacaa().Set("SHAEMNVV");
      }
   }

   config->SetArg("e", "DISC_FLATFILE_FIND_ONCALLER");
   CRef <CClickableItem> c_item(0);
   RunTest(config, c_item);
   CheckReport(c_item, "1 object contains haem");
};

BOOST_AUTO_TEST_CASE(DUP_GENPRODSET_PROTEIN)
{
   CRef <CSeq_entry> entry = unit_test_util::BuildGoodGenProdSet();

   // add cdregion
   CRef <CSeq_entry> 
     genomic_entry = unit_test_util::GetGenomicFromGenProdSet(entry); 
   const list <CRef <CSeq_annot> >& annots = genomic_entry->GetSeq().GetAnnot();
   CRef <CSeq_feat> new_feat(new CSeq_feat);
   ITERATE(list <CRef <CSeq_feat> >, it,
                        (*(annots.begin()))->GetData().GetFtable()) {
      if ( (*it)->GetData().IsCdregion()) {
         new_feat->SetData().SetCdregion().Assign(((*it)->GetData().GetCdregion()));
         new_feat->SetLocation().SetInt().SetId().Assign(*(genomic_entry->GetSeq().GetId().front()));
         new_feat->SetLocation().SetInt().SetFrom(0);
         new_feat->SetLocation().SetInt().SetTo(genomic_entry->GetSeq().GetInst().GetLength()-1);
         new_feat->SetProduct().Assign((*it)->GetProduct());
         unit_test_util::AddFeat(new_feat, genomic_entry );
         break;
      }
   }

   CRef<CObjectManager> objmgr = CObjectManager::GetInstance();
   CRef <CScope> scope(new CScope(*objmgr));
   CSeq_entry_Handle seh = scope->AddTopLevelSeqEntry(*entry);
   config->SetTopLevelSeqEntry(&seh);
   config->SetArg("e", "DUP_GENPRODSET_PROTEIN");
   CRef <CClickableItem> c_item(0);
   RunTest(config, c_item);
   CheckReport(c_item, "2 coding regions have protein ID prot");
};

BOOST_AUTO_TEST_CASE(MISSING_GENPRODSET_TRANSCRIPT_ID)
{
   CRef <CSeq_entry> entry = unit_test_util::BuildGoodGenProdSet();
   CRef<CObjectManager> objmgr = CObjectManager::GetInstance();
   CRef <CScope> scope(new CScope(*objmgr));
   CSeq_entry_Handle seh = scope->AddTopLevelSeqEntry(*entry);
 
   // good gen-prod-set
   config->SetTopLevelSeqEntry(&seh);
   config->SetArg("e", "MISSING_GENPRODSET_TRANSCRIPT_ID");
   CRef <CClickableItem> c_item(0);   
   RunTest(config, c_item); 
   if (!c_item.Empty()) {
      NCBITEST_CHECK_MESSAGE(c_item.Empty(), 
               "Wrong report: " + c_item->description);
   }

   // reset Product of mRNA
   CRef <CSeq_entry> 
        genomic_entry = unit_test_util::GetGenomicFromGenProdSet(entry);
   list <CRef <CSeq_annot> >& annots = genomic_entry->SetSeq().SetAnnot();
   list <CRef <CSeq_feat> > & feats =(*(annots.begin()))->SetData().SetFtable();
   NON_CONST_ITERATE(list <CRef <CSeq_feat> >, it, 
                        (*(annots.begin()))->SetData().SetFtable()) {
      if ( (*it)->SetData().GetSubtype() == CSeqFeatData::eSubtype_mRNA) {
         (*it)->ResetProduct(); 
         break;
      }
   }
   
   objmgr.Reset(CObjectManager::GetInstance().GetPointer());
   scope.Reset(new CScope(*objmgr));
   seh = scope->AddTopLevelSeqEntry(*entry);
   config->SetTopLevelSeqEntry(&seh);
   RunTest(config, c_item);
   CheckReport(c_item, "1 mRNA has missing transcript ID.");
};


BOOST_AUTO_TEST_CASE(MISSING_LOCUS_TAGS)
{
   CRef <CSeq_entry> entry (new CSeq_entry);
   CNcbiIstrstream istr(sc_TestEntryCollidingLocusTags);
   istr >> MSerial_AsnText >> *entry;

   CBioseq& seq = entry->SetSeq();
   list <CRef <CSeq_annot> >& annots = seq.SetAnnot();
   list <CRef <CSeq_feat> > & feats =(*(annots.begin()))->SetData().SetFtable();
   CGene_ref& gene_ref= (*(feats.begin()))->SetData().SetGene();
   string strtmp(kEmptyStr);
   gene_ref.SetLocus_tag("");

   CRef<CObjectManager> objmgr = CObjectManager::GetInstance();
   CRef <CScope> scope(new CScope(*objmgr));
   CSeq_entry_Handle seh = scope->AddTopLevelSeqEntry(*entry);
   
   config->SetTopLevelSeqEntry(&seh);
   config->SetArg("e", "MISSING_LOCUS_TAGS");
   config->CollectTests();
   config->Run();
   CRef <CClickableItem> c_item(0);
   CDiscRepOutput output_obj;
   output_obj.Export(c_item);
   CheckReport(c_item, "1 gene has no locus tags.");
};

BOOST_AUTO_TEST_CASE(DISC_COUNT_NUCLEOTIDES)
{
   CRef <CSeq_entry> entry (new CSeq_entry);
   CNcbiIstrstream istr(sc_TestEntryCollidingLocusTags);
   istr >> MSerial_AsnText >> *entry;

   CRef<CObjectManager> objmgr = CObjectManager::GetInstance();
   CRef <CScope> scope(new CScope(*objmgr));
   CSeq_entry_Handle seh = scope->AddTopLevelSeqEntry(*entry);

   config->SetTopLevelSeqEntry(&seh);
   config->SetArg("e", "DISC_COUNT_NUCLEOTIDES");
   config->CollectTests();
   config->Run();
   CRef <CClickableItem> c_item(0);
   CDiscRepOutput output_obj;
   output_obj.Export(c_item);
   CheckReport(c_item, "1 nucleotide Bioseq is present.");
};


END_NCBI_SCOPE
