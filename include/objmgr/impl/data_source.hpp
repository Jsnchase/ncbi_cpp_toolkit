#ifndef OBJECTS_OBJMGR_IMPL___DATA_SOURCE__HPP
#define OBJECTS_OBJMGR_IMPL___DATA_SOURCE__HPP

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
* Author: Aleksey Grichenko, Michael Kimelman, Eugene Vasilchenko
*
* File Description:
*   Data source for object manager
*
*/

#include <objects/objmgr/impl/tse_info.hpp>
#include <objects/objmgr/seq_id_mapper.hpp>
#include <objects/objmgr/scope.hpp>
#include <corelib/ncbimtx.hpp>
#include <set>
#include <map>
#include <list>

BEGIN_NCBI_SCOPE
BEGIN_SCOPE(objects)

// objects
class CDelta_seq;
class CDelta_ext;
class CSeq_interval;
class CSeq_data;
class CSeq_entry;
class CSeq_annot;
class CBioseq;
class CBioseq_set;

// infos
class CTSE_Info;
class CSeq_entry_Info;
class CSeq_annot_Info;
class CBioseq_Info;

// others
class CBioseq_Handle;

class NCBI_XOBJMGR_EXPORT CDataSource : public CObject
{
public:
    /// 'ctors
    CDataSource(CDataLoader& loader, CObjectManager& objmgr);
    CDataSource(CSeq_entry& entry, CObjectManager& objmgr);
    virtual ~CDataSource(void);

    /// Register new TSE (Top Level Seq-entry)
    typedef set< CTSE_Info* > TTSESet;

    CRef<CTSE_Info> AddTSE(CSeq_entry& se,
                           TTSESet* tse_set,
                           bool dead = false);

    /// Add new sub-entry to "parent".
    /// Return FALSE and do nothing if "parent" is not a node in an
    /// existing TSE tree of this data-source.
    bool AttachEntry(CSeq_entry& parent, CSeq_entry& entry);
    bool RemoveEntry(CSeq_entry& entry);

    /// Add sequence map for the given Bioseq.
    /// Return FALSE and do nothing if "bioseq" is not a node in an
    /// existing TSE tree of this data-source, or if "bioseq" is not a Bioseq.
    bool AttachMap(CSeq_entry& bioseq, CSeqMap& seqmap);

    /// Add seq-data to a Bioseq.
    /// Return FALSE and do nothing if "bioseq" is not a node in an
    /// existing TSE tree of this data-source, or if "bioseq" is not a Bioseq.
    /// If "start" is 0, "length" is equal to the sequence length (from
    /// the bioseq's seq-inst), and the segment is of type "literal", the data
    /// is added as a regular seq-data. Otherwise the bioseq is treated as a
    /// segmented sequence and the segment is added as a delta-ext segment.
    /// "seq_seg" must be allocated on the heap, since it will be referenced
    /// by the bioseq (except the case of a non-segmented bioseq, when only
    /// seq-data part of the seq_seg will be locked).
    bool AttachSeqData(CSeq_entry& bioseq, CDelta_ext& seq_seg,
                       size_t index,
                       TSeqPos start, TSeqPos length);

    /// Add annotations to a Seq-entry.
    /// Return FALSE and do nothing if "parent" is not a node in an
    /// existing TSE tree of this data-source.
    bool AttachAnnot(CSeq_entry& entry, CSeq_annot& annot);
    // Remove/replace seq-annot from the given entry
    bool RemoveAnnot(CSeq_entry& entry, CSeq_annot& annot);
    bool ReplaceAnnot(CSeq_entry& entry,
                      CSeq_annot& old_annot,
                      CSeq_annot& new_annot);

    /// Get TSE info by seq-id handle. This should also get the list of all
    /// seq-ids for all bioseqs and the list of seq-ids used in annotations.
    TTSE_Lock GetBlobById(const CSeq_id_Handle& idh);

    /// Get Bioseq info by Seq-Id.
    /// Return "NULL" handle if the Bioseq cannot be resolved.
    CRef<CBioseq_Info> GetBioseqHandle(CSeqMatch_Info& info);

    // Filter set of CSeq_id (setSource)
    // Select from the setSource ones which are "owned" by this DataSource
    // and move them into setResult
    void FilterSeqid(TSeq_id_HandleSet& setResult,
                     TSeq_id_HandleSet& setSource) const;

    // Remove TSE from the datasource, update indexes
    void DropAllTSEs(void);
    bool DropTSE(CSeq_entry& tse);

    // Contains (or can load) any entries?
    bool IsEmpty(void) const;

