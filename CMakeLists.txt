project(SPRITER_PLUS_PLUS_QT)
cmake_minimum_required(VERSION 2.9)
option(COMPILE_QML_PLUGIN "Compile the qml plugin" ON)

if(${PROJECT_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fmax-errors=2 -Wall -Wno-unused-function")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SKIP_EXAMPLES ON)

include("QtCreatorResources")

#Qt
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)

add_subdirectory(spriterplusplus EXCLUDE_FROM_ALL)
add_subdirectory(qtdocumentwrapper EXCLUDE_FROM_ALL)
add_subdirectory(qt)
add_subdirectory(example)

qtcreator_add_project_resources(${SPRITER_ENGINE_SRCS} ${SPRITER_ENGINE_HDRS})
qtcreator_add_project_resources(${SPRITER_EXAMPLE_SRCS} ${SPRITER_EXAMPLE_HDRS})
