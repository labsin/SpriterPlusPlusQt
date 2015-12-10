project(QT_SPRITER)

include_directories(${SPRITER_ENGINE_SOURCE_DIR})
include_directories(${SPRITER_PLUS_PLUS_SOURCE_DIR})
include_directories(${QT_XML_DOC_SOURCE_DIR})

set(QT_SRCS
    qsprintermodel.cpp
    qtfilefactory.cpp
    qtobjectfactory.cpp
    qtimagefile.cpp
    qtsoundfile.cpp
    qtsoundobjectinforeference.cpp
    qtboneinstanceinfo.cpp
    qtboxinstanceinfo.cpp
    qtpointinstanceinfo.cpp
    qentityinstance.cpp)

set(QT_HDRS
    qsprintermodel.h
    qtfilefactory.h
    qtobjectfactory.h
    qtimagefile.h
    qtsoundfile.h
    qtsoundobjectinforeference.h
    qtboneinstanceinfo.h
    qtboxinstanceinfo.h
    qtpointinstanceinfo.h
    qentityinstance.h)

add_library(spriterplusplusqt STATIC ${QT_MOC_SRCS} ${QT_SRCS})

target_link_libraries(spriterplusplusqt Qt5::Quick Qt5::Widgets Qt5::Multimedia spriterengine spriterplusplusqtdoc)
