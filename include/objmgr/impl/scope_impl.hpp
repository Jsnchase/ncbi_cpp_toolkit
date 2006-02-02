#ifndef OBJMGR_IMPL_SCOPE_IMPL__HPP
#define OBJMGR_IMPL_SCOPE_IMPL__HPP

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
* Authors:
*           Andrei Gourianov
*           Aleksey Grichenko
*           Michael Kimelman
*           Denis Vakatov
*           Eugene Vasilchenko
*
* File Description:
*           Scope is top-level object available to a client.
*           Its purpose is to define a scope of visibility and reference
*           resolution and provide access to the bio sequence data
*
*/

#include <corelib/ncbiobj.hpp>
#include <corelib/ncbimtx.hpp>

#include <objmgr/impl/heap_scope.hpp>
#include <objmgr/impl/priority.hpp>

#include <objects/seq/seq_id_handle.hpp>

#include <objmgr/impl/scope_info.hpp>
#include <objmgr/impl/mutex_pool.hpp>
#include <objmgr/impl/data_source.hpp>


#include <objects/seq/Seq_inst.hpp> // for enum EMol

#include <set>
#include <map>

BEGIN_NCBI_SCOPE
BEGIN_SCOPE(objects)


// fwd decl
// objects
class CSeq_entry;
class CSeq_annot;
class CSeq_data;
class CSeq_id;
class CSeq_loc;
class CBioseq;

// objmgr
class CScope;
class CHeapScope;
class CObjectManager;
class CDataSource;
class CSeq_entry_Info;
class CSeq_annot_Info;
class CBioseq_Info;
class CBioseq_set_Info;
class CSeq_id_Handle;
class CSeqMap;
class CSynonymsSet;
class CTSE_Handle;
class CBioseq_Handle;
class CSeq_annot_Handle;
class CSeq_entry_Handle;
class CBioseq_set_Handle;
class CBioseq_EditHandle;
class CSeq_annot_EditHandle;
class CSeq_entry_EditHandle;
class CBioseq_set_EditHandle;
class CSeq_annot_EditHandle;
class CHandleRangeMap;
class CDataSource_ScopeInfo;
class CTSE_ScopeInfo;
struct SAnnotTypeSelector;
struct SAnnotSelector;
class CPriorityTree;
class CPriorityNode;
class IScopeTransaction_Impl;
class CScopeTransaction_Impl;
class CBioseq_ScopeInfo;


/////////////////////////////////////////////////////////////////////////////
// CScope_Impl
/////////////////////////////////////////////////////////////////////////////


struct SSeqMatch_Scope : public SSeqMatch_TSE
{
    typedef int TBlobStateFlags;

    SSeqMatch_Scope(void)
        : m_BlobState(0)
        {
        }

    CTSE_ScopeUserLock  m_TSE_Lock;
    TBlobStateFlags     m_BlobState;
};


class NCBI_XOBJMGR_EXPORT CScope_Impl : public CObject
{
public:
    typedef CTSE_ScopeUserLock                       TTSE_Lock;

    // History of requests
    typedef map<CSeq_id_Handle, SSeq_id_ScopeInfo>   TSeq_idMap;
    typedef TSeq_idMap::value_type                   TSeq_idMapValue;
    typedef set<CSeq_id_Handle>                      TSeq_idSet;
    typedef map<CTSE_Handle, TSeq_idSet>             TTSE_LockMatchSet;
    typedef int                                      TPriority;
    typedef map<CConstRef<CObject>, CRef<CObject> >  TEditInfoMap;
    typedef map<CRef<CDataSource>, CRef<CDataSource_ScopeInfo> > TDSMap;
    typedef vector<CSeq_id_Handle>                   TIds;
    typedef vector<CSeq_entry_Handle>                TTSE_Handles;
    typedef vector<CBioseq_Handle>                   TBioseqHandles;

    //////////////////////////////////////////////////////////////////
    // Adding top level objects: DataLoader, Seq-entry, Bioseq, Seq-annot
    enum EPriority {
        kPriority_NotSet = -1
    };

    // Add default data loaders from object manager
    void AddDefaults(TPriority priority = kPriority_NotSet);
    // Add data loader by name.
    // The loader (or its factory) must be known to Object Manager.
    void AddDataLoader(const string& loader_name,
                       TPriority priority = kPriority_NotSet);
    // Add the scope's datasources as a single group with the given priority
    void AddScope(CScope_Impl& scope, TPriority priority = kPriority_NotSet);

