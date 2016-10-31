project(QT_SPRITER)

set(SPRITER_PLUGIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Spriter")

add_subdirectory(data)

include_directories(${SPRITER_ENGINE_SOURCE_DIR})
include_directories(${SPRITER_PLUS_PLUS_SOURCE_DIR})
include_directories(${QT_DOCUMENT_WRAPPER_SOURCE_DIR})

set(QT_SRCS
    qtspritermodel.cpp
    qtfilefactory.cpp
    qtobjectfactory.cpp
    qtimagefile.cpp
    qtsoundfile.cpp
    qtatlasfile.cpp
    qtsoundobjectinforeference.cpp
    qtboneinstanceinfo.cpp
    qtboxinstanceinfo.cpp
    qtpointinstanceinfo.cpp
    qtentityinstance.cpp
    qsgspriterbase.cpp
    qsgspriterobjectnode.cpp
    qsgspriterbonenode.cpp
    qsgspriterpointnode.cpp)

set(QT_HDRS
    qtspritermodel.h
    qtfilefactory.h
    qtobjectfactory.h
    qtimagefile.h
    qtsoundfile.h
    qtatlasfile.h
    qtsoundobjectinforeference.h
    qtboneinstanceinfo.h
    qtboxinstanceinfo.h
    qtpointinstanceinfo.h
    qtentityinstance.h
    qsgspriterbase.h
    qsgspriterobjectnode.h
    qsgspriterbonenode.h
    qsgspriterpointnode.h)

set(QT_PLUGIN_SRCS
    spriterplugin.cpp)

add_library(spriterplusplusqt STATIC ${QT_MOC_SRCS} ${QT_SRCS})

target_link_libraries(spriterplusplusqt Qt5::Quick Qt5::Widgets Qt5::Multimedia spriterengine spriterplusplusqtdoc)

if(COMPILE_QML_PLUGIN)
    add_library(qmlspriterplugin SHARED ${QT_PLUGIN_SRCS})
    target_link_libraries(qmlspriterplugin spriterplusplusqt)
    set_target_properties(qmlspriterplugin PROPERTIES
             LIBRARY_OUTPUT_DIRECTORY ${SPRITER_PLUGIN_DIR})
endif(COMPILE_QML_PLUGIN)
