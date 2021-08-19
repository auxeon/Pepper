project(miniaudio C)

set(miniaudio_SOURCES
    ""
)

set(miniaudio_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/miniaudio.h"
)

add_library(miniaudio INTERFACE ${miniaudio_SOURCES} ${miniaudio_INCLCUDES})

target_include_directories(miniaudio
    INTERFACE
    "${miniaudio_INCLUDES}"
)