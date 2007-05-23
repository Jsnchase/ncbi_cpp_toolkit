#ifndef DBAPI_DRIVER_IMPL___DBAPI_IMPL_CMD__HPP
#define DBAPI_DRIVER_IMPL___DBAPI_IMPL_CMD__HPP

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
 * Author:  Sergey Sikorskiy
 *
 * File Description:
 *
 */


#include <dbapi/driver/impl/dbapi_driver_utils.hpp>
#include <dbapi/driver/util/parameters.hpp>

BEGIN_NCBI_SCOPE

class CDB_LangCmd;
class CDB_RPCCmd;
class CDB_BCPInCmd;
class CDB_CursorCmd;
class CDB_SendDataCmd;

namespace impl
{

class NCBI_DBAPIDRIVER_EXPORT CCommand
{
public:
    virtual ~CCommand(void);

    void Release(void)
    {
        // Temporary solution ...
        delete this;
    }

    static CDB_Result* Create_Result(CResult& result);
};

/////////////////////////////////////////////////////////////////////////////
///
///  CBaseCmd::
///
/// Abstract base class for most "command" interface classes.
///

class CConnection;

class NCBI_DBAPIDRIVER_EXPORT CBaseCmd : public CCommand
{
    friend class ncbi::CDB_LangCmd;   // Because of AttachTo
    friend class ncbi::CDB_RPCCmd;    // Because of AttachTo
    friend class ncbi::CDB_BCPInCmd;  // Because of AttachTo
    friend class ncbi::CDB_CursorCmd; // Because of AttachTo

public:
    CBaseCmd(impl::CConnection* conn,
             const string& query,
             unsigned int nof_params);
    CBaseCmd(impl::CConnection* conn,
             const string& cursor_name,
             const string& query,
             unsigned int nof_params);
    // Destructor
    virtual ~CBaseCmd(void);

public:
    /// Send command to the server
    virtual bool Send(void);
    bool WasSent(void) const
    {
        return m_WasSent;
    }

    /// Cancel the command execution
    virtual bool Cancel(void);
    bool WasCanceled(void) const
    {
        return !WasSent();
    }

    /// Get result set
    virtual CDB_Result* Result(void);

    // There are two different strategy to implement this method ...
    // They should be eliminadted some day ...
    virtual bool HasMoreResults(void) const;

    // Check if command has failed
    virtual bool HasFailed(void) const;

    /// Get the number of rows affected by the command
    /// Special case:  negative on error or if there is no way that this
    ///                command could ever affect any rows (like PRINT).
    virtual int RowCount(void) const = 0;

    /// Dump the results of the command
    /// if result processor is installed for this connection, it will be called for
    /// each result set
    void DumpResults(void);

    /// Binding
    virtual bool Bind(unsigned int column_num, CDB_Object* param_ptr);
    virtual bool BindParam(const string& name, CDB_Object* param_ptr,
                           bool out_param = false);

    /// Setting
    virtual bool SetParam(const string& name, CDB_Object* param_ptr,
                          bool out_param = false);

    /// Add more text to the language command
    bool More(const string& query_text)
    {
        m_Query.append(query_text);
        return true;
    }

    //
    const string& GetQuery(void) const
    {
        return m_Query;
    }

    //
    const CDB_Params& GetParams(void) const
    {
        return m_Params;
    }
    CDB_Params& GetParams(void)
    {
        return m_Params;
    }

public:
    // BCP-related ...

    /// Complete batch -- to store all rows transferred by far in this batch
    /// into the table
    virtual bool CommitBCPTrans(void);

    /// Complete the BCP and store all rows transferred in last batch into
    /// the table
    virtual bool EndBCP(void);

protected:
    void DetachInterface(void);

    /// Set the "recompile before execute" flag for the stored proc
    void SetRecompile(bool recompile = true)
    {
        m_Recompile = recompile;
    }
    bool NeedToRecompile(void) const
    {
        return m_Recompile;
    }

