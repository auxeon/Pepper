project(miniaudio C CXX)

set(miniaudio_SOURCES
    ""
)

set(miniaudio_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/miniaudio.h"
    # "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(miniaudio INTERFACE ${miniaudio_SOURCES} ${miniaudio_INCLCUDES})

target_include_directories(miniaudio
    INTERFACE
    "${miniaudio_INCLUDES}"
)

#set_target_properties(miniaudio
#    PROPERTIES
#    INTERFACE_INCLUDE_DIRECTORIES "${miniaudio_INCLUDES}"
#    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#)
