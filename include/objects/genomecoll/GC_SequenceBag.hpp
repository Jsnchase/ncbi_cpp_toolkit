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

/// @file GC_SequenceBag.hpp
/// User-defined methods of the data storage class.
///
/// This file was originally generated by application DATATOOL
/// using the following specifications:
/// 'genome_collection.asn'.
///
/// New methods or data members can be added to it if needed.
/// See also: GC_SequenceBag_.hpp


#ifndef OBJECTS_GENOMECOLL_GC_SEQUENCEBAG_HPP
#define OBJECTS_GENOMECOLL_GC_SEQUENCEBAG_HPP


// generated includes
#include <objects/genomecoll/GC_SequenceBag_.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

/////////////////////////////////////////////////////////////////////////////
class NCBI_GENOME_COLLECTION_EXPORT CGC_SequenceBag : public CGC_SequenceBag_Base
{
    typedef CGC_SequenceBag_Base Tparent;
public:
    // constructor
    CGC_SequenceBag(void);
    // destructor
    ~CGC_SequenceBag(void);

private:
    // Prohibit copy constructor and assignment operator
    CGC_SequenceBag(const CGC_SequenceBag& value);
    CGC_SequenceBag& operator=(const CGC_SequenceBag& value);

};

/////////////////// CGC_SequenceBag inline methods

// constructor
inline
CGC_SequenceBag::CGC_SequenceBag(void)
{
}


/////////////////// end of CGC_SequenceBag inline methods


END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE


#endif // OBJECTS_GENOMECOLL_GC_SEQUENCEBAG_HPP
/* Original file checksum: lines: 86, chars: 2543, CRC32: 80a7160e */
