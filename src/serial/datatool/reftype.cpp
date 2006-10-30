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
* Author: Eugene Vasilchenko
*
* File Description:
*   Type reference definition
*/

#include <ncbi_pch.hpp>
#include <serial/datatool/reftype.hpp>
#include <serial/datatool/unitype.hpp>
#include <serial/datatool/statictype.hpp>
#include <serial/datatool/typestr.hpp>
#include <serial/datatool/value.hpp>
#include <serial/datatool/module.hpp>
#include <serial/datatool/exceptions.hpp>
#include <serial/datatool/blocktype.hpp>
#include <serial/datatool/enumtype.hpp>
#include <serial/datatool/srcutil.hpp>
#include <serial/classinfo.hpp>
#include <serial/serialimpl.hpp>

BEGIN_NCBI_SCOPE

CReferenceDataType::CReferenceDataType(const string& n)
    : m_UserTypeName(n)
{
}

void CReferenceDataType::PrintASN(CNcbiOstream& out, int /*indent*/) const
{
    out << CDataTypeModule::ToAsnName(m_UserTypeName);
}

string CReferenceDataType::GetSpecKeyword(void) const
{
    return string("T:") + GetUserTypeName();
}

// XML schema generator submitted by
// Marc Dumontier, Blueprint initiative, dumontier@mshri.on.ca
// modified by Andrei Gourianov, gouriano@ncbi
void CReferenceDataType::PrintXMLSchema(CNcbiOstream& out,
    int indent, bool contents_only) const
{
    string tag(XmlTagName());
    string userType(UserTypeXmlTagName());

    if (tag == userType || (GetEnforcedStdXml() && contents_only)) {
        PrintASNNewLine(out,indent) <<
            "<xs:element ref=\"" << userType << "\"";
        if (GetDataMember() && GetDataMember()->Optional()) {
            out << " minOccurs=\"0\"";
        }
        out << "/>";
    } else {
        if (!contents_only) {
            PrintASNNewLine(out,indent++) << "<xs:element name=\"" << tag << "\"";
            if (GetDataMember() && GetDataMember()->Optional()) {
                out << " minOccurs=\"0\"";
            }
            out << ">";
            PrintASNNewLine(out,indent++) << "<xs:complexType>";
            PrintASNNewLine(out,indent++) << "<xs:sequence>";
        }
        PrintASNNewLine(out,indent) << "<xs:element ref=\"" << userType << "\"/>";
        if (!contents_only) {
            PrintASNNewLine(out,--indent) << "</xs:sequence>";
            PrintASNNewLine(out,--indent) << "</xs:complexType>";
            PrintASNNewLine(out,--indent) << "</xs:element>";
        }
    }
}

void CReferenceDataType::PrintDTDElement(CNcbiOstream& out, bool contents_only) const
{
    string tag(XmlTagName());
    string userType(UserTypeXmlTagName());
    const CUniSequenceDataType* uniType = 
        dynamic_cast<const CUniSequenceDataType*>(GetParentType());

    if (tag == userType || (GetEnforcedStdXml() && uniType)) {
        const CDataType* realType = ResolveOrNull();
        if (realType) {
            realType->PrintDTDElement(out, contents_only);
        }
        return;
    }
    out <<
        "\n<!ELEMENT "<<XmlTagName()<<" ("<<UserTypeXmlTagName()<<")>";
}

void CReferenceDataType::PrintDTDExtra(CNcbiOstream& out) const
{
    string tag(XmlTagName());
    string userType(UserTypeXmlTagName());
    const CUniSequenceDataType* uniType = 
        dynamic_cast<const CUniSequenceDataType*>(GetParentType());

    if (tag == userType || (GetEnforcedStdXml() && uniType)) {
        const CDataType* realType = ResolveOrNull();
        if (realType) {
            realType->PrintDTDExtra(out);
        }
    }
}

void CReferenceDataType::FixTypeTree(void) const
{
    CParent::FixTypeTree();
    CDataType* resolved = ResolveOrNull();
    if ( resolved )
        resolved->AddReference(this);
}

