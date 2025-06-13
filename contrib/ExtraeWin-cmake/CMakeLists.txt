option (ENABLE_EXTRAE_PROFILING "Enable profiling with ExtraeWin" OFF)

if (ENABLE_EXTRAE_PROFILING)
    message(STATUS "Using ExtraeWin Profiler")
else ()
    message(STATUS "Not using ExtraeWin Profiler")
endif()

set(EXTRAEWIN_INCLUDE_DIR "${ClickHouse_SOURCE_DIR}/contrib/ExtraeWin")
add_library(_extraewin INTERFACE)
target_include_directories(_extraewin SYSTEM BEFORE INTERFACE ${EXTRAEWIN_INCLUDE_DIR})
add_library(ch_contrib::extraewin ALIAS _extraewin)