    CSeq_id_Mapper& GetSeq_id_Mapper(void) const;

    /// Get the complete Bioseq (as loaded by now)
    const CBioseq& GetBioseq(const CBioseq_Handle& handle);

    /// Get top level Seq-Entry for a Bioseq
    const CSeq_entry& GetTSE(const CBioseq_Handle& handle);

    const CSeq_entry& GetTSEFromInfo(const TTSE_Lock& tse);

    typedef CConstRef<CBioseq> TBioseqCore;
    /// Get Bioseq core structure
    TBioseqCore GetBioseqCore(const CBioseq_Handle& handle);

    /// Get sequence map
    const CSeqMap& GetSeqMap(const CBioseq_Handle& handle);

    CDataLoader* GetDataLoader(void);
    CSeq_entry* GetTopEntry(void);

    // Internal typedefs
    typedef CTSE_Info::TRange                       TRange;
    typedef CTSE_Info::TRangeMap                    TRangeMap;
    typedef CTSE_Info::TAnnotMap                    TAnnotMap;

    typedef map<CSeq_id_Handle, TTSESet>            TTSEMap;

    typedef map<const CSeq_entry*, CRef<CTSE_Info> > TTSE_InfoMap;
    typedef map<const CSeq_entry*, CSeq_entry_Info*> TSeq_entry_InfoMap;
    typedef map<const CSeq_annot*, CSeq_annot_Info*> TSeq_annot_InfoMap;
    typedef map<const CBioseq*, CBioseq_Info*>       TBioseq_InfoMap;
    typedef map<const CObject*, CAnnotObject_Info*>  TAnnotObject_InfoMap;

    void UpdateAnnotIndex(const CHandleRangeMap& loc,
                          const SAnnotTypeSelector& sel);
    void UpdateAnnotIndex(const CHandleRangeMap& loc,
                          const SAnnotTypeSelector& sel,
                          const CSeq_entry_Info& entry_info);
    void UpdateAnnotIndex(const CHandleRangeMap& loc,
                          const SAnnotTypeSelector& sel,
                          const CSeq_annot_Info& annot_info);
    void GetSynonyms(const CSeq_id_Handle& id,
                     set<CSeq_id_Handle>& syns);
    void GetTSESetWithAnnots(const CSeq_id_Handle& idh,
                             set<TTSE_Lock>& tse_set,
                             CScope::TRequestHistory& history);

    // Fill the set with bioseq handles for all sequences from a given TSE.
    // Return empty tse lock if the entry was not found or is not a TSE.
    // "filter" may be used to select a particular sequence type.
    // Used to initialize bioseq iterators.
    TTSE_Lock GetTSEHandles(const CSeq_entry& entry,
                            set<CBioseq_Info*>& bioseqs,
                            CSeq_inst::EMol filter);

    CSeqMatch_Info BestResolve(CSeq_id_Handle idh);

    bool IsSynonym(const CSeq_id_Handle& h1, const CSeq_id_Handle& h2) const;

    string GetName(void) const;

    virtual void DebugDump(CDebugDumpContext ddc, unsigned int depth) const;

    CConstRef<CTSE_Info> GetTSEInfo(const CSeq_entry& entry);
    CConstRef<CSeq_entry_Info> GetSeq_entry_Info(const CSeq_entry& entry);
    CConstRef<CSeq_annot_Info> GetSeq_annot_Info(const CSeq_annot& annot);

private:
    // attach, detach, index & unindex methods
    // TSE
    CRef<CTSE_Info> x_AttachTSE(CSeq_entry& tse, bool dead);
    void x_DetachTSE(CTSE_Info& tse_info);
    // Seq-entry
    void x_AttachSeq_entry_Contents(CSeq_entry_Info& entry_info);
    void x_DetachSeq_entry_Contents(CSeq_entry_Info& entry_info);
    // Bioseq-set
    void x_AttachBioseqSet(CBioseq_set& seq_set,
                           CSeq_entry_Info& entry_info);
    // Bioseq
    void x_AttachBioseq(CBioseq& seq,
                        CSeq_entry_Info& entry_info);
    void x_IndexBioseq(CBioseq_Info& seq_info);
    void x_UnindexBioseq(CBioseq_Info& seq_info);
    // SET OF Seq-annot
    typedef list< CRef< CSeq_annot > > TAnnots;
    void x_AttachSeq_annots(TAnnots& annots, CSeq_entry_Info& entry_info);
    // Seq-annot
    void x_IndexSeq_annot(CSeq_annot_Info& annot_info);
    void x_UnindexSeq_annot(CSeq_annot_Info& annot_info);