bool CReferenceDataType::CheckType(void) const
{
    try {
        ResolveLocal(m_UserTypeName);
        return true;
    }
    catch ( CNotFoundException& exc) {
        Warning("Unresolved type: " + m_UserTypeName + ": " + exc.what());
    }
    return false;
}

bool CReferenceDataType::CheckValue(const CDataValue& value) const
{
    CDataType* resolved = ResolveOrNull();
    if ( !resolved )
        return false;
    return resolved->CheckValue(value);
}

TTypeInfo CReferenceDataType::GetRealTypeInfo(void)
{
    CDataType* dataType = ResolveOrThrow();
    if ( dynamic_cast<CDataMemberContainerType*>(dataType) ||
         dynamic_cast<CEnumDataType*>(dataType) )
        return dataType->GetRealTypeInfo();
    return CParent::GetRealTypeInfo();
}

CTypeInfo* CReferenceDataType::CreateTypeInfo(void)
{
    CClassTypeInfo* info = CClassInfoHelper<AnyType>::CreateClassInfo(m_UserTypeName.c_str());
    info->SetImplicit();
    CMemberInfo* mem = info->AddMember("", 0, ResolveOrThrow()->GetTypeInfo());
    if (GetDataMember() && GetDataMember()->NoPrefix()) {
        mem->SetNoPrefix();
    }
    if ( GetParentType() == 0 ) {
        // global
        info->SetModuleName(GetModule()->GetName());
    }
    return info;
}

TObjectPtr CReferenceDataType::CreateDefault(const CDataValue& value) const
{
    return ResolveOrThrow()->CreateDefault(value);
}

string CReferenceDataType::GetDefaultString(const CDataValue& value) const
{
    return ResolveOrThrow()->GetDefaultString(value);
}

AutoPtr<CTypeStrings> CReferenceDataType::GenerateCode(void) const
{
    return CParent::GenerateCode();
}

AutoPtr<CTypeStrings> CReferenceDataType::GetFullCType(void) const
{
    const CDataType* resolved = ResolveOrThrow();
    if ( resolved->Skipped() )
        return resolved->GetFullCType();
    else
        return resolved->GetRefCType();
}

CDataType* CReferenceDataType::ResolveOrNull(void) const
{
    try {
        return ResolveLocal(m_UserTypeName);
    }
    catch ( CNotFoundException& /* ignored */) {
    }
    return 0;
}

CDataType* CReferenceDataType::ResolveOrThrow(void) const
{
    try {
        return ResolveLocal(m_UserTypeName);
    }
    catch ( CNotFoundException& exc) {
        NCBI_RETHROW_SAME(exc, LocationString());
    }
    // ASSERT("Not reached" == 0);
    return static_cast<CDataType*>(NULL);  // Happy compiler fix
}

CDataType* CReferenceDataType::Resolve(void)
{
    CDataType* resolved = ResolveOrNull();
    if ( !resolved )
        return this;
    return resolved;
}

const CDataType* CReferenceDataType::Resolve(void) const
{
    CDataType* resolved = ResolveOrNull();
    if ( !resolved )
        return this;
    return resolved;
}

END_NCBI_SCOPE

