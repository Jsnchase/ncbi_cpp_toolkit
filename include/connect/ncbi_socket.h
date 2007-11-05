#ifndef CONNECT___NCBI_SOCKET__H
#define CONNECT___NCBI_SOCKET__H

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
 * Author:  Denis Vakatov
 *
 * File Description:
 *   Plain portable TCP/IP socket API for:  UNIX, MS-Win, MacOS
 *   Platform-specific library requirements:
 *     [UNIX ]   -DNCBI_OS_UNIX     -lresolv -lsocket -lnsl
 *     [MSWIN]   -DNCBI_OS_MSWIN    wsock32.lib
 *     [MacOS]   -DNCBI_OS_MAC      NCSASOCK -- BSD-style socket emulation lib
 *
 *********************************
 * Generic:
 *
 *  SOCK_AllowSigPipeAPI
 *
 *  SOCK_InitializeAPI
 *  SOCK_ShutdownAPI
 *
 * Event trigger (handle TRIGGER):
 *
 *  TRIGGER_Create
 *  TRIGGER_Set
 *  TRIGGER_IsSet
 *  TRIGGER_Reset
 *  TRIGGER_Close
 *
 * Listening socket (handle LSOCK):
 *
 *  LSOCK_Create
 *  LSOCK_Accept
 *  LSOCK_Close
 *  
 * I/O Socket (handle SOCK):
 *
 *  SOCK_Create[Ex]      (see also LSOCK_Accept)
 *  SOCK_CreateOnTop[Ex]
 *  SOCK_Reconnect
 *  SOCK_Shutdown
 *  SOCK_Close
 *  SOCK_Wait
 *  SOCK_Poll
 *  SOCK_SetTimeout
 *  SOCK_GetReadTimeout
 *  SOCK_GetWriteTimeout
 *  SOCK_Read (including "peek" and "persistent read")
 *  SOCK_ReadLine
 *  SOCK_PushBack
 *  SOCK_Status
 *  SOCK_Write
 *  SOCK_Abort
 *  SOCK_GetLocalPort
 *  SOCK_GetPeerAddress
 *  SOCK_GetPeerAddressString
 *
 *  SOCK_SetReadOnWriteAPI
 *  SOCK_SetReadOnWrite
 *  SOCK_SetInterruptOnSignalAPI
 *  SOCK_SetInterruptOnSignal
 *  SOCK_SetReuseAddressAPI
 *  SOCK_SetReuseAddress
 *  SOCK_DisableOSSendDelay
 *
 * Datagram Socket:
 *
 *  DSOCK_Create[Ex]
 *  DSOCK_Bind
 *  DSOCK_Connect
 *  DSOCK_WaitMsg
 *  DSOCK_SendMsg
 *  DSOCK_RecvMsg
 *  DSOCK_WipeMsg
 *  DSOCK_SetBroadcast
 *
 * Socket classification:
 *
 *  SOCK_IsDatagram
 *  SOCK_IsClientSide
 *  SOCK_IsServerSide
 *  SOCK_IsUNIX
 *
 * Data logging:
 *
 *  SOCK_SetDataLoggingAPI
 *  SOCK_SetDataLogging
 *
 * Auxiliary:
 *
 *  SOCK_gethostname
 *  SOCK_ntoa
 *  SOCK_isip
 *  SOCK_isipEx
 *  SOCK_HostToNetShort
 *  SOCK_HostToNetLong
 *  SOCK_NetToHostShort
 *  SOCK_NetToHostLong
 *  SOCK_gethostbyname
 *  SOCK_gethostbyaddr
 *  SOCK_GetLoopbackAddress
 *  SOCK_StringToHostPort
 *  SOCK_HostPortToString
 *
 */

#if defined(NCBISOCK__H)
#  error "<ncbisock.h> and <ncbi_socket.h> must never be #include'd together"
#endif

#include <connect/ncbi_core.h>


/** @addtogroup Sockets
 *
 * @{
 */


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 *  TYPEDEFS & MACROS
 */


/** Network and host byte order enumeration type
 */
typedef enum {
    eNH_HostByteOrder,
    eNH_NetworkByteOrder
} ENH_ByteOrder;


/** Forward declarations of the hidden socket internal structures, and
 * their upper-level handles to use by the LSOCK_*() and SOCK_*() API
 */
struct LSOCK_tag;                /* listening socket:  internal storage */
typedef struct LSOCK_tag* LSOCK; /* listening socket:  handle           */

struct SOCK_tag;                 /* socket:  internal storage           */
typedef struct SOCK_tag*  SOCK;  /* socket:  handle                     */

struct TRIGGER_tag;
typedef struct TRIGGER_tag* TRIGGER;


/******************************************************************************
 * Multi-Thread safety
 *
 * If you are using this API in a multi-threaded application, and there is
 * more than one thread using this API, it is safe to call SOCK_InitializeAPI()
 * explicitly in the beginning of your main thread, before you run any other
 * threads, and to call SOCK_ShutdownAPI() after all threads are exited.
 *
 * As soon as the API is initialized it becomes relatively MT-safe, however
 * you still must not operate on same LSOCK or SOCK objects from different
 * threads simultaneously.
 *
 * A MUCH BETTER WAY of dealing with this issue is to provide your own MT
 * locking callback (see CORE_SetLOCK in "ncbi_core.h"). This will also
 * guarantee the proper MT protection should some other SOCK functions
 * start to access any static data in the future.
 *
 * @sa
 *  CORE_SetLOCK
 */



/******************************************************************************
 *  Error & Data Logging
 *
 * @li <b>NOTE:</b>  Use CORE_SetLOG() from "ncbi_core.h" to set log handler.
 *
 * @sa
 *  CORE_SetLOG
 */

/** By default ("log" == eDefault, which is eOff), data are not logged.
 * @param log
 *  To start logging the data, call this func with "log" == eOn.
 *  To stop  logging the data, call this func with "log" == eOff.
 * @return
 *  Prior setting.
 * @sa
 *  SOCK_SetDataLogging
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetDataLoggingAPI
(ESwitch log
 );


/** Control the data logging for socket "sock" individually.
 * @param sock
 *  [in]  socket handle
 * @param log
 *  [in]  requested data logging
 *  To reset to the global default behavior (as set by SOCK_SetDataLoggingAPI),
 *  call this function with "log" == eDefault.
 * @return
 *  Prior setting.
 * @sa
 *  SOCK_SetDataLoggingAPI, SOCK_Create, DSOCK_Create
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetDataLogging
(SOCK    sock,
 ESwitch log
 );



/******************************************************************************
 *   I/O restart on signals
 */

