cmake_minimum_required(VERSION 3.0.0)
project(Atlas VERSION 0.1.0)

include(CTest)
enable_testing()

file (GLOB srcFiles
    "src/*.hpp"
    "src/*.cpp"
)

add_executable(Atlas ${srcFiles} src/NetworkController.cpp src/Atlas.cpp)
target_include_directories(Atlas PUBLIC src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