    // create, delete & lookup Xxx_Info objects
    // TSE
    CRef<CTSE_Info> x_CreateTSE_Info(CSeq_entry& tse, bool dead);
    CRef<CTSE_Info> x_FindTSE_Info(const CSeq_entry& tse);
    void x_DeleteTSE_Info(CTSE_Info& tse_info);
    // Seq-entry
    CRef<CSeq_entry_Info> x_CreateSeq_entry_Info(CSeq_entry& entry,
                                                 CSeq_entry_Info& par);
    CRef<CSeq_entry_Info> x_FindSeq_entry_Info(const CSeq_entry& entry);
    void x_DeleteSeq_entry_Info(CSeq_entry_Info& entry_info);
    // Bioseq
    CRef<CBioseq_Info> x_CreateBioseq_Info(CBioseq& seq,
                                           CSeq_entry_Info& parent);
    CRef<CBioseq_Info> x_FindBioseq_Info(const CBioseq& seq);
    void x_DeleteBioseq_Info(CBioseq_Info& seq_info);
    // Seq-annot
    CRef<CSeq_annot_Info> x_CreateSeq_annot_Info(CSeq_annot& annot,
                                                 CSeq_entry_Info& parent);
    CRef<CSeq_annot_Info> x_FindSeq_annot_Info(const CSeq_annot& annot);
    void x_DeleteSeq_annot_Info(CSeq_annot_Info& annot_info);

    // index and unindex of annot objects
    bool x_MakeGenericSelector(SAnnotTypeSelector& annotSelector) const;

    // Process a single data element
    void x_MapAnnotObject(CTSE_Info::TRangeMap& mapByRange,
                          const CTSE_Info::TRange& range,
                          const SAnnotObject_Index& annotRef);
    void x_MapAnnotObject(CRef<CAnnotObject_Info>& annot_info,
                          const CHandleRangeMap& hrm,
                          SAnnotTypeSelector annotSelector);
    bool x_DropAnnotObject(CTSE_Info::TRangeMap& mapByRange,
                           const CTSE_Info::TRange& range,
                           CRef<CAnnotObject_Info>& annotObj);
    void x_DropAnnotObject(CRef<CAnnotObject_Info>& annotObj,
                           const CHandleRangeMap& hrm,
                           SAnnotTypeSelector annotSelector);
    void x_DropAnnotObject(const CObject* annotPtr);
    void x_MapAnnotObject(CSeq_feat& feat, CSeq_annot_Info& annot);
    void x_MapAnnotObject(CSeq_align& align, CSeq_annot_Info& annot);
    void x_MapAnnotObject(CSeq_graph& graph, CSeq_annot_Info& annot);
    //
    void x_MapAnnotObject(CAnnotObject_Info* annot_info_ptr);
    void x_DropAnnotObject(const CObject* annotObj, CSeq_annot_Info& annot);


    // Find the seq-entry with best bioseq for the seq-id handle.
    // The best bioseq is the bioseq from the live TSE or from the
    // only one TSE containing the ID (no matter live or dead).
    // If no matches were found, return 0.
    TTSE_Lock x_FindBestTSE(const CSeq_id_Handle& handle) const;

    // Create CSeqMap for a bioseq
    void x_CreateSeqMap(CBioseq_Info& seq_info);
    // Non-const version of GetSeqMap()
    const CSeqMap& x_GetSeqMap(const CBioseq_Handle& handle);

    void x_GetAnnotData(const CHandleRangeMap& loc,
                        const SAnnotTypeSelector& sel);
    void x_IndexAllAnnots(CSeq_entry_Info& entry_info);

    bool x_RemoveSeq_annot(TAnnots& annot_set, CSeq_annot& annot);

    // Global cleanup -- search for unlocked TSEs and drop them.
    void x_CleanupUnusedEntries(void);

    // Change live/dead status of a TSE
    void x_UpdateTSEStatus(CSeq_entry& tse, bool dead);

    // Used to lock: m_Entries, m_TSE_seq, m_TSE_ref
    mutable CMutex m_DataSource_Mtx;

    CDataLoader          *m_Loader;
    CRef<CSeq_entry>      m_pTopEntry;
    CObjectManager*       m_ObjMgr;