/** Control restartability of I/O interrupted by signals.
 * By default ("on_off" == eDefault,eOff), I/O is restartable if interrupted.
 * @param on_off
 *  [in]  eOn to cancel I/O on signals;  eOff to restart
 * @return
 *  Prior setting.
 * @sa
 *  SOCK_SetInterruptOnSignal
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetInterruptOnSignalAPI
(ESwitch on_off
 );


/** Control restartability of I/O interrupted by signals on a per-socket basis.
 * eDefault causes the use of global API flag.
 * @param sock
 *  [in]  socket handle
 * @param on_off
 *  [in]  per-socket I/O restart behavior on signals
 * @return
 *  Prior setting.
 * @sa
 *  SOCK_SetInterruptOnSignalAPI, SOCK_Create, DSOCK_Create
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetInterruptOnSignal
(SOCK    sock,
 ESwitch on_off
 );



/******************************************************************************
 *   Address reuse: EXPERIMENTAL and may be removed in the upcoming releases!
 */

/** Control address reuse for socket addresses taken by the API.
 * By default ("on_off" == eDefault,eOff), address is not marked
 * for reuse in SOCK, but is always reused for LSOCK.
 * @param on_off
 *  [in]  whether to turn on (eOn), turn off (eOff), or use default (eDefault)
 * @return
 *  Prior setting.
 * @sa
 *  SOCK_SetReuseAddress
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetReuseAddressAPI
(ESwitch on_off
 );


/** Control reuse of socket addresses on per-socket basis
 * Note: only a boolean parameter value is can be used here.
 * @param sock
 *  [in]  socket handle
 * @param on_off
 *  [in]  whether to reuse the address (true, non-zero) or not (false, zero)
 * @sa
 *  SOCK_SetReuseAddressAPI, SOCK_Create, DSOCK_Create
 */
extern NCBI_XCONNECT_EXPORT void SOCK_SetReuseAddress
(SOCK         sock,
 int/**bool*/ on_off);



/******************************************************************************
 *  API Initialization and Shutdown/Cleanup
 */

/** This is a helper call that can improve I/O behavior.
 * @param timeout
 *  [in]  Break down long waits on I/O into smaller chunks of at most "timeout"
 *  duration each.  This can help recover "hanging" sockets from indefinite
 *  wait and allow them to report an exceptional I/O condition.
 * @sa
 *  SOCK_Wait, SOCK_Poll
 */
extern NCBI_XCONNECT_EXPORT const STimeout*
SOCK_SetSelectInternalRestartTimeout
(const STimeout* timeout);


/** By default (on UNIX platforms) the SOCK API functions automagically call
 * "signal(SIGPIPE, SIG_IGN)" on initialization.  To prohibit this feature,
 * you must call SOCK_AllowSigPipeAPI() before you call any other
 * function from the SOCK API.
 */
extern NCBI_XCONNECT_EXPORT void SOCK_AllowSigPipeAPI(void);


/** Initialize all internal/system data & resources to be used by the SOCK API.
 * @li <b>NOTE:</b>
 *  You can safely call it more than once; just, all calls after the first
 *  one will have no result. 
 * @li <b>NOTE:</b>
 *  Usually, SOCK API does not require an explicit initialization -- as it is
 *  guaranteed to initialize itself automagically, in one of API functions,
 *  when necessary.  Yet, see the "Multi Thread safety" remark above.
 * @li <b>NOTE:</b>
 *  This call, when used for the very first time in the application, enqueues
 *  SOCK_ShutdownAPI() to be called upon application exit on plaftorms that
 *  provide this functionality. In any case, the application can opt for
 *  explicit SOCK_ShutdownAPI() call when it is done with all sockets.
 * @sa
 *  SOCK_ShutdownAPI
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_InitializeAPI(void);


/** Cleanup; destroy all internal/system data & resources used by the SOCK API.
 * ATTENTION:  no function from the SOCK API should be called after this call!
 * @li <b>NOTE:</b>
 *  You can safely call it more than once; just, all calls after the first
 *  one will have no result. 
 * @sa
 *  SOCK_InitializeAPI
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_ShutdownAPI(void);



/******************************************************************************
 *  EVENT TRIGGER
 */

/** Create an event trigger.
 * @param trigger
 *  [in|out]  a pointer to a location where to store a handle of the trigger
 * @return
 *  eIO_Success on success; other status on error
 * @sa
 *  TRIGGER_Close, TRIGGER_Set
 */
extern NCBI_XCONNECT_EXPORT EIO_Status TRIGGER_Create
(TRIGGER* trigger,
 ESwitch  log
 );


/** Close an event trigger.
 * @param trigger
 *  [in]  a handle returned by TRIGGER_Create
 * @return
 *   eIO_Success on success; other status on error
 * @sa
 *  TRIGGER_Create
 */
extern NCBI_XCONNECT_EXPORT EIO_Status TRIGGER_Close
(TRIGGER  trigger
 );


/** Set an event trigger.  Can be used from many threads concurrently.
 * @param trigger
 *  [in]  a handle returned by TRIGGER_Create
 * @return
 *   eIO_Success on success; other status on error.
 * @sa
 *  TRIGGER_Create, TRIGGER_IsSet
 */
extern NCBI_XCONNECT_EXPORT EIO_Status TRIGGER_Set
(TRIGGER  trigger
 );


/** Check whether the trigger has been set.  Should not be used from
 * multiple threads concurrently at a time.
 * @param trigger
 *  [in]  a handle returned by TRIGGER_Create
 * @return
 *  eIO_Success if the trigger has been set;
 *  eIO_Closed  if the trigger has not yet been set;
 *  other status on error.
 * @sa
 *  TRIGGER_Create, TRIGGER_Set, TRIGGER_Reset
 */
extern NCBI_XCONNECT_EXPORT EIO_Status TRIGGER_IsSet
(TRIGGER  trigger
 );


/** Reset trigger.  Should not be used from multiple threads concurently.
 * @param trigger
 *  [in]  a handle returned by TRIGGER_Create
 * @return
 *  eIO_Success if the trigger has been set; other status on error.
 * @sa
 *  TRIGGER_Create, TRIGGER_Set
 */
extern NCBI_XCONNECT_EXPORT EIO_Status TRIGGER_Reset
(TRIGGER  trigger
 );



/******************************************************************************
 *  LISTENING SOCKET [SERVER-side]
 */

