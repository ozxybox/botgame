
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( SOURCES 
	main.cpp
	gameinfo.h

	baseentity.cpp
	baseentity.h

	transform.cpp
	transform.h
	
	shadermanager.cpp
	shadermanager.h

	log.cpp
	log.h

	primitivedraw.cpp
	primitivedraw.h

	renderer.cpp
	renderer.h

	basesystem.h

	materials.cpp
	materials.h

	textures.cpp
	textures.h

	imguibackend.cpp
	imguibackend.h
)

set( SHADERS
	cube.vs
	cube.fs
)




include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
add_executable( game ${SOURCES} ${SHADERS} )
target_include_directories( game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( game SDL2 glad glm lodepng imgui )
set_target_properties( game PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)

file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR})


foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_PATH "${SHADER}" ABSOLUTE)

    add_custom_command(TARGET game PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SHADER_PATH}" "${CMAKE_BINARY_DIR}/${SHADER}")
endforeach()