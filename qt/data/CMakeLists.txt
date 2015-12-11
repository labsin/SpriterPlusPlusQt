set(QMLDIR
    qmldir)

qtcreator_add_project_resources(${QMLDIR})

if(COMPILE_QML_PLUGIN)
    file(COPY ${QMLDIR}
        DESTINATION ${SPRITER_PLUGIN_DIR})
endif(COMPILE_QML_PLUGIN)