    // Add seq_entry, default priority is higher than for defaults or loaders
    CSeq_entry_Handle AddSeq_entry(CSeq_entry& entry,
                                   TPriority pri = kPriority_NotSet);
    CSeq_entry_Handle AddSharedSeq_entry(const CSeq_entry& entry,
                                         TPriority pri = kPriority_NotSet);
    // Add bioseq, return bioseq handle. Try to use unresolved seq-id
    // from the bioseq, fail if all ids are already resolved to
    // other sequences.
    CBioseq_Handle AddBioseq(CBioseq& bioseq,
                             TPriority pri = kPriority_NotSet);
    CBioseq_Handle AddSharedBioseq(const CBioseq& bioseq,
                                   TPriority pri = kPriority_NotSet);

    // Add Seq-annot.
    CSeq_annot_Handle AddSeq_annot(CSeq_annot& annot,
                                   TPriority pri = kPriority_NotSet);
    CSeq_annot_Handle AddSharedSeq_annot(const CSeq_annot& annot,
                                         TPriority pri = kPriority_NotSet);

    //////////////////////////////////////////////////////////////////
    // Modification of existing object tree
    CTSE_Handle GetEditHandle(const CTSE_Handle& src_tse);
    CBioseq_EditHandle GetEditHandle(const CBioseq_Handle& seq);
    CSeq_entry_EditHandle GetEditHandle(const CSeq_entry_Handle& entry);
    CSeq_annot_EditHandle GetEditHandle(const CSeq_annot_Handle& annot);
    CBioseq_set_EditHandle GetEditHandle(const CBioseq_set_Handle& seqset);

    // Add new sub-entry to the existing tree if it is in this scope
    CSeq_entry_EditHandle AttachEntry(const CBioseq_set_EditHandle& seqset,
                                      CSeq_entry& entry,
                                      int index = -1);
    CSeq_entry_EditHandle AttachEntry(const CBioseq_set_EditHandle& seqset,
                                      CRef<CSeq_entry_Info> entry,
                                      int index = -1);
    /*
    CSeq_entry_EditHandle CopyEntry(const CBioseq_set_EditHandle& seqset,
                                    const CSeq_entry_Handle& entry,
                                    int index = -1);
        // Argument entry will be moved to new place.
    CSeq_entry_EditHandle TakeEntry(const CBioseq_set_EditHandle& seqset,
                                    const CSeq_entry_EditHandle& entry,
                                    int index = -1);
    */
    // Argument entry must be removed.
    CSeq_entry_EditHandle AttachEntry(const CBioseq_set_EditHandle& seqset,
                                      const CSeq_entry_EditHandle& entry,
                                      int index = -1);

    // Add annotations to a seq-entry (seq or set)
    CSeq_annot_EditHandle AttachAnnot(const CSeq_entry_EditHandle& entry,
                                      CSeq_annot& annot);
    CSeq_annot_EditHandle AttachAnnot(const CSeq_entry_EditHandle& entry,
                                      CRef<CSeq_annot_Info> annot);
    /*
    CSeq_annot_EditHandle CopyAnnot(const CSeq_entry_EditHandle& entry,
                                    const CSeq_annot_Handle& annot);
    // Argument annot will be moved to new place.
    CSeq_annot_EditHandle TakeAnnot(const CSeq_entry_EditHandle& entry,
                                    const CSeq_annot_EditHandle& annot);
    */
    // Argument annot must be removed.
    CSeq_annot_EditHandle AttachAnnot(const CSeq_entry_EditHandle& entry,
                                      const CSeq_annot_EditHandle& annot);

    // Remove methods.
    void RemoveEntry(const CSeq_entry_EditHandle& entry);
    void RemoveBioseq(const CBioseq_EditHandle& seq);
    void RemoveBioseq_set(const CBioseq_set_EditHandle& seqset);
    void RemoveAnnot(const CSeq_annot_EditHandle& annot);

    // Modify Seq-entry.
    void SelectNone(const CSeq_entry_EditHandle& entry);
    CBioseq_EditHandle SelectSeq(const CSeq_entry_EditHandle& entry,
                                 CBioseq& seq);
    CBioseq_EditHandle SelectSeq(const CSeq_entry_EditHandle& entry,
                                 CRef<CBioseq_Info> seq);

