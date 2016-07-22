project(SPRITER_QT_EXAMPLE)
set(EXECUTABLE_NAME SpriterQtExample)

if(COMPILE_QML_PLUGIN)
    add_definitions(-DCOMPILE_QML_PLUGIN)
endif(COMPILE_QML_PLUGIN)

add_custom_target(SPRITER_QT_EXAMPLE ALL)
add_definitions(-DQT_QML_DEBUG)

file(COPY ${SPRITER_PLUS_PLUS_SOURCE_DIR}/Release/GreyGuy
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/GreyGuyAtlas
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

INCLUDE_DIRECTORIES(${QT_SPRITER_SOURCE_DIR})

set(EXAMPLE_SRCS
    main.cpp)

set(QML_SRCS
    main.qml)

file(GLOB_RECURSE GrayGuyFiles ${SPRITER_PLUS_PLUS_SOURCE_DIR}/Release/GreyGuy/*)

qtcreator_add_project_resources(${GrayGuyFiles})

qtcreator_add_project_resources(${QML_SRCS})

add_executable(${EXECUTABLE_NAME} ${EXAMPLE_SRCS})

foreach(_file ${QML_SRCS})
add_custom_command(
    TARGET SPRITER_QT_EXAMPLE
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${_file} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_file}
    DEPENDS ${_file})
endforeach()

target_link_libraries(${EXECUTABLE_NAME} spriterplusplusqt)
