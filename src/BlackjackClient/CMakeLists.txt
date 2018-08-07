cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(BlackjackClient)

set(CMAKE_CXX_STANDARD 11)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB controllers
	"./Controllers/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Controllers/*.hpp")
file(GLOB views
	"./Views/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Views/*.hpp")

file(GLOB models
	"./Models/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Models/*.hpp")

source_group("Models" FILES ${models})
source_group("Views" FILES ${views})
source_group("Controllers" FILES ${controllers})

file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp"
)

add_executable(${PROJECT_NAME}
	${srcs}
	${includes}
	
	${models}
	${views}
	${controllers}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

target_link_libraries(${PROJECT_NAME} PRIVATE
	NetworkTools
	Requests
	SocketConnection
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)

# Flags
# String(CONCAT Flags ${Flags} " " "-Wall")
# String(CONCAT Flags ${Flags} " " "-Wextra")
# String(CONCAT Flags ${Flags} " " "-Werror")
# String(CONCAT Flags ${Flags} " " "-Waddress")
# String(CONCAT Flags ${Flags} " " "-Waggregate-return")
Set(CMAKE_CXX_FLAGS "${Flags}")