    /*
    CBioseq_EditHandle CopySeq(const CSeq_entry_EditHandle& entry,
                               const CBioseq_Handle& seq);
    // Argument seq will be moved to new place.
    CBioseq_EditHandle TakeSeq(const CSeq_entry_EditHandle& entry,
                               const CBioseq_EditHandle& seq);
    */
    // Argument seq must be removed.
    CBioseq_EditHandle SelectSeq(const CSeq_entry_EditHandle& entry,
                                 const CBioseq_EditHandle& seq);

    CBioseq_set_EditHandle SelectSet(const CSeq_entry_EditHandle& entry,
                                     CBioseq_set& seqset);
    CBioseq_set_EditHandle SelectSet(const CSeq_entry_EditHandle& entry,
                                     CRef<CBioseq_set_Info> seqset);

    /*
    CBioseq_set_EditHandle CopySet(const CSeq_entry_EditHandle& entry,
                                   const CBioseq_set_Handle& seqset);
    // Argument seqset will be moved to new place.
    CBioseq_set_EditHandle TakeSet(const CSeq_entry_EditHandle& entry,
                                   const CBioseq_set_EditHandle& seqset);
    */
    // Argument seqset must be removed.
    CBioseq_set_EditHandle SelectSet(const CSeq_entry_EditHandle& entry,
                                     const CBioseq_set_EditHandle& seqset);

    // Get bioseq handle, limit id resolving
    CBioseq_Handle GetBioseqHandle(const CSeq_id_Handle& id, int get_flag);

    CBioseq_Handle GetBioseqHandleFromTSE(const CSeq_id_Handle& id,
                                          const CTSE_Handle& tse);

    // Get a set of bioseq handles
    TBioseqHandles GetBioseqHandles(const TIds& ids);

    // Get bioseq handle by seqloc
    CBioseq_Handle GetBioseqHandle(const CSeq_loc& loc, int get_flag);

    enum EActionIfLocked {
        eKeepIfLocked,
        eThrowIfLocked,
        eRemoveIfLocked
    };
    // History cleanup methods
    void ResetScope(void); // reset scope in initial state (no data)
    void ResetHistory(EActionIfLocked action = eKeepIfLocked);
    void RemoveFromHistory(CTSE_Handle tse);

    // Revoke data sources from the scope. Throw exception if the
    // operation fails (e.g. data source is in use or not found).
    void RemoveDataLoader(const string& loader_name,
                          EActionIfLocked action = eThrowIfLocked);
    // Remove TSE previously added using AddTopLevelSeqEntry() or
    // AddBioseq().
    void RemoveTopLevelSeqEntry(CTSE_Handle entry);

    // Deprecated interface
    CBioseq_Handle GetBioseqHandle(const CBioseq& bioseq);
    CBioseq_set_Handle GetBioseq_setHandle(const CBioseq_set& seqset);
    CSeq_entry_Handle GetSeq_entryHandle(const CSeq_entry& entry);
    CSeq_annot_Handle GetSeq_annotHandle(const CSeq_annot& annot);
    CSeq_entry_Handle GetSeq_entryHandle(const CTSE_Handle& tse);

    CScope& GetScope(void);

    // Get "native" bioseq ids without filtering and matching.
    TIds GetIds(const CSeq_id_Handle& idh);

    // Get bioseq synonyms, resolving to the bioseq in this scope.
    CConstRef<CSynonymsSet> GetSynonyms(const CSeq_id_Handle& id,
                                        int get_flag);
    CConstRef<CSynonymsSet> GetSynonyms(const CBioseq_Handle& bh);

    void GetAllTSEs(TTSE_Handles& tses, int kind);

    IScopeTransaction_Impl* CreateTransaction();
    void SetActiveTransaction(IScopeTransaction_Impl*);
    IScopeTransaction_Impl& GetTransaction();
   
    bool IsTransactionActive() const;

private:
    // constructor/destructor visible from CScope
    CScope_Impl(CObjectManager& objmgr);
    virtual ~CScope_Impl(void);

    // to prevent copying
    CScope_Impl(const CScope_Impl&);
    CScope_Impl& operator=(const CScope_Impl&);

    // Return the highest priority loader or null
    CDataSource* GetFirstLoaderSource(void);

    TTSE_LockMatchSet GetTSESetWithAnnots(const CSeq_id_Handle& idh);
    TTSE_LockMatchSet GetTSESetWithAnnots(const CBioseq_Handle& bh);

    void x_AttachToOM(CObjectManager& objmgr);
    void x_DetachFromOM(void);
    void x_RemoveFromHistory(CRef<CTSE_ScopeInfo> tse_info,
                             EActionIfLocked action);

