add_executable(04-texture-mapping
    main.cpp
    controls.hpp
    controls.cpp
    shader.cpp
    shader.hpp
    texture.cpp
    texture.hpp
    transform.vert
    texture.frag
)
target_link_libraries(04-texture-mapping glew32 glfw3 opengl32 ${OpenCV_LIBS} SOIL)

add_custom_command(TARGET 04-texture-mapping POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${lib_path}/glew-1.13.0/bin/Release/x64/glew32.dll"
        $<TARGET_FILE_DIR:04-texture-mapping>/..)
add_custom_command(TARGET 04-texture-mapping POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/transform.vert"
        $<TARGET_FILE_DIR:04-texture-mapping>/..)
add_custom_command(TARGET 04-texture-mapping POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/texture.frag"
        $<TARGET_FILE_DIR:04-texture-mapping>/..)
add_custom_command(TARGET 04-texture-mapping POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/panda.jpg"
        $<TARGET_FILE_DIR:04-texture-mapping>/..)