    TTSE_InfoMap          m_TSE_InfoMap; // set of all TSEs to keep them locked
    TSeq_entry_InfoMap    m_Seq_entry_InfoMap; // All known Seq-entries
    TSeq_annot_InfoMap    m_Seq_annot_InfoMap; // all known Seq-annots
    TBioseq_InfoMap       m_Bioseq_InfoMap;    // all known Bioseqs
    TAnnotObject_InfoMap  m_AnnotObject_InfoMap; // all known annotations

    TTSEMap               m_TSE_seq;   // id -> TSEs with bioseq
    TTSEMap               m_TSE_ref;   // id -> TSEs with references to id

    // "true" if annotations need to be indexed.
    bool                  m_DirtyAnnotIndex;

    friend class CAnnotTypes_CI; // using mutex etc.
    friend class CBioseq_Handle; // using mutex
    friend class CGBDataLoader;  //
};

inline
CDataLoader* CDataSource::GetDataLoader(void)
{
    return m_Loader;
}

inline
CSeq_entry* CDataSource::GetTopEntry(void)
{
    return m_pTopEntry;
}

inline
bool CDataSource::IsEmpty(void) const
{
    return m_Loader == 0  &&  m_Seq_entry_InfoMap.empty();
}

inline
CSeq_id_Mapper& CDataSource::GetSeq_id_Mapper(void) const
{
    return CSeq_id_Mapper::GetSeq_id_Mapper();
}


END_SCOPE(objects)
END_NCBI_SCOPE

