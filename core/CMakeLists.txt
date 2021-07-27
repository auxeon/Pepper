project(core C CXX)

set(core_SOURCES
    "ps_chronon.c"
    "ps_events.c"
    "ps_graphics.c"
)

set(core_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_chronon.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_engine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_events.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_graphics.h"
)

add_library(core STATIC ${core_SOURCES} ${core_INCLUDES})
target_include_directories(core PUBLIC ${core_INCLUDES})
target_link_libraries(core
    glad
    glfw
)


set_target_properties(core
PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${core_INCLUDES}"
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)