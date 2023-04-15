cmake_minimum_required(VERSION 3.0.0)
project(Atlas VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*")

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(PROJECT_LIBS
        ${PROJECT_LIBS}
        ${CMAKE_DL_LIBS}
        OpenGL::GL
        glfw
        GLEW::GLEW)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
