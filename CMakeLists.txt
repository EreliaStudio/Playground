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

add_custom_command(TARGET Playground POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:Playground>/resources"
)

set_target_properties(Playground PROPERTIES
    CXX_EXTENSIONS OFF
)

if(BUILD_TESTING)
    set(VOXEL_WORLD_TEST_SOURCES
        srcs/voxel/chunk.cpp
        srcs/voxel/chunk_bake_scheduler.cpp
        srcs/voxel/chunk_baker.cpp
        srcs/voxel/chunk_baker_occlusion.cpp
        srcs/voxel/chunk_collection.cpp
        srcs/voxel/chunk_coordinate_hash.cpp
        srcs/voxel/chunk_editor.cpp
        srcs/voxel/chunk_generator.cpp
        srcs/voxel/chunk_requester.cpp
        srcs/voxel/debug_chunk_generator.cpp
        srcs/voxel/voxel_catalog.cpp
        srcs/voxel/voxel_transform.cpp
    )
    add_executable(voxel_world_tests
        tests/voxel_world_tests.cpp
        ${VOXEL_WORLD_TEST_SOURCES}
    )
    target_compile_features(voxel_world_tests PRIVATE cxx_std_23)
    target_include_directories(voxel_world_tests PRIVATE ${PLAYGROUND_INCLUDE_DIR})
    target_link_libraries(voxel_world_tests PRIVATE sparkle::sparkle)
    target_compile_definitions(voxel_world_tests PRIVATE
        VOXEL_TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/voxels"
    )
    add_test(NAME voxel_world_tests COMMAND voxel_world_tests)

	add_executable(first_person_controller_tests
		tests/first_person_controller_tests.cpp
		srcs/behaviour/first_person_controller.cpp
	)
	target_compile_features(first_person_controller_tests PRIVATE cxx_std_23)
	target_include_directories(first_person_controller_tests PRIVATE ${PLAYGROUND_INCLUDE_DIR})
	target_link_libraries(first_person_controller_tests PRIVATE sparkle::sparkle)
	add_test(NAME first_person_controller_tests COMMAND first_person_controller_tests)

	add_executable(camera_tests
		tests/camera_tests.cpp
		srcs/engine/camera.cpp
	)
	target_compile_features(camera_tests PRIVATE cxx_std_23)
	target_include_directories(camera_tests PRIVATE ${PLAYGROUND_INCLUDE_DIR})
	target_link_libraries(camera_tests PRIVATE sparkle::sparkle)
	add_test(NAME camera_tests COMMAND camera_tests)
endif()

install(
    TARGETS Playground
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