    void SetWasSent(bool flag = true)
    {
        m_WasSent = flag;
    }
    void SetHasFailed(bool flag = true)
    {
        m_HasFailed = flag;
    }

    //
    CConnection& GetConnImpl(void) const
    {
        _ASSERT(m_ConnImpl);
        return *m_ConnImpl;
    }

protected:
    // Cursor-related interface ...

    /// Open the cursor.
    /// Return NULL if cursor resulted in no data.
    /// Throw exception on error.
    virtual CDB_Result* OpenCursor(void);

    /// Update the last fetched row.
    /// NOTE: the cursor must be declared for update in CDB_Connection::Cursor()
    virtual bool Update(const string& table_name, const string& upd_query);

    virtual bool UpdateTextImage(unsigned int item_num, CDB_Stream& data,
                                 bool log_it = true);

    virtual CDB_SendDataCmd* SendDataCmd(unsigned int item_num, size_t size,
                                         bool log_it = true);
    /// Delete the last fetched row.
    /// NOTE: the cursor must be declared for delete in CDB_Connection::Cursor()
    virtual bool Delete(const string& table_name);

    /// Close the cursor.
    /// Return FALSE if the cursor is closed already (or not opened yet)
    virtual bool CloseCursor(void);

protected:
    // Cursor-related methods

    //
    bool CursorIsOpen(void) const
    {
        return m_IsOpen;
    }
    void SetCursorOpen(bool flag = true)
    {
        m_IsOpen = flag;
    }

    //
    bool CursorIsDeclared(void) const
    {
        return m_IsDeclared;
    }
    void SetCursorDeclared(bool flag = true)
    {
        m_IsDeclared = flag;
    }

    //
    string GetCursorName(void) const
    {
        return m_CursorName;
    }

private:
    void AttachTo(CDB_LangCmd*      interface);
    void AttachTo(CDB_RPCCmd*       interface);
    void AttachTo(CDB_BCPInCmd*     interface);
    void AttachTo(CDB_CursorCmd*    interface);

    CInterfaceHook<CDB_LangCmd>     m_InterfaceLang;
    CInterfaceHook<CDB_RPCCmd>      m_InterfaceRPC;
    CInterfaceHook<CDB_BCPInCmd>    m_InterfaceBCPIn;
    CInterfaceHook<CDB_CursorCmd>   m_InterfaceCursor;

    impl::CConnection*  m_ConnImpl;
    string              m_Query;
    CDB_Params          m_Params;
    bool                m_Recompile; // Temporary. Should be deleted.
    bool                m_WasSent;
    bool                m_HasFailed;

private:
    // Cursor-related data ...
    bool            m_IsOpen;
    bool            m_IsDeclared;
    const string    m_CursorName;
};


////////////////////////////////////////////////////////////////////////////////
class NCBI_DBAPIDRIVER_EXPORT CSendDataCmd : public CCommand
{
    friend class ncbi::CDB_SendDataCmd; // Because of AttachTo

public:
    CSendDataCmd(size_t nof_bytes);
    virtual ~CSendDataCmd(void);

protected:
    /// Send chunk of data to the server.
    /// Return number of bytes actually transferred to server.
    virtual size_t SendChunk(const void* pChunk, size_t nofBytes) = 0;
    virtual bool Cancel(void) = 0;

    void DetachInterface(void);

    //
    size_t GetBytes2go(void) const
    {
        return m_Bytes2go;
    }
    void SetBytes2go(size_t value)
    {
        m_Bytes2go = value;
    }

private:
    size_t  m_Bytes2go;

private:
    void AttachTo(CDB_SendDataCmd* interface);

    CInterfaceHook<CDB_SendDataCmd> m_Interface;
};

} // namespace impl

END_NCBI_SCOPE


#endif  /* DBAPI_DRIVER_IMPL___DBAPI_IMPL_CMD__HPP */
