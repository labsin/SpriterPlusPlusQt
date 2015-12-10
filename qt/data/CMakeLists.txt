set(DATA
    qmldir)

qtcreator_add_project_resources(${DATA})

file(COPY ${DATA}
    DESTINATION ${SPRITER_PLUGIN_DIR})