typedef enum {
    fLSCE_LogOff      = eOff,    /** logging is inherited in Accept()ed SOCKs*/
    fLSCE_LogOn       = eOn,     /**                    -"-                  */
    fLSCE_LogDefault  = eDefault,/**                    -"-                  */
    fLSCE_BindAny     = 0,       /** bind to 0.0.0.0 (i.e. any), default     */
    fLSCE_BindLocal   = 0x10,    /** bind to 127.0.0.1 only                  */
    fLSCE_CloseOnExec = 0x20
} FLSCE_Flags;
typedef unsigned int TLSCE_Flags;

/** [SERVER-side]  Create and initialize the server-side(listening) socket
 * (socket() + bind() + listen())
 * @param port
 *  [in]  the port to listen at
 * @param backlog
 *  [in]  maximal # of pending connections
 *  <b>NOTE:</b> on some systems, "backlog" can be silently limited
 *  down to 128 (or 5).
 * @param lsock
 *  [out] handle of the created listening socket
 * @param flags
 *  [in]  special modifiers
 * @sa
 *  LSOCK_Create, LSOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status LSOCK_CreateEx
(unsigned short port,    
 unsigned short backlog, 
 LSOCK*         lsock,   
 TLSCE_Flags    flags    
 );


/** [SERVER-side]  Create and initialize the server-side(listening) socket
 * Same as LSOCK_CreateEx called with the last argument provided as 0.
 * @param port
 *  [in]  the port to listen at
 * @param backlog
 *  [in]  maximal # of pending connections
 *  <b>NOTE:</b> on some systems, "backlog" can be silently limited
 *  down to 128 (or 5).
 * @param lsock
 *  [out] handle of the created listening socket
 * @sa
 *  LSOCK_CreateEx, LSOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status LSOCK_Create
(unsigned short port,   
 unsigned short backlog, 
 LSOCK*         lsock    
 );


/** [SERVER-side]  Accept connection from a client.
 * @param lsock
 *  [in]  handle of a listening socket
 * @param timeout
 *  [in]  timeout (infinite if NULL)
 *  <b>NOTE:</b> the provided "timeout" is for this accept() only.
 *  To set I/O timeout on the resulted socket use SOCK_SetTimeout();
 *  all I/O timeouts are infinite by default.
 * @param sock
 *  [out] handle of the accepted socket
 * @sa
 *  SOCK_Create, SOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status LSOCK_Accept
(LSOCK           lsock,    
 const STimeout* timeout, 
 SOCK*           sock      
 );


/** [SERVER-side]  Close the listening socket, destroy relevant internal data.
 * @param lsock
 *  [in]  listening socket handle to close
 * The call invalidates the handle, so its further is not allowed.
 * @sa
 *  LSOCK_Create
 */
extern NCBI_XCONNECT_EXPORT EIO_Status LSOCK_Close(LSOCK lsock);


/** Get an OS-dependent native socket handle to use by platform-specific API.
 * FYI: on MS-Windows it will be "SOCKET", on other platforms -- "int".
 * @param lsock
 *  [in]  socket handle 
 * @param handle_buf
 *  [in]  pointer to a memory location to store the OS-dependent handle at
 * @param handle_size
 *  The exact(!) size of the expected OS handle
 * @sa
 *  SOCK_GetOSHandle
 */
extern NCBI_XCONNECT_EXPORT EIO_Status LSOCK_GetOSHandle
(LSOCK  lsock,
 void*  handle_buf, 
 size_t  
 );



/******************************************************************************
 *  SOCKET
 */

/** [CLIENT-side]  Connect client to another(server-side, listening) socket
 * (socket() + connect() [+ select()])
 * Equivalent to SOCK_CreateEx(host, port, timeout, sock, 0, 0, eDefault).
 *
 * @param host
 *  [in]  host to connect to 
 * @param port
 *  [in]  port to connect to 
 * @param timeout
 *  [in]  the connect timeout (infinite if NULL)
 * @param sock
 *  [out] handle of the created socket
 * @sa
 *  SOCK_CreateEx, SOCK_Reconnect, SOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Create
(const char*     host,   
 unsigned short  port,   
 const STimeout* timeout, 
 SOCK*           sock     
 );


/** [CLIENT-side]  Connect client to another(server-side, listening) socket
 * (socket() + connect() [+ select()])
 *
 * @param host
 *  [in]  host to connect to 
 * @param port
 *  [in]  port to connect to 
 * @param timeout
 *  [in]  the connect timeout (infinite if NULL)
 * @param sock
 *  [out] handle of the created socket
 * @param init_data
 *  [in]  initial output data segment (may be NULL)
 * @param init_size
 *  [in]  size of initial data segment (may be 0) 
 * @param log
 *  [in]  whether to do logging on this socket
 * @sa
 *  SOCK_Create, SOCK_Reconnect, SOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_CreateEx
(const char*     host,    
 unsigned short  port,    
 const STimeout* timeout,  
 SOCK*           sock,     
 const void*     init_data,
 size_t          init_size,
 ESwitch         log       
 );


/** SOCK_Close behavior for SOCKs created on top of OS handles.
 * @sa
 *  SOCK_Close, SOCK_CreateOnTop, SOCK_CreateOnTopEx
 */
typedef enum {
    eSCOT_KeepOnClose,    /** Do not close "handle" on SOCK_Close */
    eSCOT_CloseOnClose    /** Do close "handle" on SOCK_Close     */
} ESCOT_OnClose;


