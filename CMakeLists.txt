cmake_minimum_required( VERSION 3.0 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build )

project( game )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory( thirdparty/SDL2 )
add_subdirectory( thirdparty/glm )
add_subdirectory( thirdparty/glad )

include_directories( thirdparty )
add_subdirectory( game )
