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
 *   using the following specifications:
 *   'valid.asn'.
 */

// standard includes
#include <ncbi_pch.hpp>

// generated includes
#include <objects/valid/Field_rule.hpp>
#include <util/xregexp/regexp.hpp>

// generated classes

BEGIN_NCBI_SCOPE

BEGIN_objects_SCOPE // namespace ncbi::objects::

// destructor
CField_rule::~CField_rule(void)
{
}


bool CField_rule::DoesStringMatchRuleExpression(string str) const
{
    if (!IsSetMatch_expression()) {
        return true;
    }

    // because C Toolkit pattern match needs to escape '|' but C++ Toolkit does not
    string match = GetMatch_expression();
    NStr::ReplaceInPlace(match, "\\|", "|");
    NStr::ReplaceInPlace(match, "\\(", "(");
    NStr::ReplaceInPlace(match, "\\)", ")");

    CRegexp r(match);

    if (r.IsMatch (str)) {
        return true;
    } else {
        return false;
    }

}


END_objects_SCOPE // namespace ncbi::objects::

END_NCBI_SCOPE

/* Original file checksum: lines: 57, chars: 1723, CRC32: 7dc89f12 */