/** [SERVER-side]  Create a socket on top of OS-dependent "handle".
 * Equivalent to SOCK_CreateOnTopEx(handle, handle_size, sock,
 *                                  0, 0, eDefault, eSCOT_CloseOnClose).
 * @param handle
 *  [in]  OS-dependent "handle" to be converted
 * @param handle_size
 *  [in]  "handle" size
 * @param sock
 *  [out] SOCK built on top of OS "handle"
 * @sa
 *  SOCK_CreateOnTopEx
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_CreateOnTop
(const void*   handle,      
 size_t        handle_size,                       
 SOCK*         sock         
 );


/** [SERVER-side]  Create a socket on top of OS-dependent "handle"
 * (file descriptor on Unix, SOCKET on MS-Windows).  Returned socket
 * is not reopenable to its default peer (SOCK_Reconnect may not specify
 * zeros for the connection point).
 * All timeouts are set to default [infinite] values.
 * SOCK_Close will close the "handle" only if the "close_on_close"
 * parameter is passed non-zero (eSCOT_CloseOnClose).
 * @param handle
 *  [in]  OS-dependent "handle" to be converted
 * @param handle_size
 *  [in]  "handle" size
 * @param sock
 *  [out] SOCK built on top of OS "handle"
 * @param init_data
 *  [in]  initial output data segment (may be NULL)
 * @param init_size
 *  [in]  size of the initial data segment (may be 0)
 * @param log
 *  [in]  data logging for the resulting SOCK 
 * @param on_close
 *  [in]  if to keep "handle" in SOCK_Close
 * @return 
 *  Return eIO_Success on success;  otherwise: eIO_Closed if the "handle"
 *  does not refer to an open socket [but e.g. to a normal file or a pipe];
 *  other error codes in case of other errors.
 * @sa
 *  SOCK_CreateOnTop, SOCK_Reconnect, SOCK_Close
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_CreateOnTopEx
(const void*   handle,      
 size_t        handle_size, 
 SOCK*         sock,       
 const void*   init_data,   
 size_t        init_size,   
 ESwitch       log,         
 ESCOT_OnClose on_close     
 );


/** [CLIENT-side]  Close the socket referred to by "sock" and then connect
 * it to another "host:port";  fail if it takes more than "timeout"
 * (close() + connect() [+ select()])
 *
 * HINT:  if "host" is NULL then connect to the same host address as before;
 *        if "port" is zero then connect to the same port # as before.
 *
 * @li <b>NOTE 1:</b> "new" socket inherits old I/O timeouts;
 * @li <b>NOTE 2:</b> the call is only applicable to stream [not datagram] sockets.
 * @li <b>NOTE 3:</b> "timeout"==NULL is infinite; "timeout"=={0,0} causes no wait for
 *        connection to be established and to return immediately.
 * @li <b>NOTE 4:</b> UNIX sockets can only be reconnected to the same file thus both
 *        host and port have to be passed as 0s.
 * @param sock
 *  [in] handle of the socket to reconnect
 * @param host
 *  [in] host to connect to  (can be NULL)
 * @param port
 *  [in] port to connect to  (can be 0)
 * @param timeout
 *  [in] the connect timeout (infinite if NULL)
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Reconnect
(SOCK            sock,   
 const char*     host,    
 unsigned short  port,    
 const STimeout* timeout  
 );


/** Shutdown the connection in only one direction (specified by "direction").
 * Later attempts to I/O (or to wait) in the shutdown direction will
 * do nothing, and immediately return with "eIO_Closed" status.
 * Pending data output can cause data transfer to the remote end (subject
 * for eIO_Close timeout as previously set by SOCK_SetTimeout()).
 * Cannot be applied to datagram sockets (eIO_InvalidArg results).
 * @param sock
 *  [in] handle of the socket to shutdown  
 * @param how
 *  [in] one of:  eIO_Read, eIO_Write, eIO_ReadWrite
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Shutdown
(SOCK      sock, 
 EIO_Event how   
 );


/** Close the connection, destroy relevant internal data.
 * The "sock" handle goes invalid after this function call, regardless
 * of whether the call was successful or not.
 * @li <b>NOTE 1:</b>  if eIO_Close timeout was specified (or NULL) then it blocks until
 *         either all unsent data are sent, error flagged, or the timeout
 *         expires.
 * @li <b>NOTE 2:</b>  if there is output pending, that output will be flushed.
 * @param sock
 *  [in] Socket
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Close(SOCK sock);


/** Close the connection, and conditionally destroy relevant internal data.
 * @li <b>NOTE 1:</b>  if eIO_Close timeout was specified (or NULL) then it blocks until
 *         either all unsent data are sent, error flagged, or the timeout
 *         expires.
 * @li <b>NOTE 2:</b>  if there is output pending, that output will be flushed.
 * @li <b>NOTE 3:</b>  SOCK_CloseEx(sock, 1) is equivalent to SOCK_Close(sock);
 * @param sock
 *  [in] handle of the socket to close
 * @param destroy 
 *  [in] =1 to destroy handle; =0 to keep handle
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_CloseEx
(SOCK         sock,    
 int/**bool*/ destroy  
 );


/** Block on the socket until either read/write (dep. on the "event" arg) is
 * available or timeout expires (if "timeout" is NULL then assume it infinite).
 * For a datagram socket, eIO_Closed is returned if the internally latched
 * message was entirely read out, and eIO_Read was requested as the "event".
 * Both eIO_Write and eIO_ReadWrite events always immediately succeed for
 * the datagram socket.
 * @param sock
 *  [in] socket handle 
 * @param event
 *  [in] one of:  eIO_Read, eIO_Write, eIO_ReadWrite 
 * @param timeout
 *  [in] maximum time to wait for an event
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Wait
(SOCK            sock,
 EIO_Event       event, 
 const STimeout* timeout
 );


/**
 * @sa SOCK_Poll()
 */
typedef struct {
    SOCK      sock;   /** [in]          SOCK to poll (NULL if not to poll)  */
    EIO_Event event;  /** [in]  one of: eIO_Read, eIO_Write, eIO_ReadWrite  */
    EIO_Event revent; /** [out] one of: eIO_Open/Read/Write/ReadWrite/Close */
} SSOCK_Poll;


/** Block until at least one of the sockets enlisted in "polls" array
 * (of size "n") becomes available for requested operation (event),
 * or until timeout expires (wait indefinitely if timeout is passed NULL).
 * Return eIO_Success if at least one socket was found ready; eIO_Timeout
 * if timeout expired; eIO_Unknown if underlying system call(s) failed.
 * @li <b>NOTE 1:</b> For a socket found not ready for an operation, eIO_Open is returned
 *        in its "revent"; for a failing socket, eIO_Close is returned;
 * @li <b>NOTE 2:</b> This call may return eIO_InvalidArg if
 *        - parameters to the call are inconsistent;
 *        - a non-NULL socket polled with a bad "event" (eIO_Open, eIO_Close).
 *        With this return code, the calling program cannot rely on "revent"
 *        fields the "polls" array as they might not be properly updated.
 * @li <b>NOTE 3:</b> If either both "n" and "polls" are NULL, or all sockets in "polls"
 *        are NULL, then the returned result is either
 *        eIO_Timeout (after the specified amount of time was spent idle), or
 *        eIO_Interrupted (if signal came while the waiting was in progress).
 * @li <b>NOTE 4:</b> For datagram sockets, the readiness for reading is determined by
 *        message data latched since last message receive call (DSOCK_RecvMsg).
 * @li <b>NOTE 5:</b> This call allows intermixture of stream and datagram sockets.
 * @li <b>NOTE 6:</b> This call can cause some socket I/O in those sockets marked for
 *        read-on-write and those with pending connection or output data.
 * @param n
 *  [in] # of SSOCK_Poll elems in "polls"  
 * @param polls[]
 *  [in|out] array of query/result structures   
 * @param timeout
 *  [in] max time to wait (infinite if NULL)
 * @param n_ready
 *  [out] # of ready sockets  (may be NULL) 
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Poll
(size_t          n,         
 SSOCK_Poll      polls[],   
 const STimeout* timeout,   
 size_t*         n_ready    
 );



/** GENERIC POLLABLE INTERFACE, please see above for explanations
 */
