cmake_minimum_required(VERSION 2.8)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(visua)

set(lib_path "${CMAKE_CURRENT_LIST_DIR}/../lib")
set(OpenCV_DIR "${lib_path}/opencv/build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("${lib_path}/glm")

# GLFW
if(MSVC11)
    set(GLFW_LIB_DIR "${lib_path}/glfw-3.1.2/lib-vc2012")
elseif(MSVC12)
    set(GLFW_LIB_DIR "${lib_path}/glfw-3.1.2/lib-vc2013")
elseif(MSVC14)
    set(GLFW_LIB_DIR "${lib_path}/glfw-3.1.2/lib-vc2015")
endif()
if (NOT GLFW_LIB_DIR)
    message(FATAL_ERROR "GLFW not found for current platform")
endif()
include_directories("${lib_path}/glfw-3.1.2/include")
link_directories("${GLFW_LIB_DIR}")

# GLEW
include_directories("${lib_path}/glew-1.13.0/include")
link_directories("${lib_path}/glew-1.13.0/lib/Release/x64")

# SOIL (Simple OpenGL Image Library)
include_directories("${lib_path}/Simple OpenGL Image Library/src")
link_directories("${lib_path}/Simple OpenGL Image Library/lib")

# Add subdirectories for individual projects
add_subdirectory("01-simple-glfw")
add_subdirectory("02-opengl-primitives")
add_subdirectory("02-2d-plot")
add_subdirectory("02-3d-gauss")
add_subdirectory("03-3d-plot")
add_subdirectory("04-shaders")
add_subdirectory("04-texture-mapping")