/*
* ---------------------------------------------------------------------------
* $Log$
* Revision 1.37  2006/10/30 18:15:40  gouriano
* Added writing data specification in internal format
*
* Revision 1.36  2006/08/03 19:16:54  gouriano
* Get rid of crashes when parsing incomplete DTD or schema
*
* Revision 1.35  2006/06/19 17:34:06  gouriano
* Redesigned generation of XML schema
*
* Revision 1.34  2006/06/05 15:33:14  gouriano
* Implemented local elements when parsing XML schema
*
* Revision 1.33  2006/05/16 14:30:13  gouriano
* Corrected generation of ASN spec - to make sure it is valid
*
* Revision 1.32  2005/02/02 19:08:36  gouriano
* Corrected DTD generation
*
* Revision 1.31  2004/08/03 15:11:24  gouriano
* Check if GetDataMember() returns 0 before using it
*
* Revision 1.30  2004/05/19 17:25:42  gouriano
* Corrected reading of containers when the code was generated by DTD
*
* Revision 1.29  2004/05/17 21:03:14  gorelenk
* Added include of PCH ncbi_pch.hpp
*
* Revision 1.28  2003/06/16 14:41:05  gouriano
* added possibility to convert DTD to XML schema
*
* Revision 1.27  2003/05/14 14:42:22  gouriano
* added generation of XML schema
*
* Revision 1.26  2003/03/10 18:55:18  gouriano
* use new structured exceptions (based on CException)
*
* Revision 1.25  2001/12/03 14:49:04  juran
* Eliminate warning.
*
* Revision 1.24  2001/05/17 15:07:12  lavr
* Typos corrected
*
* Revision 1.23  2000/11/20 17:26:33  vasilche
* Fixed warnings on 64 bit platforms.
* Updated names of config variables.
*
* Revision 1.22  2000/11/15 20:34:55  vasilche
* Added user comments to ENUMERATED types.
* Added storing of user comments to ASN.1 module definition.
*
* Revision 1.21  2000/11/14 21:41:26  vasilche
* Added preserving of ASN.1 definition comments.
*
* Revision 1.20  2000/11/08 17:02:52  vasilche
* Added generation of modular DTD files.
*
* Revision 1.19  2000/11/07 17:26:26  vasilche
* Added module names to CTypeInfo and CEnumeratedTypeValues
* Added possibility to set include directory for whole module
*
* Revision 1.18  2000/11/01 20:38:59  vasilche
* OPTIONAL and DEFAULT are not permitted in CHOICE.
* Fixed code generation for DEFAULT.
*
* Revision 1.17  2000/09/26 17:38:27  vasilche
* Fixed incomplete choiceptr implementation.
* Removed temporary comments.
*
* Revision 1.16  2000/09/18 20:00:29  vasilche
* Separated CVariantInfo and CMemberInfo.
* Implemented copy hooks.
* All hooks now are stored in CTypeInfo/CMemberInfo/CVariantInfo.
* Most type specific functions now are implemented via function pointers instead of virtual functions.
*
* Revision 1.15  2000/08/25 15:59:24  vasilche
* Renamed directory tool -> datatool.
*
* Revision 1.14  2000/07/11 20:36:29  vasilche
* Removed unnecessary generation of namespace references for enum members.
* Removed obsolete methods.
*
* Revision 1.13  2000/07/10 17:32:00  vasilche
* Macro arguments made more clear.
* All old ASN stuff moved to serialasn.hpp.
* Changed prefix of enum info functions to GetTypeInfo_enum_.
*
* Revision 1.12  2000/06/16 16:31:40  vasilche
* Changed implementation of choices and classes info to allow use of the same classes in generated and user written classes.
*
* Revision 1.11  2000/05/24 20:09:29  vasilche
* Implemented DTD generation.
*
* Revision 1.10  2000/04/07 19:26:33  vasilche
* Added namespace support to datatool.
* By default with argument -oR datatool will generate objects in namespace
* NCBI_NS_NCBI::objects (aka ncbi::objects).
* Datatool's classes also moved to NCBI namespace.
*
* Revision 1.9  2000/03/15 21:24:12  vasilche
* Error diagnostic about ambiguous types made more clear.
*
* Revision 1.8  2000/02/01 21:48:05  vasilche
* Added CGeneratedChoiceTypeInfo for generated choice classes.
* Removed CMemberInfo subclasses.
* Added support for DEFAULT/OPTIONAL members.
* Changed class generation.
* Moved datatool headers to include/internal/serial/tool.
*
* Revision 1.7  1999/12/29 16:01:51  vasilche
* Added explicit virtual destructors.
* Resolved overloading of InternalResolve.
*
* Revision 1.6  1999/12/03 21:42:12  vasilche
* Fixed conflict of enums in choices.
*
* Revision 1.5  1999/12/01 17:36:26  vasilche
* Fixed CHOICE processing.
*
* Revision 1.4  1999/11/16 15:41:16  vasilche
* Added plain pointer choice.
* By default we use C pointer instead of auto_ptr.
* Start adding initializers.
*
* Revision 1.3  1999/11/15 19:36:18  vasilche
* Fixed warnings on GCC
*
* ===========================================================================
*/