struct SPOLLABLE_tag;
typedef struct SPOLLABLE_tag* POLLABLE;

typedef struct {
    POLLABLE  poll;
    EIO_Event event;
    EIO_Event revent;
} SPOLLABLE_Poll;

/**
 * @param n
 * 
 * @param polls[]
 *   
 * @param timeout
 * 
 * @param n_ready
 * 
 */
extern NCBI_XCONNECT_EXPORT EIO_Status POLLABLE_Poll
(size_t          n,
 SPOLLABLE_Poll  polls[],
 const STimeout* timeout,
 size_t*         n_ready
 );


/**
 * @return
 *  Return 0 if conversion cannot be made; otherwise converted handle 
 */
extern NCBI_XCONNECT_EXPORT POLLABLE POLLABLE_FromSOCK   (SOCK);
extern NCBI_XCONNECT_EXPORT POLLABLE POLLABLE_FromLSOCK  (LSOCK);
extern NCBI_XCONNECT_EXPORT POLLABLE POLLABLE_FromTRIGGER(TRIGGER);
extern NCBI_XCONNECT_EXPORT SOCK     POLLABLE_ToSOCK   (POLLABLE);
extern NCBI_XCONNECT_EXPORT LSOCK    POLLABLE_ToLSOCK  (POLLABLE);
extern NCBI_XCONNECT_EXPORT TRIGGER  POLLABLE_ToTRIGGER(POLLABLE);


/** Specify timeout for the connection i/o (see SOCK_[Read|Write|Close] funcs).
 * If "timeout" is NULL then set the timeout to be infinite;
 * @li <b>NOTE:</b> the default timeout is infinite (wait "ad infinitum" on I/O).
 * @param sock
 *  [in]  socket handle 
 * @param event
 *  [in] one of:  eIO_[Read/Write/ReadWrite/Close]
 * @param timeout
 *  [in] new timeout value to set  
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_SetTimeout
(SOCK            sock,
 EIO_Event       event,   
 const STimeout* timeout  
 );


/** Get the connection's i/o timeout (or NULL, if the timeout is infinite).
 * @li <b>NOTE 1:</b>  the returned timeout is guaranteed to be pointing to a valid
 *         (and correct) structure in memory at least until the SOCK is closed
 *         or SOCK_SetTimeout is called for this "sock".
 * @li <b>NOTE 2:</b>  eIO_ReadWrite timeout is the least of eIO_Read and eIO_Write ones.
 * @param sock
 *  [in]  socket handle 
 * @param event
 *  [in]  one of:  eIO_[Read/Write/Close] 
 */
extern NCBI_XCONNECT_EXPORT const STimeout* SOCK_GetTimeout
(SOCK      sock,
 EIO_Event event  
 );


