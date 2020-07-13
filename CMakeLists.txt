cmake_minimum_required(VERSION 3.17)
project(clearsdf)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw)
