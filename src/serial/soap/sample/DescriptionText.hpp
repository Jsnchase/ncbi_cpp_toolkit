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

/// @DescriptionText.hpp
/// User-defined methods of the data storage class.
///
/// This file was originally generated by application DATATOOL
/// using the following specifications:
/// 'samplesoap.dtd'.
///
/// New methods or data members can be added to it if needed.
/// See also: DescriptionText_.hpp


#ifndef DESCRIPTIONTEXT_HPP
#define DESCRIPTIONTEXT_HPP


// generated includes
#include "DescriptionText_.hpp"

// generated classes

/////////////////////////////////////////////////////////////////////////////
class CDescriptionText : public CDescriptionText_Base
{
    typedef CDescriptionText_Base Tparent;
public:
    // constructor
    CDescriptionText(void);
    // destructor
    ~CDescriptionText(void);

private:
    // Prohibit copy constructor and assignment operator
    CDescriptionText(const CDescriptionText& value);
    CDescriptionText& operator=(const CDescriptionText& value);

};

/////////////////// CDescriptionText inline methods

// constructor
inline
CDescriptionText::CDescriptionText(void)
{
}


/////////////////// end of CDescriptionText inline methods



/*
* ===========================================================================
*
* $Log$
* Revision 1.1  2004/07/06 13:48:49  gouriano
* Initial revision: sample SOAP server and client
*
*
* ===========================================================================
*/

#endif // DESCRIPTIONTEXT_HPP
/* Original file checksum: lines: 86, chars: 2487, CRC32: a1b9b949 */