    // clean some cache entries when new data source is added
    void x_ClearCacheOnNewData(const CTSE_ScopeInfo* replaced_tse = 0);
    void x_ClearCacheOnRemoveData(void);
    void x_ClearAnnotCache(void);

    CRef<CDataSource_ScopeInfo> GetEditDataSource(CDataSource_ScopeInfo& ds);

    CSeq_entry_EditHandle x_AttachEntry(const CBioseq_set_EditHandle& seqset,
                                        CRef<CSeq_entry_Info> entry,
                                        int index);
    void x_AttachEntry(const CBioseq_set_EditHandle& seqset,
                       const CSeq_entry_EditHandle& entry,
                       int index);
    CSeq_annot_EditHandle x_AttachAnnot(const CSeq_entry_EditHandle& entry,
                                        CRef<CSeq_annot_Info> annot);
    void x_AttachAnnot(const CSeq_entry_EditHandle& entry,
                       const CSeq_annot_EditHandle& annot);

    CBioseq_EditHandle x_SelectSeq(const CSeq_entry_EditHandle& entry,
                                   CRef<CBioseq_Info> bioseq);
    CBioseq_set_EditHandle x_SelectSet(const CSeq_entry_EditHandle& entry,
                                       CRef<CBioseq_set_Info> seqset);
    void x_SelectSeq(const CSeq_entry_EditHandle& entry,
                     const CBioseq_EditHandle& bioseq);
    void x_SelectSet(const CSeq_entry_EditHandle& entry,
                     const CBioseq_set_EditHandle& seqset);

    // Find the best possible resolution for the Seq-id
    void x_ResolveSeq_id(TSeq_idMapValue& id,
                         int get_flag,
                         SSeqMatch_Scope& match);
    // Iterate over priorities, find all possible data sources
    SSeqMatch_Scope x_FindBioseqInfo(const CPriorityTree& tree,
                                     const CSeq_id_Handle& idh,
                                     int get_flag);
    SSeqMatch_Scope x_FindBioseqInfo(const CPriorityNode& node,
                                     const CSeq_id_Handle& idh,
                                     int get_flag);
    SSeqMatch_Scope x_FindBioseqInfo(CDataSource_ScopeInfo& ds_info,
                                     const CSeq_id_Handle& idh,
                                     int get_flag);

    CBioseq_Handle x_GetBioseqHandleFromTSE(const CSeq_id_Handle& id,
                                            const CTSE_Handle& tse);
    void x_UpdateHandleSeq_id(CBioseq_Handle& bh);

    // guarded
    CBioseq_Handle GetBioseqHandle(const CBioseq_Info& seq,
                                   const CTSE_Handle& tse);
    // unguarded
    CBioseq_Handle x_GetBioseqHandle(const CBioseq_Info& seq,
                                     const CTSE_Handle& tse);

public:
    typedef pair<CConstRef<CSeq_entry_Info>, TTSE_Lock> TSeq_entry_Lock;
    typedef pair<CConstRef<CSeq_annot_Info>, TTSE_Lock> TSeq_annot_Lock;
    typedef pair<CConstRef<CBioseq_set_Info>, TTSE_Lock> TBioseq_set_Lock;
    typedef CDataSource_ScopeInfo::TBioseq_Lock TBioseq_Lock;

    TTSE_Lock x_GetTSE_Lock(const CSeq_entry& tse);
    TSeq_entry_Lock x_GetSeq_entry_Lock(const CSeq_entry& entry);
    TSeq_annot_Lock x_GetSeq_annot_Lock(const CSeq_annot& annot);
    TBioseq_set_Lock x_GetBioseq_set_Lock(const CBioseq_set& seqset);
    TBioseq_Lock x_GetBioseq_Lock(const CBioseq& bioseq);

    TTSE_Lock x_GetTSE_Lock(const CTSE_Lock& lock, CDataSource_ScopeInfo& ds);
    TTSE_Lock x_GetTSE_Lock(const CTSE_ScopeInfo& tse);

