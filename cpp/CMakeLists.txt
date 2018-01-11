cmake_minimum_required (VERSION 3.9)
project (bankers)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set (CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wformat -pedantic \
     -Iimgui -Iimgui/examples/libs/gl3w -Limgui/examples/libs/gl3w -lGL \
    ")
# set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Iimgui/examples/libs/gl3w -lGL -lglfw")
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB IMGUISOURCES "imgui/*")
file(GLOB_RECURSE GL3WSOURCES "imgui/examples/libs/gl3w/*.cpp")
add_executable(bankers ${SOURCES} ${IMGUISOURCES} ${GL3WSOURCES})
target_link_libraries(bankers glfw)

