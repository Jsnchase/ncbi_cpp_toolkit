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
 * Author:  Aleksandr Morgulis
 *
 * File Description:
 *   WindowMasker helper functions (implementation).
 *
 */

#include <ncbi_pch.hpp>

#include <vector>

#include "win_mask_util.hpp"

BEGIN_NCBI_SCOPE
USING_SCOPE(objects);

//------------------------------------------------------------------------------
bool CWinMaskUtil::consider( const CBioseq_Handle & bsh,
                             const CWinMaskConfig::CIdSet * ids,
                             const CWinMaskConfig::CIdSet * exclude_ids )
{
    if( (ids == 0 || ids->empty()) && 
        (exclude_ids == 0 || exclude_ids->empty()) ) {
        return true;
    }

    bool result = true;

    if( ids != 0 && !ids->empty() )
    {
        result = false;

        if( ids->find( bsh ) ) {
            result = true;
        }
    }

    if( exclude_ids != 0 && !exclude_ids->empty() )
    {
        if( exclude_ids->find( bsh ) ) {
            result = false;
        }
    }

    return result;
}

END_NCBI_SCOPE

/*
 * ========================================================================
 * $Log$
 * Revision 1.3  2005/11/21 16:49:15  morgulis
 * 1. Fixed a bug causing infinite loop in the case of empty genome.
 * 2. Added possibility to use substring matching with -ids and -exclude-ids
 *    options.
 *
 * Revision 1.2  2005/07/11 14:36:17  morgulis
 * Fixes for performance problems with large number of short sequences.
 * Windowmasker is now statically linked against object manager libs.
 *
 * Revision 1.1  2005/03/24 16:50:22  morgulis
 * -ids and -exclude-ids options can be applied in Stage 1 and Stage 2.
 *
 */
