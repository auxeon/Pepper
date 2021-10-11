project(glad C)

set(glad_SOURCES
    src/glad.c
)

set(glad_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(glad STATIC ${glad_SOURCES} ${glad_INCLCUDES})

target_include_directories(glad
    PUBLIC
    "${glad_INCLUDES}"
)

set_target_properties(glad
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${glad_INCLUDES}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)