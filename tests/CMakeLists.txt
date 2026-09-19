find_package(sparkle CONFIG REQUIRED COMPONENTS TestLibrary)
find_package(GTest CONFIG REQUIRED)

set(PLAYGROUND_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE PLAYGROUND_TEST_SOURCES
    CONFIGURE_DEPENDS
    "${PLAYGROUND_TEST_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE PLAYGROUND_TEST_HEADERS
    CONFIGURE_DEPENDS
    "${PLAYGROUND_TEST_SOURCE_DIR}/*.hpp"
    "${PLAYGROUND_TEST_SOURCE_DIR}/*.h"
)

if(NOT WIN32)
    list(FILTER PLAYGROUND_TEST_SOURCES EXCLUDE REGEX "/chunk_golden_tests\\.cpp$")
endif()

#
# Playground Test Library
#

add_library(PlaygroundTestLibrary STATIC
    ${PLAYGROUND_TEST_SOURCES}
    ${PLAYGROUND_TEST_HEADERS}
)

target_compile_features(PlaygroundTestLibrary PUBLIC cxx_std_23)

target_include_directories(PlaygroundTestLibrary PUBLIC
    ${PLAYGROUND_TEST_SOURCE_DIR}
)

target_link_libraries(PlaygroundTestLibrary PRIVATE
    PlaygroundCore
    Sparkle::TestLibrary
    GTest::gtest
)

target_compile_definitions(PlaygroundTestLibrary PRIVATE
    VOXEL_TEST_RESOURCE_DIR="${PROJECT_SOURCE_DIR}/resources"
    PLAYGROUND_TEST_RESOURCE_DIR="${PLAYGROUND_TEST_SOURCE_DIR}/resources"
    PLAYGROUND_TEST_RESULT_DIR="${CMAKE_CURRENT_BINARY_DIR}/results"
)

set_target_properties(PlaygroundTestLibrary PROPERTIES
    CXX_EXTENSIONS OFF
)

configure_debug_symbols(PlaygroundTestLibrary)

#
# Playground Test Executable
#

set(PLAYGROUND_TEST_RUNNER_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/playground_test_runner.cpp")
file(GENERATE OUTPUT "${PLAYGROUND_TEST_RUNNER_SOURCE}" CONTENT "// Entry point is provided by GTest::gtest_main.\n")

add_executable(PlaygroundTest
    ${PLAYGROUND_TEST_RUNNER_SOURCE}
)

target_compile_features(PlaygroundTest PRIVATE cxx_std_23)

target_link_libraries(PlaygroundTest PRIVATE
    "$<LINK_LIBRARY:WHOLE_ARCHIVE,PlaygroundTestLibrary>"
    GTest::gtest_main
)

set_target_properties(PlaygroundTest PROPERTIES
    CXX_EXTENSIONS OFF
)

configure_debug_symbols(PlaygroundTest)

add_test(
    NAME playground_cpu_tests
    COMMAND PlaygroundTest --gtest_filter=-CurrentChunkGoldenTest.*
)

set_tests_properties(playground_cpu_tests PROPERTIES
    LABELS "cpu;headless"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

if(WIN32)
    add_test(
        NAME playground_gpu_tests
        COMMAND PlaygroundTest --gtest_filter=CurrentChunkGoldenTest.*
    )

    set_tests_properties(playground_gpu_tests PROPERTIES
        LABELS "gpu;golden;windows-opengl"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif()
