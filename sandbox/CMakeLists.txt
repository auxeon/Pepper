project(sandbox C CXX)

set(sandbox_SOURCES
    "main.c"
)
set(sandbox_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../ps_types.h"
)

add_executable(sandbox ${sandbox_SOURCES} ${sandbox_INCLUDES})

target_link_libraries(sandbox
    core
    glad
    glfw
)

target_include_directories(sandbox PUBLIC ${sandbox_INCLUDES})

set_target_properties(sandbox
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${sandbox_INCLUDES}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)