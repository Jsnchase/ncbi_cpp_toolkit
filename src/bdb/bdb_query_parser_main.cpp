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
 * Author: Anatoliy Kuznetsov
 *
 * File Description:
 *	  Query parser entry point. 
 *    Glues together code generated by BISON, lexical tokenizer, 
 *    error processing, etc.
 *
 */

#include <ncbi_pch.hpp>
#include <bdb/bdb_query.hpp>
#include <bdb/bdb_expt.hpp>
#include <bdb/bdb_query_parser.hpp>
#include <util/resource_pool.hpp>


USING_NCBI_SCOPE;

/* extern "C" */ static int yyerror (const char *s);
/* extern "C" */ static int yylex (CBDB_Query::TQueryClause** lvalp, void* param);

BEGIN_NCBI_SCOPE

/// Query tree garbage collection functor.
///
/// Finds nodes in the query tree and removes them from the 
/// memory pool. All lost nodes are deleted by the pool object.
///
/// @internal

class CQueryTreeCleanPoolFunc
{
public: 
    CQueryTreeCleanPoolFunc(CResourcePool<CBDB_Query::TQueryClause>& mem_pool)
    : m_Pool(mem_pool)
    {}
  
    ETreeTraverseCode 
    operator()(CTreeNode<CBDB_QueryNode>& tr, int delta)
    {
        if (delta == 0 || delta == 1) {
            m_Pool.Forget(&tr);
        }
        return eTreeTraverse;
    }
private:
    CResourcePool<CBDB_Query::TQueryClause>&  m_Pool;
};


/// Class passed to Lex/Bison (yylex/yyparse) functions.
/// Works as a glue to pass all the information 
/// (both parsed stream and output syntactic tree)
///
/// @internal

class CBDB_QueryParserEnvironment
{
public:
    CBDB_QueryParserEnvironment(const char* query_str)
    : m_Query(query_str),
      m_Ptr(query_str),
      m_QueryClause(0)
    {}

    ~CBDB_QueryParserEnvironment()
    {
        if (m_QueryClause) {
            // Sync the memory pool not to delete the in-tree nodes
            CQueryTreeCleanPoolFunc func(m_NodePool);
            TreeDepthFirstTraverse(*m_QueryClause, func);

            delete m_QueryClause;
        }
    }

    const char*  GetQueryBuffer() const { return m_Query; }
    const char*  GetBufPtr() const { return m_Ptr; }
    void SetBufPtr(const char* ptr) { m_Ptr = ptr; }

    int GetChar()
    {
        int r = *m_Ptr++;
        return r;
    }

    /// Skip number of characters
    void Skip(int num)
    {
        m_Ptr += num;
    }

    void UnGetChar() { --m_Ptr; }

    void AttachQueryClause(CBDB_Query::TQueryClause* qc)
    {
        m_QueryClause = qc;
    }

    CBDB_Query::TQueryClause* GetQueryClause() { return m_QueryClause; }

    /// Return query clause.
    ///
    /// Caller is responsible for deleteion
    CBDB_Query::TQueryClause* DetachQueryClause() 
    { 
        if (m_QueryClause) {
            // Sync the memory pool
            CQueryTreeCleanPoolFunc func(m_NodePool);
            TreeDepthFirstTraverse(*m_QueryClause, func);

            CBDB_Query::TQueryClause* qc = m_QueryClause;
            m_QueryClause = 0;

            return qc; 
        }
        return 0;
    }

    void AddNodeToPool(CBDB_Query::TQueryClause* qnode)
    {
        m_NodePool.Put(qnode);
    }

    CResourcePool<CBDB_Query::TQueryClause>& GetPool()
    {
        return m_NodePool;
    }

private:
    const char*    m_Query;   ///< Request buffer. (Source for the scanner)
    const char*    m_Ptr;     ///< Current position in the request buffer

    /// Query clause tree. This is the result of the statement parsing.
    CBDB_Query::TQueryClause*               m_QueryClause;
    /// Memory pool of query clause tree nodes. 
    /// Used to avoid memory leaks when BISON shifts stack and calls
    /// yyerror (throws a C++ exception)
    CResourcePool<CBDB_Query::TQueryClause> m_NodePool;
};





END_NCBI_SCOPE


#include "bdb_query_bison.tab.c"
#include "bdb_query_lexer.inc"

/// Called by yyparse on error 
static
int yyerror (const char *s) 
{
    BDB_THROW(eQuerySyntaxError, s);
    return 0;
}

BEGIN_NCBI_SCOPE

void BDB_ParseQuery(const char* query_str, CBDB_Query* query)
{
    CBDB_QueryParserEnvironment env(query_str);
    /*int res = */ yyparse((void*) &env);
    CBDB_Query::TQueryClause* qc = env.GetQueryClause();

    if (qc) {
        qc = env.DetachQueryClause();
        query->SetQueryClause(qc);
    } else {

        // Check if we have only one token node created by the tokenizer
        // means it's just a single word request (legal)

        CResourcePool<CBDB_Query::TQueryClause>& pool = env.GetPool();
        CResourcePool<CBDB_Query::TQueryClause>::TPoolList& vec = 
                                                        pool.GetFreeList();

        // the only node in pool
        if (vec.size() == 1) {
            CBDB_Query::TQueryClause* qcl = vec[0];
            query->SetQueryClause(qcl);
            pool.ForgetAll();
        }
    }
}


END_NCBI_SCOPE
