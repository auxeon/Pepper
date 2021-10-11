project(sandbox C)

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
    miniaudio
)

target_include_directories(sandbox PUBLIC ${sandbox_INCLUDES})

set_target_properties(sandbox
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${sandbox_INCLUDES}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/daft_punk_get_lucky.mp3 ${CMAKE_BINARY_DIR}/bin/daft_punk_get_lucky.mp3
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3 ${CMAKE_BINARY_DIR}/bin/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3
)