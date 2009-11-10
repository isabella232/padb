#pragma once
#include <QObject>
#include <QPointer>
class QgisInterface;
class QWidget;
class QToolBar;
class QAction;
class QgsMapTool;
#include "pa_object_list.h"
#include "pa_element.h"
#include "free_elements_controller.h"
#include "objects_tree_controller.h"
#include "map_pa_elements_list.h"

namespace pa_db
{
	class select_object_tool;
	class pa_manager : public QObject
	{
		Q_OBJECT
	public:
		pa_manager( QgisInterface *the_interface, QObject *parent = 0 );
		virtual ~pa_manager( );
		void show_object_menu( const QPoint &pos, int object_id );

		void move_free_element_to_obj( const pa_element &el, int object_id );
		void move_element_from_obj_to_free( const pa_element &el );
		void move_element_from_obj_to_other( const pa_element &el, int object_id );
		void delete_object( int object_id );

		QgisInterface *qgis_iface( ) const;
		void select( const pa_element &el, bool zoom = false );
		void select( int object_id, bool zoom = false );
		bool is_element_free( const pa_element &el ) const;
		bool is_element_valid( const pa_element &el ) const;

		bool object_has_elements( int object_id ) const;

		const pa_object* create_object( const QString &obj_name = QString( ) );

		void try_show_object_properties( const QString &layer_id, int feature_id );

	private Q_SLOTS:
		void on_load_pa_objects_list( );
		void on_create_pa_object( );
		void on_show_settings( );
		void on_about( );
		void on_select_object( );
		void on_map_tool_set( QgsMapTool *tool );
//		void on_change_layers_count( );
		void on_map_all_objects( );

	private:
		void create_tool_bar( );
		void update_widget( );
		void show_properties_form_ALTER( int object_id );
		void map_object_data_to_shape( int object_id );
		QPointer< QAction > create_plugin_menu_action( const QString &text, const char *sl );

		QgisInterface				*m_qgis_iface;
		QPointer< QWidget >			m_docker;
		QPointer< QToolBar >		m_tool_bar;
		QPointer< QAction >			m_info_action;
		QPointer< QAction >			m_settings_action;
		QPointer< QAction >			m_action_select;
		QPointer< QAction >			m_ac_map_all_objs;
		QPointer< select_object_tool > m_tool_select;
		pa_object_list				m_objects_list;
		map_pa_elements_list		m_elements_list;
		free_elements_controller	m_free_elements_cntrl;
		objects_tree_controller		m_objects_tree;
	};
}