/*
* ---------------------------------------------------------------------------
* $Log$
* Revision 1.50  2003/04/24 16:12:37  vasilche
* Object manager internal structures are splitted more straightforward.
* Removed excessive header dependencies.
*
* Revision 1.49  2003/04/14 21:32:16  grichenk
* Avoid passing CScope as an argument to CDataSource methods
*
* Revision 1.48  2003/03/24 21:26:43  grichenk
* Added support for CTSE_CI
*
* Revision 1.47  2003/03/21 19:22:50  grichenk
* Redesigned TSE locking, replaced CTSE_Lock with CRef<CTSE_Info>.
*
* Revision 1.46  2003/03/18 21:48:28  grichenk
* Removed obsolete class CAnnot_CI
*
* Revision 1.45  2003/03/18 14:52:57  grichenk
* Removed obsolete methods, replaced seq-id with seq-id handle
* where possible. Added argument to limit annotations update to
* a single seq-entry.
*
* Revision 1.44  2003/03/11 14:15:50  grichenk
* +Data-source priority
*
* Revision 1.43  2003/03/10 16:55:16  vasilche
* Cleaned SAnnotSelector structure.
* Added shortcut when features are limited to one TSE.
*
* Revision 1.42  2003/03/03 20:31:09  vasilche
* Removed obsolete method PopulateTSESet().
*
* Revision 1.41  2003/02/27 14:35:32  vasilche
* Splitted PopulateTSESet() by logically independent parts.
*
* Revision 1.40  2003/02/25 14:48:07  vasilche
* Added Win32 export modifier to object manager classes.
*
* Revision 1.39  2003/02/24 18:57:21  vasilche
* Make feature gathering in one linear pass using CSeqMap iterator.
* Do not use feture index by sub locations.
* Sort features at the end of gathering in one vector.
* Extracted some internal structures and classes in separate header.
* Delay creation of mapped features.
*
* Revision 1.38  2003/02/24 14:51:10  grichenk
* Minor improvements in annot indexing
*
* Revision 1.37  2003/02/13 14:34:32  grichenk
* Renamed CAnnotObject -> CAnnotObject_Info
* + CSeq_annot_Info and CAnnotObject_Ref
* Moved some members of CAnnotObject to CSeq_annot_Info
* and CAnnotObject_Ref.
* Added feat/align/graph to CAnnotObject_Info map
* to CDataSource.
*
* Revision 1.36  2003/02/05 17:57:41  dicuccio
* Moved into include/objects/objmgr/impl to permit data loaders to be defined
* outside of xobjmgr.
*
* Revision 1.35  2003/01/29 22:03:46  grichenk
* Use single static CSeq_id_Mapper instead of per-OM model.
*
* Revision 1.34  2003/01/22 20:11:54  vasilche
* Merged functionality of CSeqMapResolved_CI to CSeqMap_CI.
* CSeqMap_CI now supports resolution and iteration over sequence range.
* Added several caches to CScope.
* Optimized CSeqVector().
* Added serveral variants of CBioseqHandle::GetSeqVector().
* Tried to optimize annotations iterator (not much success).
* Rewritten CHandleRange and CHandleRangeMap classes to avoid sorting of list.
*
* Revision 1.33  2002/12/26 20:55:17  dicuccio
* Moved seq_id_mapper.hpp, tse_info.hpp, and bioseq_info.hpp -> include/ tree
*
* Revision 1.32  2002/12/26 16:39:24  vasilche
* Object manager class CSeqMap rewritten.
*
* Revision 1.31  2002/12/06 15:36:00  grichenk
* Added overlap type for annot-iterators
*
* Revision 1.30  2002/11/08 22:15:51  grichenk
* Added methods for removing/replacing annotations
*
* Revision 1.29  2002/10/18 19:12:40  grichenk
* Removed mutex pools, converted most static mutexes to non-static.
* Protected CSeqMap::x_Resolve() with mutex. Modified code to prevent
* dead-locks.
*
* Revision 1.28  2002/10/02 17:58:23  grichenk
* Added sequence type filter to CBioseq_CI
*
* Revision 1.27  2002/09/30 20:01:19  grichenk
* Added methods to support CBioseq_CI
*
* Revision 1.26  2002/07/08 20:51:01  grichenk
* Moved log to the end of file
* Replaced static mutex (in CScope, CDataSource) with the mutex
* pool. Redesigned CDataSource data locking.
*
* Revision 1.25  2002/06/04 17:18:33  kimelman
* memory cleanup :  new/delete/Cref rearrangements
*
* Revision 1.24  2002/05/31 17:53:00  grichenk
* Optimized for better performance (CTSE_Info uses atomic counter,
* delayed annotations indexing, no location convertions in
* CAnnot_Types_CI if no references resolution is required etc.)
*
* Revision 1.23  2002/05/28 18:00:43  gouriano
* DebugDump added
*
* Revision 1.22  2002/05/14 20:06:26  grichenk
* Improved CTSE_Info locking by CDataSource and CDataLoader
*
* Revision 1.21  2002/05/06 03:28:47  vakatov
* OM/OM1 renaming
*
* Revision 1.20  2002/05/03 21:28:09  ucko
* Introduce T(Signed)SeqPos.
*
* Revision 1.19  2002/04/17 21:09:14  grichenk
* Fixed annotations loading
* +IsSynonym()
*
* Revision 1.18  2002/04/11 12:08:21  grichenk
* Fixed GetResolvedSeqMap() implementation
*
* Revision 1.17  2002/03/28 14:02:31  grichenk
* Added scope history checks to CDataSource::x_FindBestTSE()
*
* Revision 1.16  2002/03/20 04:50:13  kimelman
* GB loader added
*
* Revision 1.15  2002/03/07 21:25:34  grichenk
* +GetSeq_annot() in annotation iterators
*
* Revision 1.14  2002/03/05 18:44:55  grichenk
* +x_UpdateTSEStatus()
*
* Revision 1.13  2002/03/05 16:09:10  grichenk
* Added x_CleanupUnusedEntries()
*
* Revision 1.12  2002/03/04 15:09:27  grichenk
* Improved MT-safety. Added live/dead flag to CDataSource methods.
*
* Revision 1.11  2002/03/01 19:41:34  gouriano
* *** empty log message ***
*
* Revision 1.10  2002/02/28 20:53:32  grichenk
* Implemented attaching segmented sequence data. Fixed minor bugs.
*
* Revision 1.9  2002/02/21 19:27:05  grichenk
* Rearranged includes. Added scope history. Added searching for the
* best seq-id match in data sources and scopes. Updated tests.
*
* Revision 1.8  2002/02/07 21:27:35  grichenk
* Redesigned CDataSource indexing: seq-id handle -> TSE -> seq/annot
*
* Revision 1.7  2002/02/06 21:46:11  gouriano
* *** empty log message ***
*
* Revision 1.6  2002/02/05 21:46:28  gouriano
* added FindSeqid function, minor tuneup in CSeq_id_mapper
*
* Revision 1.5  2002/01/29 17:45:00  grichenk
* Added seq-id handles locking
*
* Revision 1.4  2002/01/28 19:44:49  gouriano
* changed the interface of BioseqHandle: two functions moved from Scope
*
* Revision 1.3  2002/01/23 21:59:31  grichenk
* Redesigned seq-id handles and mapper
*
* Revision 1.2  2002/01/18 15:56:24  gouriano
* changed TSeqMaps definition
*
* Revision 1.1  2002/01/16 16:25:55  gouriano
* restructured objmgr
*
* Revision 1.1  2002/01/11 19:04:01  gouriano
* restructured objmgr
*
*
* ===========================================================================
*/

#endif  // OBJECTS_OBJMGR_IMPL___DATA_SOURCE__HPP
