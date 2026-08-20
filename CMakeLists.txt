cmake_minimum_required(VERSION 3.25)

project(
    Playground
    VERSION 0.1.0
    DESCRIPTION "Executable using the installed Sparkle package"
    LANGUAGES CXX
)

include(GNUInstallDirs)

find_package(sparkle CONFIG REQUIRED)

set(PLAYGROUND_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/srcs")
set(PLAYGROUND_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")

file(GLOB_RECURSE PLAYGROUND_SOURCES
    CONFIGURE_DEPENDS
    "${PLAYGROUND_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE PLAYGROUND_HEADERS
    CONFIGURE_DEPENDS
    "${PLAYGROUND_INCLUDE_DIR}/*.hpp"
    "${PLAYGROUND_INCLUDE_DIR}/*.h"
)

add_executable(Playground
    ${PLAYGROUND_SOURCES}
    ${PLAYGROUND_HEADERS}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Playground PRIVATE
        $<$<CONFIG:RelWithDebInfo>:-gcodeview>
        $<$<CONFIG:Release>:-gcodeview>
    )

    target_link_options(Playground PRIVATE
        $<$<CONFIG:RelWithDebInfo>:-Wl,/DEBUG>
        $<$<CONFIG:Release>:-Wl,/DEBUG>
    )
elseif(MSVC)
    target_compile_options(Playground PRIVATE
        $<$<CONFIG:RelWithDebInfo>:/Zi>
        $<$<CONFIG:Release>:/Zi>
    )

    target_link_options(Playground PRIVATE
        $<$<CONFIG:RelWithDebInfo>:/DEBUG>
        $<$<CONFIG:Release>:/DEBUG>
    )
endif()

target_compile_features(Playground PRIVATE cxx_std_23)
target_include_directories(Playground PRIVATE
    ${PLAYGROUND_INCLUDE_DIR}
)
target_link_libraries(Playground PRIVATE sparkle::sparkle)

set_target_properties(Playground PROPERTIES
    CXX_EXTENSIONS OFF
)

install(
    TARGETS Playground
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
