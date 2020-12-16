cmake_minimum_required(VERSION 3.12)
project(CS211HW4)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
)

add_executable(hw4
    main.cpp
    tiny_obj_loader.cpp
)
target_link_libraries(hw4
    GLUT::GLUT
    GLEW::glew
    OpenGL::GL
    OpenGL::GLU
    OpenGL::GLX
)