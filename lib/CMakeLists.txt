project(lib C CXX)

set(lib_SOURCES
    # "${CMAKE_CURRENT_SOURCE_DIR}/ps_hashmap.c"
    ""
)

set(lib_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    #"${CMAKE_CURRENT_SOURCE_DIR}/ps_hashmap.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/ps_logging.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/ps_mergesort.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/ps_vector.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/miniaudio/miniaudio.h"
)

include_directories(${lib_INCLUDES})

# below are needed only if i add .c source files into lib dir as well

# add_library(lib STATIC ${lib_SOURCES} ${lib_INCLUDES})
# target_include_directories(lib PUBLIC ${lib_INCLUDES})


# set_target_properties(lib
#     PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES "${lib_INCLUDES}"
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#     LINKER_LANGUAGE "C"
# )

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(miniaudio)
