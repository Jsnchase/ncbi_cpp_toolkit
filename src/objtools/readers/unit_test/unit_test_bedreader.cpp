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
* Author:  Mostly Mike Kornbluh, NCBI.
*          Customizations by Frank Ludwig, NCBI.
*
* File Description:
*   GFF3 reader unit test.
*
* ===========================================================================
*/

#include <ncbi_pch.hpp>

#include <corelib/ncbi_system.hpp>
#include <corelib/ncbiapp.hpp>
#include <corelib/ncbifile.hpp>

#include <objtools/readers/bed_reader.hpp>
#include "error_logger.hpp"

#include <cstdio>

// This header must be included before all Boost.Test headers if there are any
#include <corelib/test_boost.hpp>


USING_NCBI_SCOPE;
USING_SCOPE(objects);

//  ============================================================================
//  Customization data:
const string extInput("bed");
const string extOutput("asn");
const string extErrors("errors");
const string dirTestFiles("bedreader_test_cases");
// !!!
// !!! Must also customize reader type in sRunTest !!!
// !!!
//  ============================================================================

struct STestInfo {
    CFile mInFile;
    CFile mOutFile;
    CFile mErrorFile;
};
typedef string TTestName;
typedef map<TTestName, STestInfo> TTestNameToInfoMap;

class CTestNameToInfoMapLoader {
public:
    CTestNameToInfoMapLoader(
        TTestNameToInfoMap * pTestNameToInfoMap,
        const string& extInput,
        const string& extOutput,
        const string& extErrors)
        : m_pTestNameToInfoMap(pTestNameToInfoMap),
          mExtInput(extInput),
          mExtOutput(extOutput),
          mExtErrors(extErrors)
    { }

    void operator()( const CDirEntry & dirEntry ) {
        const static size_t kInvalidFileNumber = numeric_limits<size_t>::max();

        if( ! dirEntry.IsFile() ) {
            return;
        }

        CFile file(dirEntry);
        string name = file.GetName();
        if (NStr::EndsWith(name, ".txt")  ||  NStr::StartsWith(name, ".")) {
            return;
        }

        // extract info from the file name
        const string sFileName = file.GetName();
        vector<CTempString> vecFileNamePieces;
        NStr::Tokenize( sFileName, ".", vecFileNamePieces );
        BOOST_REQUIRE(vecFileNamePieces.size() == 2);

        CTempString tsTestName = vecFileNamePieces[0];
        BOOST_REQUIRE(!tsTestName.empty());
        CTempString tsFileType = vecFileNamePieces[1];
        BOOST_REQUIRE(!tsFileType.empty());
            
        STestInfo & test_info_to_load =
            (*m_pTestNameToInfoMap)[vecFileNamePieces[0]];

        // figure out what type of file we have and set appropriately
        if (tsFileType == mExtInput) {
            BOOST_REQUIRE( test_info_to_load.mInFile.GetPath().empty() );
            test_info_to_load.mInFile = file;
        } 
        else if (tsFileType == mExtOutput) {
            BOOST_REQUIRE( test_info_to_load.mOutFile.GetPath().empty() );
            test_info_to_load.mOutFile = file;
        } 
        else if (tsFileType == mExtErrors) {
            BOOST_REQUIRE( test_info_to_load.mErrorFile.GetPath().empty() );
            test_info_to_load.mErrorFile = file;
        } 

        else {
            BOOST_FAIL("Unknown file type " << sFileName << ".");
        }
    }

private:
    // raw pointer because we do NOT own this
    TTestNameToInfoMap * m_pTestNameToInfoMap;
    string mExtInput;
    string mExtOutput;
    string mExtErrors;
};

void sRunTest(const string &sTestName, const STestInfo & testInfo)
{
    cerr << "Testing " << testInfo.mInFile.GetName() << " against " <<
        testInfo.mOutFile.GetName() << " and " <<
        testInfo.mErrorFile.GetName() << endl;

    string logName = CDirEntry::GetTmpName();
    CErrorLogger logger(logName);

    CBedReader reader(CBedReader::fThreeFeatFormat);
    CNcbiIfstream ifstr(testInfo.mInFile.GetPath().c_str());

    typedef vector<CRef<CSeq_annot> > ANNOTS;
    ANNOTS annots;
    try {
        reader.ReadSeqAnnots(annots, ifstr, &logger);
    }
    catch (...) {
        BOOST_ERROR("Error: " << sTestName << " failed during conversion.");
        ifstr.close();
        return;
    }

    string resultName = CDirEntry::GetTmpName();
    CNcbiOfstream ofstr(resultName.c_str());
    for (ANNOTS::iterator cit = annots.begin(); cit != annots.end(); ++cit){
        ofstr << MSerial_AsnText << **cit;
        ofstr.flush();
    }
    ifstr.close();
    ofstr.close();

    bool success = testInfo.mOutFile.CompareTextContents(resultName, CFile::eIgnoreWs);
    if (!success) {
        CDirEntry(resultName).Remove();
        CDirEntry(logName).Remove();
        BOOST_ERROR("Error: " << sTestName << " failed due to post processing diffs.");
    }
    CDirEntry(resultName).Remove();

    success = testInfo.mErrorFile.CompareTextContents(logName, CFile::eIgnoreWs);
    CDirEntry(logName).Remove();
    if (!success) {
        BOOST_ERROR("Error: " << sTestName << " failed due to error handling diffs.");
    }
};

NCBITEST_AUTO_INIT()
{
}


NCBITEST_INIT_CMDLINE(arg_descrs)
{
    arg_descrs->AddDefaultKey("test-dir", "DIRECTORY",
        "Set the root directory under which all test files can be found.",
        CArgDescriptions::eDirectory,
        dirTestFiles );
}



NCBITEST_AUTO_FINI()
{
}

BOOST_AUTO_TEST_CASE(RunTests)
{
    const CArgs& args = CNcbiApplication::Instance()->GetArgs();
    const vector<string> kEmptyStringVec;
    TTestNameToInfoMap testNameToInfoMap;
    CTestNameToInfoMapLoader testInfoLoader(
        &testNameToInfoMap, extInput, extOutput, extErrors);
    CDir test_cases_dir( args["test-dir"].AsDirectory() );

    BOOST_REQUIRE_MESSAGE( test_cases_dir.IsDir(), 
        "Cannot find dir: " << test_cases_dir.GetPath() );

    FindFilesInDir(
        test_cases_dir,
        kEmptyStringVec,
        kEmptyStringVec,
        testInfoLoader,
        fFF_Default | fFF_Recursive );

    ITERATE(TTestNameToInfoMap, name_to_info_it, testNameToInfoMap) {
        const string & sName = name_to_info_it->first;
        const STestInfo & testInfo = name_to_info_it->second;
        cout << "Verifying: " << sName << endl;
        BOOST_REQUIRE_MESSAGE( testInfo.mInFile.Exists(),
            "GVF file does not exist: " << testInfo.mInFile.GetPath() );
        BOOST_REQUIRE_MESSAGE( testInfo.mOutFile.Exists(),
            "ASN file does not exist: " << testInfo.mOutFile.GetPath() );
    }
    ITERATE(TTestNameToInfoMap, name_to_info_it, testNameToInfoMap) {
        const string & sName = name_to_info_it->first;
        const STestInfo & testInfo = name_to_info_it->second;
        
        cout << "Running test: " << sName << endl;

        BOOST_CHECK_NO_THROW(sRunTest(sName, testInfo));
    }
}