    CRef<CDataSource_ScopeInfo> x_GetDSInfo(CDataSource& ds);
    CRef<CDataSource_ScopeInfo> AddDS(CRef<CDataSource> ds,
                                      TPriority priority);
    CRef<CDataSource_ScopeInfo> GetNonSharedDS(TPriority priority);
    CRef<CDataSource_ScopeInfo> AddDSBefore(CRef<CDataSource> ds,
                                            CRef<CDataSource_ScopeInfo> ds2);

private:
    // Get bioseq handles for sequences from the given TSE using the filter
    typedef vector<CBioseq_Handle> TBioseq_HandleSet;
    typedef int TBioseqLevelFlag;
    void x_PopulateBioseq_HandleSet(const CSeq_entry_Handle& tse,
                                    TBioseq_HandleSet& handles,
                                    CSeq_inst::EMol filter,
                                    TBioseqLevelFlag level);

    CConstRef<CSynonymsSet> x_GetSynonyms(CBioseq_ScopeInfo& info);
    void x_AddSynonym(const CSeq_id_Handle& idh,
                      CSynonymsSet& syn_set, CBioseq_ScopeInfo& info);

    TSeq_idMapValue& x_GetSeq_id_Info(const CSeq_id_Handle& id);
    TSeq_idMapValue& x_GetSeq_id_Info(const CBioseq_Handle& bh);
    TSeq_idMapValue* x_FindSeq_id_Info(const CSeq_id_Handle& id);

    CRef<CBioseq_ScopeInfo> x_InitBioseq_Info(TSeq_idMapValue& info,
                                              int get_flag,
                                              SSeqMatch_Scope& match);
    bool x_InitBioseq_Info(TSeq_idMapValue& info,
                           CBioseq_ScopeInfo& bioseq_info);
    CRef<CBioseq_ScopeInfo> x_GetBioseq_Info(const CSeq_id_Handle& id,
                                             int get_flag,
                                             SSeqMatch_Scope& match);
    CRef<CBioseq_ScopeInfo> x_FindBioseq_Info(const CSeq_id_Handle& id,
                                              int get_flag,
                                              SSeqMatch_Scope& match);

    typedef CBioseq_ScopeInfo::TTSE_MatchSet TTSE_MatchSet;
    typedef CDataSource::TTSE_LockMatchSet TTSE_LockMatchSet_DS;
    void x_AddTSESetWithAnnots(TTSE_LockMatchSet& lock,
                               TTSE_MatchSet& match,
                               const TTSE_LockMatchSet_DS& add,
                               CDataSource_ScopeInfo& ds_info);
    void x_GetTSESetWithOrphanAnnots(TTSE_LockMatchSet& lock,
                                     TTSE_MatchSet& match,
                                     const TSeq_idSet& ids,
                                     CDataSource_ScopeInfo* excl_ds);
    void x_GetTSESetWithBioseqAnnots(TTSE_LockMatchSet& lock,
                                     TTSE_MatchSet& match,
                                     CBioseq_ScopeInfo& binfo);

    void x_LockMatchSet(TTSE_LockMatchSet& lock,
                        const TTSE_MatchSet& match);

private:
    CScope*              m_HeapScope;

    CRef<CObjectManager> m_ObjMgr;
    CPriorityTree        m_setDataSrc; // Data sources ordered by priority

    TDSMap               m_DSMap;

    CInitMutexPool       m_MutexPool;

    typedef CRWLock                     TConfLock;
    typedef TConfLock::TReadLockGuard   TConfReadLockGuard;
    typedef TConfLock::TWriteLockGuard  TConfWriteLockGuard;
    typedef CFastMutex                  TSeq_idMapLock;

    mutable TConfLock       m_ConfLock;

    TSeq_idMap              m_Seq_idMap;
    mutable TSeq_idMapLock  m_Seq_idMapLock;

    IScopeTransaction_Impl* m_Transaction;

    friend class CScope;
    friend class CHeapScope;
    friend class CObjectManager;
    friend class CSeqVector;
    friend class CDataSource;
    friend class CBioseq_CI;
    friend class CAnnot_Collector;
    friend class CBioseq_Handle;
    friend class CBioseq_set_Handle;
    friend class CSeq_entry_Handle;
    friend class CBioseq_EditHandle;
    friend class CBioseq_set_EditHandle;
    friend class CSeq_entry_EditHandle;
    friend class CSeq_annot_EditHandle;
    friend class CTSE_CI;
    friend class CSeq_annot_CI;
    friend class CSeqMap_CI;
    friend class CPrefetchTokenOld_Impl;
    friend class CDataSource_ScopeInfo;
    friend class CTSE_ScopeInfo;
    friend class CScopeTransaction_Impl;

    friend class CBioseq_ScopeInfo;
};


END_SCOPE(objects)
END_NCBI_SCOPE

#endif//OBJMGR_IMPL_SCOPE_IMPL__HPP
