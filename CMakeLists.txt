########################################################
# Files

SET (pa_db_SRCS
     dock_widget.cpp
     free_elements_controller.cpp
     free_elements_widget.cpp
     map_pa_elements_list.cpp
     objects_tree_controller.cpp
     objects_tree_widget.cpp
     pa_db_plugin.cpp
     pa_element.cpp
     pa_species.cpp
     pa_manager.cpp
     pa_object.cpp
     pa_object_list.cpp
     properties_form.cpp
     properties_page_activity.cpp
     properties_page_base.cpp
     properties_page_common.cpp
     properties_page_docs.cpp
     properties_page_nature.cpp
     properties_page_organization.cpp
     properties_page_regime.cpp
     properties_page_importance.cpp
     properties_page_subjection.cpp
     signal_blocker.cpp
     sql_relation_combo.cpp
     utils.cpp
     properties_page_obj_table.cpp
     select_object_tool.cpp
     # settings_form.cpp
     settings.cpp
     # qgsspeciesimportdialog.cpp
     qgspadbsettingsdialog.cpp
     qgsimportspeciesdialog.cpp
     qgseditspeciesdialog.cpp
     qgsspeciespropertiesdialog.cpp
     species_widget.cpp
     pa_species_list.cpp
     species_controller.cpp
)

SET (pa_db_UIS
     # qgsspeciesimportbase.ui
     qgssettingsbase.ui
     qgsimportspeciesbase.ui
     qgseditspeciesbase.ui
     qgsspeciespropertiesbase.ui)

SET (pa_db_MOC_HDRS
     dock_widget.h
     free_elements_controller.h
     free_elements_widget.h
     objects_tree_controller.h
     objects_tree_widget.h
     pa_db_plugin.h
     pa_manager.h
     pa_object_list.h
     properties_form.h
     properties_page_activity.h
     properties_page_base.h
     properties_page_common.h
     properties_page_docs.h
     properties_page_importance.h
     properties_page_nature.h
     properties_page_organization.h
     properties_page_regime.h
     properties_page_subjection.h
     sql_relation_combo.h
     properties_page_obj_table.h
     select_object_tool.h
     # settings_form.h
     settings.h
     # qgsspeciesimportdialog.h
     qgspadbsettingsdialog.h
     qgsimportspeciesdialog.h
     qgseditspeciesdialog.h
     qgsspeciespropertiesdialog.h
     species_widget.h
     species_controller.h
)

SET (pa_db_RCCS  pa_db_plugin.qrc)

########################################################
# Build
QT4_WRAP_UI (pa_db_UIS_H  ${pa_db_UIS}) 

QT4_WRAP_CPP (pa_db_MOC_SRCS  ${pa_db_MOC_HDRS})

QT4_ADD_RESOURCES(pa_db_RCC_SRCS ${pa_db_RCCS})

ADD_LIBRARY (pa_dbplugin MODULE ${pa_db_SRCS} ${pa_db_MOC_SRCS} ${pa_db_RCC_SRCS} ${pa_db_UIS_H})

INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_BINARY_DIR}
     ../../core ../../core/raster ../../core/renderer ../../core/symbology
     ../../gui
     ..
)

TARGET_LINK_LIBRARIES(pa_dbplugin
  ${QT_QTSQL_LIBRARY} 
  qgis_core
  qgis_gui
)


########################################################
# Install

INSTALL(TARGETS pa_dbplugin
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

