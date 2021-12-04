cmake_minimum_required(VERSION 3.8)
project(Pepper C)

set(pepper_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_single.c"
)

set(pepper_INCLUDES 
    "${CMAKE_CURRENT_SOURCE_DIR}/ps_single.h"
)

# Location of additional cmake modules
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_MODULE_PATH}"
)

if (MSVC)
    set(COMPILER_OPTIONS
        "-Wall"
        "-O3"
        "-Oi"
        )
    set(LINKER_OPTIONS
    )
else()
    set(COMPILER_OPTIONS
        "-Wall"
        "-O3"
        )
    set(LINKER_OPTIONS
        "-mavx"
    )
endif()

add_compile_options(${COMPILE_OPTIONS})
add_link_options(${LINKER_OPTIONS})
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES FALSE)

# add_executable(pepper ${pepper_SOURCES} ${pepper_INCLUDES})
# target_include_directories(pepper PUBLIC ${pepper_INCLUDES})
# target_link_libraries(pepper
#     core
#     glad
#     glfw
# )
# set_target_properties(pepper
#     PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES "${pepper_INCLUDES}"
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# )

add_subdirectory(core)
add_subdirectory(lib)
add_subdirectory(sandbox)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
endif()
