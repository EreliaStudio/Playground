cmake_minimum_required(VERSION 3.25)

project(Playground VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(sparkle 0.1.3 CONFIG REQUIRED COMPONENTS Core Graphics)

add_executable(Playground srcs/main.cpp)
target_compile_features(Playground PRIVATE cxx_std_23)
target_link_libraries(Playground PRIVATE sparkle::sparkle)
set_target_properties(Playground PROPERTIES CXX_EXTENSIONS OFF)

install(TARGETS Playground RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
