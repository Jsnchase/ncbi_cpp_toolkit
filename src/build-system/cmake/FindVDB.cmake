IF (UNIX AND NOT APPLE)
  if (EXISTS /opt/ncbi/64/trace_software/vdb/vdb-versions/2.8.0/interfaces)
    set(VDB_FOUND true)
    set(VDB_INCLUDE /opt/ncbi/64/trace_software/vdb/vdb-versions/2.8.0/interfaces)
    set(VDB_LIB "")
    set(VDB_LIBS -L/opt/ncbi/64/trace_software/vdb/vdb-versions/2.8.0/linux/release/x86_64/lib -Wl,-rpath,/opt/ncbi/64/trace_software/vdb/vdb-versions/cxx_toolkit/2/linux/release/x86_64/lib -lncbi-vdb -L/netopt/ncbi_tools64/libxml-2.7.8/GCC401-Release64/lib -Wl,-rpath,/opt/ncbi/64/libxml-2.7.8/GCC401-Release64/lib:/netopt/ncbi_tools64/libxml-2.7.8/GCC401-Release64/lib -lxml2  -lnsl -lbz2  -lz  -ldl)
    set(VDB_STATIC_LIBS -L/opt/ncbi/64/trace_software/vdb/vdb-versions/2.8.0/linux/release/x86_64/lib -Wl,-rpath,/opt/ncbi/64/trace_software/vdb/vdb-versions/cxx_toolkit/2/linux/release/x86_64/lib -lncbi-vdb -L/netopt/ncbi_tools64/libxml-2.7.8/GCC401-Release64/lib -Wl,-rpath,/opt/ncbi/64/libxml-2.7.8/GCC401-Release64/lib:/netopt/ncbi_tools64/libxml-2.7.8/GCC401-Release64/lib -lxml2  -lnsl -lbz2  -lz  -ldl)

  endif ()
ENDIF()

IF (APPLE)
  if (EXISTS /net/snowman/vol/projects/trace_software/vdb/vdb-versions/2.8.0/mac/debug/x86_64/lib)
    set(VDB_FOUND true)
    set(VDB_INCLUDE /net/snowman/vol/projects/trace_software/vdb/vdb-versions/2.8.0/interfaces /net/snowman/vol/projects/trace_software/vdb/vdb-versions/2.8.0/interfaces/cc/gcc)
    set(VDB_LIB "")
    set(VDB_LIBS -L/net/snowman/vol/projects/trace_software/vdb/vdb-versions/2.8.0/mac/debug/x86_64/lib -Wl,-rpath,/net/snowman/vol/projects/trace_software/vdb/vdb-versions/cxx_toolkit/2/mac/debug/x86_64/lib -lncbi-vdb -L/netopt/ncbi_tools/libxml-2.7.8/GCC401-DebugMTUniv/lib -Wl,-rpath,/netopt/ncbi_tools/libxml-2.7.8/GCC401-DebugMTUniv/lib -lxml2 -liconv  -lbz2  -lz)
    set(VDB_STATIC_LIBS -L/net/snowman/vol/projects/trace_software/vdb/vdb-versions/2.8.0/mac/debug/x86_64/lib -Wl,-rpath,/net/snowman/vol/projects/trace_software/vdb/vdb-versions/cxx_toolkit/2/mac/debug/x86_64/lib -lncbi-vdb -L/netopt/ncbi_tools/libxml-2.7.8/GCC401-DebugMTUniv/lib -Wl,-rpath,/netopt/ncbi_tools/libxml-2.7.8/GCC401-DebugMTUniv/lib -lxml2 -liconv  -lbz2  -lz)
    set(VDB_REQ VDB)
    set(VDB_POST_LINK install_name_tool -change @executable_path/../lib/libncbi-vdb.2.8.0.dylib @rpath/libncbi-vdb.2.dylib)
  endif ()
ENDIF ()

if (VDB_FOUND)
    set(SRA_INCLUDE ${VDB_INCLUDE})
    set(SRA_SDK_SYSLIBS ${VDB_LIBS})
    set(SRA_SDK_LIBS ${VDB_LIBS})
    set(SRAXF_LIBS ${SRA_SDK_LIBS})
    set(SRA_LIBS ${SRA_SDK_LIBS})
    set(BAM_LIBS ${SRA_SDK_LIBS})
    set(SRAREAD_LDEP ${SRA_SDK_LIBS})
    set(SRAREAD_LIBS sraread ${SRA_SDK_LIBS})
    set(HAVE_NCBI_VDB 1)
endif ()  