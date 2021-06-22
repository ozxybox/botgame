cmake_minimum_required(VERSION 3.0)

project(glad)

add_library(glad STATIC ${CMAKE_CURRENT_LIST_DIR}/src/glad.c ${CMAKE_CURRENT_LIST_DIR}/include/glad/glad.h ${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/include)