/** Read/peek up to "size" bytes from "sock" to the mem.buffer pointed by "buf".
 * In "*n_read", return the number of successfully read bytes.
 * Read method "how" can be either of the following:
 * eIO_ReadPlain   -- read as many as "size" bytes and return (eIO_Success);
 *                    if no data are readily available then wait at most
 *                    read timeout and return (eIO_Timeout) if no data still
 *                    could be got; eIO_Success if some data were obtained.
 * eIO_ReadPeek    -- same as "eIO_ReadPlain" but do not extract the data from
 *                    the socket (so that the next read operation will see the
 *                    data again), with one important exception noted below.
 * eIO_ReadPersist -- read exactly "size" bytes and return eIO_Success; if less
 *                    data received then return an error condition (including
 *                    eIO_Timeout).
 *
 * If there is no data available to read (also, if eIO_ReadPersist and cannot
 * read exactly "size" bytes) and the timeout(see SOCK_SetTimeout) is expired
 * then return eIO_Timeout.
 *
 * Both eIO_ReadPlain and eIO_ReadPeek return eIO_Success iff some data have
 * been read (perhaps within the time allowance specified by eIO_Read timeout).
 * Both mothods return any other code when no data at all were available.
 * eIO_ReadPersist differs from the other two methods as it can return an
 * error condition even if some data were actually obtained from the socket.
 * Hence, as a rule of thumb, an application should always check the number
 * of read bytes BEFORE checking the return status, which merely advises
 * whether it is okay to read again.
 *
 * As a special case, "buf" may passed as NULL:
 *   eIO_ReadPeek      -- read up to "size" bytes and store them
 *                        in internal buffer;
 *   eIO_Read[Persist] -- discard up to "size" bytes from internal buffer
 *                        and socket (check "*n_read" to know how many).
 *
 * @li <b>NOTE 1:</b> "Read" and "peek" methods differ:  if "read" is performed and not
 *        enough but some data available immediately from the internal buffer,
 *        then the call completes with eIO_Success status.  For "peek", if
 *        not all requested data were available, the real I/O occurs to pick up
 *        additional data (if any) from the system. Keep this difference in
 *        mind when programming loops that heavily use "peek"s without "read"s.
 * @li <b>NOTE 2:</b> If on input "size" == 0, then "*n_read" is set to 0, and the
 *        return value can be either of eIO_Success, eIO_Closed or
 *        eIO_Unknown depending on connection status of the socket.
 * @param sock
 *  [in]  socket handle 
 * @param buf
 *  [out] data buffer to read to 
 * @param size
 *  [in] max # of bytes to read to "buf" 
 * @param n_read
 *  [out] # of bytes read  (can be NULL)
 * @param how
 *  [in] how to read the data 
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Read
(SOCK           sock,
 void*          buf,   
 size_t         size,   
 size_t*        n_read, 
 EIO_ReadMethod how    
 );


/**
 * Read a line from SOCK.  A line is terminated by either '\n' (with
 * optional preceding '\r') or '\0'.  Returned result is always '\0'-
 * terminated and having '\r'(if any)'\n' stripped. *n_read (if 'n_read'
 * passed non-NULL) contains the numbed of characters written into
 * 'buf' (not counting the terminating '\0').  If 'buf', whose size is
 * specified via 'size' parameter, is not big enough to contain the
 * line, all 'size' bytes will be filled, with *n_read == size upon
 * return.  Note that there will be no terminating '\0' in this
 * (and the only) case, which the caller can easily distinguish.
 * @param sock
 *  [in]  socket handle 
 * @param buf
 *  [out] data buffer to read to 
 * @param size
 *  [in] max # of bytes to read to "buf"
 * @param n_read
 *  [out] # of bytes read  (can be NULL) 
 * @return
 *  Return code eIO_Success upon successful completion, other - upon
 *  an error.  Note that *n_read must be analyzed prior to return code,
 *  because the buffer could have received some contents before
 *  the indicated error occurred (especially when connection closed).
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_ReadLine
(SOCK           sock,
 char*          buf,    
 size_t         size,   
 size_t*        n_read  
 );


/** Push the specified data back to the socket input queue (in the socket's
 * internal read buffer). These can be any data, not necessarily the data
 * previously read from the socket.
 * @param sock
 *  [in]  socket handle 
 * @param buf
 *  [in] data to push back to the socket's local buffer 
 * @param size
 *  [in] # of bytes (starting at "buf") to push back 
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_PushBack
(SOCK        sock,
 const void* buf,  
 size_t      size  
 );


/**
 * @param sock
 *  [in]  socket handle 
 * @param direction
 *  [in] one of:  eIO_Open, eIO_Read, eIO_Write
 * @return
 *  Return (for the specified "direction" [eIO_Open to check for closed sock]):
 *   eIO_Closed     -- if the connection was shutdown by SOCK_Shutdown(), or
 *                     (for "eIO_Read" only) if EOF was detected
 *                     if "direction"==eIO_Open, this code means socket closed
 *   eIO_Unknown    -- if an error was detected during the last I/O
 *   eIO_InvalidArg -- if "direction" is not one of:  Open, Read, Write
 *   eIO_Timeout    -- if the socket is not yet actually connected
 *   eIO_Success    -- otherwise (incl. eIO_Timeout on last I/O)
 *
 * @li <b>NOTE:</b>  The SOCK_Read() and SOCK_Wait(eIO_Read) will not return any error
 *        as long as there is any unread (buffered) data left.
 *        Thus, when you are "peeking" data instead of actually reading it,
 *        then this is the only "non-destructive" way to check whether EOF
 *        or an error has actually occurred on read.
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Status
(SOCK      sock,
 EIO_Event direction  
 );


/** Write "size" bytes from buffer "buf" to "sock".
 * @param sock
 *  [in]  socket handle 
 * @param buf
 *  [in] data to write to the socket 
 * @param size
 *  [in] # of bytes (starting at "buf") to write 
 * @param n_written
 *  [out] # of written bytes (can be NULL)
 * @param how
 *  [in] eIO_WritePlain | eIO_WritePersist
 * @return
 *  In "*n_written", return the number of bytes actually written.
 *  eIO_WritePlain   --  write as many bytes as possible at once and return
 *                      immediately; if no bytes can be written then wait
 *                      at most WRITE timeout, try again and return.
 *  eIO_WritePersist --  write all data (doing an internal retry loop
 *                      if necessary); if any single write attempt times out
 *                      or fails then stop writing and return (error code).
 *  Return status: eIO_Success -- some bytes were written successfully  [Plain]
 *                            -- all bytes were written successfully [Persist]
 *                other code denotes an error, but some bytes might have
 *                been sent nevertheless (always check *n_written to know).
 *
 * @li <b>NOTE 1:</b> With eIO_WritePlain the call returns eIO_Success iff some data
 *        were actually written to the socket. If no data could be written
 *        (and perhaps timeout expired) this call always returns an error.
 * @li <b>NOTE 2:</b> eIO_WritePlain and eIO_WritePersist differs that the latter can
 *        flag an error condition even if some data were actually written
 *        (see "the rule of thumb" in the comments for SOCK_Read() above).
 * @li <b>NOTE 3:</b> if "size"==0, return value can be eIO_Success if no pending data
 *        left in the socket, or eIO_Timeout if there are still data pending.
 *        In either case, "*n_written" is set to 0 on return.
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Write
(SOCK            sock,
 const void*     buf,       
 size_t          size,      
 size_t*         n_written, 
 EIO_WriteMethod how        
 );


/** If there is outstanding connection or output data pending, cancel it.
 * Mark the socket as if it has been shut down for both reading and writing.
 * Break actual connection if any was established.
 * Do not attempt to send anything upon SOCK_Close().
 * This call is available for stream sockets only.
 * @param sock
 * Socket handle
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_Abort
(SOCK sock
 );


/** Get local port of the socket.
 * @param sock
 *  [in] socket handle 
 * @param byte_order
 *  [in] port byte order
 * @return
 *  If "network_byte_order" is true(non-zero) then return the port in the
 *  network byte order; otherwise return it in the local host byte order.
 */
extern NCBI_XCONNECT_EXPORT unsigned short SOCK_GetLocalPort
(SOCK            sock,
 ENH_ByteOrder   byte_order          
 );


/** Get host and port of the socket's peer.
 * @param sock
 *  [in]  socket handle 
 * @param host
 *  [out] the peer's host (can be NULL) 
 * @param port
 *  [out] the peer's port (can be NULL)
 * @param byte_order
 *  [in] host/port byte order
 * @return
 *  If "network_byte_order" is true(non-zero) then return the host/port in the
 *  network byte order; otherwise return them in the local host byte order.
 */
extern NCBI_XCONNECT_EXPORT void SOCK_GetPeerAddress
(SOCK            sock,
 unsigned int*   host,               
 unsigned short* port,                
 ENH_ByteOrder   byte_order          
 );


/** Get textual representation of the socket's peer.
 * For INET domain sockets, the result is of the form "aaa.bbb.ccc.ddd:ppppp";
 * for UNIX domain socket, the result is the name of the socket's file.
 * @param sock
 *  [in]  socket handle 
 * @param buf
 *  [out] pointer to provided buffer to store the text to
 * @param buflen
 *  [in] usable size of the buffer above 
 * @return
 *  On success, return its "buf" argument; return 0 on error.
*/
extern NCBI_XCONNECT_EXPORT char* SOCK_GetPeerAddressString
(SOCK   sock,
 char*  buf,       
 size_t buflen     
 );


/** Get an OS-dependent native socket handle to use by platform-specific API.
 * FYI:  on MS-Windows it will be "SOCKET", on other platforms -- "int".
 * @param sock
 *   Socket handle
 * @param handle_buf
 *  pointer to a memory area to put the OS handle at
 * @param handle_size
 *  the exact(!) size of the expected OS handle
 */
extern NCBI_XCONNECT_EXPORT EIO_Status SOCK_GetOSHandle
(SOCK   sock,
 void*  handle_buf,  
 size_t handle_size  
 );


