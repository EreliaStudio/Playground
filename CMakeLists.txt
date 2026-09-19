cmake_minimum_required(VERSION 3.25)

project(
    Playground
    VERSION 0.1.0
    DESCRIPTION "Executable using the installed Sparkle package"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CTest)

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

set(PLAYGROUND_MAIN_SOURCE "${PLAYGROUND_SOURCE_DIR}/main.cpp")

list(FILTER PLAYGROUND_SOURCES EXCLUDE REGEX "/main\\.cpp$")

function(configure_debug_symbols target)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:RelWithDebInfo>:-gcodeview>
            $<$<CONFIG:Release>:-gcodeview>
        )

        get_target_property(TARGET_TYPE ${target} TYPE)
        if(NOT TARGET_TYPE STREQUAL "STATIC_LIBRARY")
            target_link_options(${target} PRIVATE
                $<$<CONFIG:RelWithDebInfo>:-Wl,/DEBUG>
                $<$<CONFIG:Release>:-Wl,/DEBUG>
            )
        endif()
    elseif(MSVC)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:RelWithDebInfo>:/Zi>
            $<$<CONFIG:Release>:/Zi>
        )

        get_target_property(TARGET_TYPE ${target} TYPE)
        if(NOT TARGET_TYPE STREQUAL "STATIC_LIBRARY")
            target_link_options(${target} PRIVATE
                $<$<CONFIG:RelWithDebInfo>:/DEBUG>
                $<$<CONFIG:Release>:/DEBUG>
            )
        endif()
    endif()
endfunction()

#
# Playground Core
#

add_library(PlaygroundCore STATIC
    ${PLAYGROUND_SOURCES}
    ${PLAYGROUND_HEADERS}
)

target_compile_features(PlaygroundCore PUBLIC cxx_std_23)

target_include_directories(PlaygroundCore PUBLIC
    ${PLAYGROUND_INCLUDE_DIR}
)

target_link_libraries(PlaygroundCore PUBLIC
    sparkle::sparkle
)

set_target_properties(PlaygroundCore PROPERTIES
    CXX_EXTENSIONS OFF
)

configure_debug_symbols(PlaygroundCore)

#
# Playground Executable
#

add_executable(Playground
    ${PLAYGROUND_MAIN_SOURCE}
)

target_compile_features(Playground PRIVATE cxx_std_23)

target_link_libraries(Playground PRIVATE
    PlaygroundCore
)

set_target_properties(Playground PROPERTIES
    CXX_EXTENSIONS OFF
)

configure_debug_symbols(Playground)

add_custom_command(TARGET Playground POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Playground>/resources"
)

#
# Tests
#

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

#
# Installation
#

install(
    TARGETS Playground
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
