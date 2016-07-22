project(QT_XML_DOC)

include_directories(${SPRITER_ENGINE_SOURCE_DIR})
include_directories(${SPRITER_PLUS_PLUS_SOURCE_DIR})

find_package(Qt5Xml REQUIRED)

set(QTDOC_SRCS
    qtxmlspriterfileattributewrapper.cpp
    qtxmlspriterfileelementwrapper.cpp
    qtxmlspriterfiledocumentwrapper.cpp
set(QTDOC_HDRS
    qtxmlspriterfiledocumentwrapper.h
    qtxmlspriterfileattributewrapper.h
    qtxmlspriterfileelementwrapper.h

add_library(spriterplusplusqtdoc STATIC ${QTDOC_MOC_SRCS} ${QTDOC_SRCS})

target_link_libraries(spriterplusplusqtdoc Qt5::Xml spriterengine)