/** By default ("on_off" == eDefault,eOff), sockets will not try to read data
 * from inside SOCK_Write(). If you want to automagically upread the data
 * (and cache it in the internal socket buffer) when the write operation
 * is not immediately available, call this func with "on_off" == eOn.
 * @param on_off
 * 
 * @return
 *  Prior setting.
*/
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetReadOnWriteAPI
(ESwitch on_off
 );


/** Control the reading-while-writing feature for socket "sock" individually.
 * To reset to the global default behavior (as set by
 * SOCK_SetReadOnWriteAPI), call this function with "on_off" == eDefault.
 * @param sock
 *  [in]  socket handle 
 * @param on_off
 *  
 * @return
 *  Prior setting.
 */
extern NCBI_XCONNECT_EXPORT ESwitch SOCK_SetReadOnWrite
(SOCK    sock,
 ESwitch on_off
 );


/** Control OS-defined send strategy by disabling/enabling TCP
 * Nagle algorithm that packs multiple requests into a single
 * frame and thus transferring data in fewer transactions,
 * miminizing the network traffic and bursting the throughput.
 * Some applications, however, may find it useful to disable this
 * default behavior for the sake of their performance increase
 * (like in case of short transactions otherwise held by the system
 * to be possibly coalesced into larger chunks).
 * @param sock
 *  [in]  socket handle 
 * @param on_off
 *  
 * NB: use true to disable; false to enable
 */
extern NCBI_XCONNECT_EXPORT void SOCK_DisableOSSendDelay
(SOCK         sock,
 int/**bool*/ on_off  
 );



/******************************************************************************
 *  Connectionless (datagram) sockets
 *
 *  How the datagram exchange API works:
 *
 *  Datagram socket is created with special DSOCK_Create[Ex] calls but the
 *  resulting object is a SOCK handle. That is, almost all SOCK routines
 *  may be applied to the handle. There are few exceptions, though.
 *  In datagram sockets I/O differs from how it is done in stream sockets:
 *
 *  SOCK_Write() writes data into an internal message buffer, appending new
 *  data as they come with each SOCK_Write(). When the message is complete,
 *  SOCK_SendMsg() should be called (optionally with additional last,
 *  or the only [if no SOCK_Write() preceded the call] message fragment)
 *  to actually send the message down the wire. If successful, SOCK_SendMsg()
 *  cleans the internal buffer, and the process may repeat. If unsuccessful,
 *  SOCK_SendMsg() can be repeated with restiction that no additional data are
 *  provided in the call. This way, the entire message will be attempted to
 *  be sent again. On the other hand, if after any SOCK_SendMsg() new data
 *  are added [regardless of whether previous data were successfully sent
 *  or not], all previously written [and kept in the internal send buffer]
 *  data get dropped and replaced with the new data.
 *
 *  DSOCK_WaitMsg() can be used to learn whether there is a new message
 *  available for read by DSOCK_RecvMsg() immediately.
 *
 *  SOCK_RecvMsg() receives the message into an internal receive buffer,
 *  and optionally can return the initial datagram fragment via provided
 *  buffer [this initial fragment is then stripped from what remains unread
 *  in the internal buffer]. Optimized version can supply a maximal message
 *  size (if known in advance), or 0 to get a message of any allowed size.
 *  The actual size of the received message can be obtained via a
 *  pointer-type argument 'msgsize'. The message kept in the internal buffer
 *  can be read out in several SOCK_Read() calls, last returning eIO_Closed,
 *  when all data have been taken out. SOCK_Wait() returns eIO_Success while
 *  there are data in the internal message buffer that SOCK_Read() can read.
 *
 *  SOCK_WipeMsg() can be used to clear the internal message buffers in
 *  either eIO_Read or eIO_Write directions, meaning receive and send
 *  buffers correspondingly.
 */

/**
 * @param sock
 *  [out] socket created
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_Create
(SOCK*           sock                                
 );

/**
 * @param sock
 *  [out] socket created
 * @param log
 *  [in] whether to log data activity
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_CreateEx
(SOCK*           sock,               
 ESwitch         log                 
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param port
 *  [in] port to bind to (!=0)
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_Bind
(SOCK            sock,                  
 unsigned short  port                  
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param host
 *  [in] peer host
 * @param port
 *  [in] peer port
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_Connect
(SOCK            sock,                  
 const char*     host,                 
 unsigned short  port                   
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param timeout
 *  [in] time to wait for message
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_WaitMsg
(SOCK            sock,                 
 const STimeout* timeout            
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param host
 *  [in] hostname or dotted IP
 * @param port
 *  [in] port number, host byte order
 * @param data
 *  [in] additional data to send
 * @param datalen
 *  [in] size of additional data (bytes)
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_SendMsg
(SOCK            sock,                
 const char*     host,                 
 unsigned short  port,           
 const void*     data,                 
 size_t          datalen               
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param buf
 *  [in] buf to store msg at,m.b.NULL
 * @param buflen
 *  [in]  buf length provided 
 * @param maxmsglen
 *  [in] maximal expected message len
 * @param msglen
 *  [out] actual msg size, may be NULL
 * @param sender_addr
 *  [out] net byte order, may be NULL
 * @param sender_port
 *  [out] host byte order, may be NULL
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_RecvMsg
(SOCK            sock,                 
 void*           buf,                  
 size_t          buflen,                
 size_t          maxmsglen,             
 size_t*         msglen,                
 unsigned int*   sender_addr,           
 unsigned short* sender_port            
);

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param direction
 *  [in] either of eIO_Read|eIO_Write
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_WipeMsg
(SOCK            sock,                 
 EIO_Event       direction             
 );

/**
 * @param sock
 *  [in] SOCK from DSOCK_Create[Ex]()
 * @param broadcast
 *  [in] set(1)/unset(0) bcast capab
 */
extern NCBI_XCONNECT_EXPORT EIO_Status DSOCK_SetBroadcast
(SOCK            sock,                  
 int/**bool*/    broadcast             
 );



/******************************************************************************
 *  Type information for SOCK sockets
 */


/**
 * @param sock
 *  [in]  socket handle 
 * @return 
 *  Non-zero value if socket "sock" was created by DSOCK_Create[Ex]().
 *  Return zero otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/**bool*/ SOCK_IsDatagram(SOCK sock);


/**
 * @param sock
 *  [in]  socket handle 
 * @return
 *  Non-zero value if socket "sock" was created by SOCK_Create[Ex]().
 *  Return zero otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/**bool*/ SOCK_IsClientSide(SOCK sock);


