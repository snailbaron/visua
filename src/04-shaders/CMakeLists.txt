add_executable(04-shaders main.cpp shader.cpp)
target_link_libraries(04-shaders LINK_PUBLIC glew32s glfw3 opengl32 ${OpenCV_LIBS} SOIL)
