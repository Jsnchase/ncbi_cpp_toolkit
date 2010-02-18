#ifndef CPPCORE__DBAPI__SIMPLE__SDBAPI_IMPL__HPP
#define CPPCORE__DBAPI__SIMPLE__SDBAPI_IMPL__HPP

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
 * Author:  Pavel Ivanov
 *
 * File Description:
 *
 */

#include <corelib/ncbiobj.hpp>
#include <dbapi/dbapi.hpp>

#include <dbapi/simple/sdbapi.hpp>

BEGIN_NCBI_SCOPE

typedef CObjectFor<IConnection*>  TConnHolder;

class CDatabaseImpl : public CObject
{
public:
    CDatabaseImpl(const CDatabaseImpl& other);
    CDatabaseImpl(const CSDB_ConnectionParam& params);
    ~CDatabaseImpl(void);

    bool IsOpen() const;
    void Close();

    IConnection* GetConnection(void);

private:
    CRef<TConnHolder>  m_Conn;
};


class CBulkInsertImpl : public CObject
{
public:
    CBulkInsertImpl(CDatabaseImpl* db_impl,
                    const string&  tableName,
                    int            autoflush);
    ~CBulkInsertImpl(void);

    void SetHints(CTempString hints);
    void AddHint(IBulkInsert::EHints hint, unsigned int value);
    void AddOrderHint(CTempString columns);

    void Bind(int col, ESDB_Type type);
    void EndRow(void);
    void Complete(void);

    void WriteNull(void);
    template <class T>
    void WriteVal(const T& val);

private:
    void x_CheckCanWrite(int col);
    void x_CheckWriteStarted(void);


    CRef<CDatabaseImpl> m_DBImpl;
    IBulkInsert*        m_BI;
    vector<CVariant>    m_Cols;
    int                 m_Autoflush;
    int                 m_RowsWritten;
    int                 m_ColsWritten;
    bool                m_WriteStarted;
};


struct SQueryParamInfo
{
    ESP_ParamType   type;
    CVariant*       value;
    CQuery::CField* field;

    SQueryParamInfo(void);
};

class CQueryImpl: public CObject
{
public:
    CQueryImpl(CDatabaseImpl* db_impl);
    ~CQueryImpl(void);

    template <class T>
    void SetParameter(CTempString name, const T& value, ESDB_Type type, ESP_ParamType param_type);
    void SetNullParameter(CTempString name, ESDB_Type type, ESP_ParamType param_type);
    const CQuery::CField& GetParameter(CTempString name);
    void ClearParameter(CTempString name);
    void ClearParameters(void);

    void SetSql(CTempString sql);
    void Execute(void);
    void ExecuteSP(CTempString sp);
    bool HasMoreResultSets(void);
    void PurgeResults(void);
    void BeginNewRS(void);
    void Next(void);
    const CQuery::CField& GetColumn(const CDBParamVariant& col) const;
    const CVariant& GetFieldValue(const CQuery::CField& field);
    bool IsFinished(void) const;

    void SetIgnoreBounds(bool is_ignore);
    unsigned int GetResultSetNo(void);
    unsigned int GetRowNo(void);
    int GetRowCount(void);
    int GetStatus(void);
    int GetTotalColumns(void);
    string GetColumnName(unsigned int col);
    ESDB_Type GetColumnType(unsigned int col);

private:
    void x_CheckCanWork(bool need_rs = false) const;
    void x_SetOutParameter(const string& name, const CVariant& value);
    bool x_Fetch(void);
    void x_InitBeforeExec(void);
    void x_InitRSFields(void);
    void x_Close(void);


    typedef map<string, SQueryParamInfo>        TParamsMap;
    typedef map<string, int>                    TColNumsMap;
    typedef vector< AutoPtr<CQuery::CField> >   TFields;

    CRef<CDatabaseImpl> m_DBImpl;
    IStatement*         m_Stmt;
    ICallableStatement* m_CallStmt;
    TParamsMap          m_Params;
    string              m_Sql;
    bool                m_IgnoreBounds;
    IResultSet*         m_CurRS;
    bool                m_RSBeginned;
    bool                m_RSFinished;
    unsigned int        m_CurRSNo;
    unsigned int        m_CurRowNo;
    int                 m_RowCount;
    int                 m_Status;
    TColNumsMap         m_ColNums;
    TFields             m_Fields;
};

END_NCBI_SCOPE

#endif  // CPPCORE__DBAPI__SIMPLE__SDBAPI_IMPL__HPP