/**
 * @param sock
 *  [in]  socket handle 
 * @return
 *  Non-zero value if socket "sock" was created by LSOCK_Accept().
 *  Return zero otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/**bool*/ SOCK_IsServerSide(SOCK sock);


/**
 * @param sock
 *  [in]  socket handle 
 * @return
 *  Non-zero value if socket "sock" was created by LSOCK_Accept().
 *  Return zero otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/**bool*/ SOCK_IsUNIX(SOCK sock);



/******************************************************************************
 *  AUXILIARY network-specific functions (added for the portability reasons)
 */


/**
 * @param name
 *  [out] (guaranteed to be '\0'-terminated)
 * @param namelen
 *  [in] max # of bytes allowed to put to "name" 
 * @return
 *  Zero on success, non-zero on error.  See BSD gethostname().
 *  On error "name" returned emptied (name[0] == '\0').
 */
extern NCBI_XCONNECT_EXPORT int SOCK_gethostname
(char*  name,
 size_t namelen
 );


/**
 * @param addr
 *  [in] must be in the network byte-order
 * @param buf
 *  [out] to be filled by smth. like "123.45.67.89\0"
 * @param buflen
 *  [in] max # of bytes to put to "buf"
 * @return
 *  Zero on success, non-zero on error.  Vaguely related to BSD's
 *  inet_ntoa(). On error "buf" returned emptied (buf[0] == '\0').
 */
extern NCBI_XCONNECT_EXPORT int SOCK_ntoa
(unsigned int addr,
 char*        buf,
 size_t       buflen
 );


/**
 * @param host
 *  [in] '\0'-terminated string to check against being a plain IP address
 * @param fullquad
 *  [in] non-zero to only accept "host" if it is a full-quad IP notation
 * @return
 *  Non-zero (true) if given string is an IP address, zero (false) otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/*bool*/ SOCK_isipEx
(const char* host,
 int/*bool*/ fullquad
 );


/** Equivalent of SOCK_isip(host, 0)
 * @param host
 *  [in] '\0'-terminated string to check against being a plain IP address
 * @return
 *  Non-zero (true) if given string is an IP address, zero (false) otherwise.
 */
extern NCBI_XCONNECT_EXPORT int/*bool*/ SOCK_isip
(const char* host
 );


/** See man for the BSDisms, htonl() and htons().
 * @param value
 * 
 */
extern NCBI_XCONNECT_EXPORT unsigned int SOCK_HostToNetLong
(unsigned int value
 );

#define SOCK_NetToHostLong SOCK_HostToNetLong

/** 
 * @param value
 * 
 */
extern NCBI_XCONNECT_EXPORT unsigned short SOCK_HostToNetShort
(unsigned short value
 );

#define SOCK_NetToHostShort SOCK_HostToNetShort


/* Deprecated:  Use SOCK_{Host|Net}To{Net|Host}{Long|Short}() instead */
#ifndef NCBI_DEPRECATED
#  define NCBI_SOCK_DEPRECATED
#else
#  define NCBI_SOCK_DEPRECATED NCBI_DEPRECATED
#endif
extern NCBI_XCONNECT_EXPORT NCBI_SOCK_DEPRECATED
unsigned int   SOCK_htonl(unsigned int);
#define        SOCK_ntohl SOCK_htonl
extern NCBI_XCONNECT_EXPORT NCBI_SOCK_DEPRECATED
unsigned short SOCK_htons(unsigned short);
#define        SOCK_ntohs SOCK_htons


/** 
 * @param hostname
 *  [in] return current host address if hostname is 0 
 * @return
 *  INET host address (in network byte order) of the
 *  specified host (or local host, if hostname is passed as NULL),
 *  which can be either domain name or an IP address in
 *  dotted notation (e.g. "123.45.67.89\0"). Return 0 on error.
 *  @li <b>NOTE:</b> "0.0.0.0" and "255.255.255.255" are considered invalid.
 */
extern NCBI_XCONNECT_EXPORT unsigned int SOCK_gethostbyname
(const char* hostname \
 );


/** Take INET host address (in network byte order) and fill out the
 * the provided buffer with the name, which the address corresponds to
 * (in case of multiple names the primary name is used).
 * @param addr
 *  [in] host address in network byte order
 * @param name
 *  [out] buffer to put the name to 
 * @param namelen
 *  [in] size (bytes) of the buffer above 
 * @return
 *  Value 0
 *  means error, while success is denoted by the 'name' argument returned.
 *  Note that on error the name returned emptied (name[0] == '\0').
 */
extern NCBI_XCONNECT_EXPORT char* SOCK_gethostbyaddr
(unsigned int addr,  
 char*        name,   
 size_t       namelen  
 );


/**
 * @return
 *  Loopback address (in network byte order).
 */
extern NCBI_XCONNECT_EXPORT unsigned int SOCK_GetLoopbackAddress(void);


/** Get (and cache for faster follow-up retrievals) the address of local host
 * @param reget
 *  eOn      to forcibly recache and return the address;
 *  eDefault to recache only if unknown, return the cached value;
 *  eOff     not to recache even if unknown, return whatever is available.
 * @return
 *  Local address (in network byte order).
 */
extern NCBI_XCONNECT_EXPORT unsigned int SOCK_GetLocalHostAddress
(ESwitch reget
 );


/** Read (skipping leading blanks) "[host][:port]" from a string.
 * @param str
 *  must not be NULL
 * @param host
 *  must not be NULL
 * @param port
 *  must not be NULL
 * @return
 *  On success, return the advanced pointer past the host/port read.
 *  If no host/port detected, return 'str'.
 *  On format error, return 0.
 *  If host and/or port fragments are missing,
 *  then corresponding 'host'/'port' value returned as 0.
 *  Note that 'host' returned is in network byte order,
 *  unlike 'port', which always comes out in host (native) byte order.
 */
extern NCBI_XCONNECT_EXPORT const char* SOCK_StringToHostPort
(const char*     str,  
 unsigned int*   host,  
 unsigned short* port   
 );


/** Print host:port into provided buffer string, not to exceed 'buflen' size.
 * Suppress printing host if parameter 'host' is zero.
 * @param host
 * 
 * @param port
 * 
 * @param buf
 * 
 * @param buflen
 * 
 * @return
 *  Number of bytes printed.
 */
extern NCBI_XCONNECT_EXPORT size_t SOCK_HostPortToString
(unsigned int   host,
 unsigned short port,
 char*          buf,
 size_t         buflen
 );


#ifdef __cplusplus
} /* extern "C" */
#endif


/* @} */

#endif /* CONNECT___NCBI_SOCKET__H */
