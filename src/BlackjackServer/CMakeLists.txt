cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(BlackjackServer)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")

file(GLOB models
	"./Models/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Models/*.h")
file(GLOB views
	"./Views/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Views/*.hpp")
file(GLOB controllers
	"./Controllers/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/Controllers/*.hpp")

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

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/includes/
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/)

target_link_libraries(${PROJECT_NAME} PUBLIC
	NetworkTools
	Services
	Requests
	SocketConnection
	DataLayer
	BlackjackLogic
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)

# Flags
String(CONCAT Flags ${Flags} " " "-Wall")
String(CONCAT Flags ${Flags} " " "-Wextra")
String(CONCAT Flags ${Flags} " " "-Werror")
String(CONCAT Flags ${Flags} " " "-Waddress")
Set(CMAKE_CXX_FLAGS "${Flags}")