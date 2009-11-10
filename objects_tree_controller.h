#pragma once
#include <QObject>
#include <QPointer>
class QgisInterface;
class QPoint;
class QTreeWidgetItem;
class QMimeData;

namespace pa_db
{
	class pa_object;
	class pa_object_list;
	class pa_manager;
	class objects_tree_widget;
	class pa_element;
	class pa_species;
	class objects_tree_controller : public QObject
	{
		Q_OBJECT
		public:
			objects_tree_controller( QgisInterface *the_interface, pa_manager *parent );
			void add_object( const pa_object &object );
			void delete_object( int object_id );
			void update_object_name( int object_id, const QString &name );
			
			void add_element( int object_id, const pa_element &el );
			void delete_element( int object_id, const pa_element &el );
			void on_key_clicked( int key );
			void on_double_click( );
			//for adding species
			void addSpecies( int objectId, const pa_species &el );
			void deleteSpecies( int objectId, const pa_species &el );
			
			void set_tree_widget( objects_tree_widget *widget );
			bool drop_element( QTreeWidgetItem *parent, const QMimeData *data );
			
			void update_icons( );
			
			void render( const pa_object_list &objects );
			
		private Q_SLOTS:
			void show_menu( const QPoint& pos );
			
		private:
			int find_object( int object_id ) const;
		
		QgisInterface					*m_qgis_iface;
		QPointer< objects_tree_widget >	m_view;
		pa_manager						*m_manager;
	};
}
