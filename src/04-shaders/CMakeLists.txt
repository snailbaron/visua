add_executable(04-shaders main.cpp shader.cpp shader.hpp simple.frag simple.vert)
target_link_libraries(04-shaders LINK_PUBLIC glew32 glfw3 opengl32 ${OpenCV_LIBS} SOIL)

add_custom_command(TARGET 04-shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${lib_path}/glew-1.13.0/bin/Release/x64/glew32.dll"
        $<TARGET_FILE_DIR:04-shaders>)
add_custom_command(TARGET 04-shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/simple.vert"
        $<TARGET_FILE_DIR:04-shaders>/..)
add_custom_command(TARGET 04-shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/simple.frag"
        $<TARGET_FILE_DIR:04-shaders>/..)
