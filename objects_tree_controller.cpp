#include "objects_tree_controller.h"
#include "objects_tree_widget.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QEvent>
#include <QDragEnterEvent>
#include <QBuffer>
#include "resources.h"
#include "pa_object.h"
#include "pa_manager.h"

namespace { namespace aux {
	const int working_column = 0;
} }

pa_db::objects_tree_controller::objects_tree_controller( QgisInterface *the_interface, pa_manager *parent )
	: QObject( parent )
	, m_qgis_iface( the_interface )
	, m_manager( parent )
{
}

void pa_db::objects_tree_controller::add_object( const pa_object &object )
{
 	QTreeWidgetItem *obj_item = new QTreeWidgetItem;
	obj_item->setText( aux::working_column, object.name( ) );
	obj_item->setData( aux::working_column, Qt::UserRole, object.id( ) );
	m_view->addTopLevelItem( obj_item );
 	QVector< pa_element > el = object.elements( );
 	if ( el.count( ) == 0 )
 	{
 		obj_item->setIcon( aux::working_column, QPixmap( pa_db::icon::oopt_object_invalid_nolinks ) );
 	}
 	else
 	{
		bool all_is_valid = true;
		for ( int nn = 0; nn < el.count( ); ++nn )
		{
			all_is_valid = all_is_valid && m_manager->is_element_valid( el.at( nn ) );
			add_element( object.id( ), el.at( nn ) );
		}
		obj_item->setIcon( aux::working_column,
			QPixmap( all_is_valid ? pa_db::icon::oopt_object_valid : pa_db::icon::oopt_object_invalid ) );
 	}
	if ( m_view->topLevelItemCount( ) == 1 )
		m_view->setCurrentItem( obj_item, aux::working_column, QItemSelectionModel::SelectCurrent );
}

void pa_db::objects_tree_controller::set_tree_widget( pa_db::objects_tree_widget *widget )
{
	m_view = widget;
	m_view->header( )->hide( );
	m_view->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( m_view, SIGNAL( customContextMenuRequested( const QPoint& ) ),
		SLOT( show_menu( const QPoint & ) ) );
	m_view->set_controller( this );
	m_view->setSortingEnabled( true );
	m_view->sortByColumn( aux::working_column, Qt::AscendingOrder );
}

void pa_db::objects_tree_controller::show_menu( const QPoint &pos)
{
	QTreeWidgetItem *curr_item = m_view->currentItem( );
	if ( curr_item )
	{
		if ( !curr_item->parent( ) )
			m_manager->show_object_menu( m_view->mapToGlobal( pos ), curr_item->data( aux::working_column, Qt::UserRole ).toInt( ) );
	}
	else
		m_manager->show_object_menu( m_view->mapToGlobal( pos ), -1 );
}

void pa_db::objects_tree_controller::delete_object( int object_id )
{
	const int index = find_object( object_id );
	if ( index != -1 )
		delete m_view->takeTopLevelItem( index );
}

bool pa_db::objects_tree_controller::drop_element( QTreeWidgetItem *parent, const QMimeData *data )
{
	if ( parent )
	{
		QTreeWidgetItem *object_item = parent->parent( ) ? parent->parent( ) : parent;
		if ( data->hasFormat( pa_db::text::free_element_mime_data( ) ) )
		{
			const pa_element moved_el = pa_element::from_byte_array( data->data( pa_db::text::free_element_mime_data( ) ) );
			m_manager->move_free_element_to_obj( moved_el, object_item->data( aux::working_column, Qt::UserRole ).toInt( ) );
		}
		else if ( data->hasFormat( pa_db::text::link_element_mime_data( ) ) )
		{
			const pa_element moved_el = pa_element::from_byte_array( data->data( pa_db::text::link_element_mime_data( ) ) );
			m_manager->move_element_from_obj_to_other( moved_el, object_item->data( aux::working_column, Qt::UserRole ).toInt( ) );
		}
	}
	else
	{
		if ( data->hasFormat( pa_db::text::free_element_mime_data( ) ) )
		{
			const pa_element moved_el = pa_element::from_byte_array( data->data( pa_db::text::free_element_mime_data( ) ) );
			const pa_object* new_obj = m_manager->create_object( moved_el.feature_type( ) );
			if ( new_obj )
				m_manager->move_free_element_to_obj( moved_el, new_obj->id( ) );
//			m_manager->move_element_from_obj_to_other( moved_el, object_item->data( aux::working_column, Qt::UserRole ).toInt( ) );
		}
	}
	return true;
}

void pa_db::objects_tree_controller::add_element( int object_id, const pa_db::pa_element &el )
{
	const int index = find_object( object_id );
	Q_ASSERT( index != -1 );
	if ( index != -1 )
	{
		QTreeWidgetItem *el_item = new QTreeWidgetItem;
		el_item->setText( aux::working_column, el.feature_type( ) );
		el_item->setData( aux::working_column, Qt::UserRole, el.to_byte_array( ) );
		el_item->setIcon( aux::working_column, QPixmap( pa_db::icon::oopt_element_invalid ) );
		m_view->topLevelItem( index )->addChild( el_item );
	}
}

int pa_db::objects_tree_controller::find_object( int object_id ) const
{
	const int count = m_view->topLevelItemCount( );
	for ( int nn = 0; nn < count; ++nn )
		if ( m_view->topLevelItem( nn )->data( aux::working_column, Qt::UserRole ).toInt( ) == object_id )
			return nn;
	return -1;
}

void pa_db::objects_tree_controller::update_icons( )
{
	const int count = m_view->topLevelItemCount( );
	for ( int nn = 0; nn < count; ++nn )
	{
		QTreeWidgetItem *obj_item = m_view->topLevelItem( nn );
		if ( const int child_count = obj_item->childCount( ) )
		{
			bool all_is_valid = true;
			for ( int mm = 0; mm < child_count; ++mm )
			{
				QTreeWidgetItem *child = obj_item->child( mm );
				const pa_element el = pa_element::from_byte_array( child->data( aux::working_column, Qt::UserRole ).toByteArray( ) );
				if ( m_manager->is_element_valid( el ) )
				{
					child->setIcon( aux::working_column, QPixmap( pa_db::icon::oopt_element_valid ) );
				}
				else
				{
					child->setIcon( aux::working_column, QPixmap( pa_db::icon::oopt_element_invalid ) );
					all_is_valid = false;
				}
			}
			obj_item->setIcon( aux::working_column,
				QPixmap( all_is_valid ? pa_db::icon::oopt_object_valid : pa_db::icon::oopt_object_invalid ) );
		}
		else
		{
			obj_item->setIcon( aux::working_column, QPixmap( pa_db::icon::oopt_object_invalid_nolinks ) );
		}
	}
}

void pa_db::objects_tree_controller::delete_element( int object_id, const pa_element &el )
{
	const int index = find_object( object_id );
	if ( index != -1 )
	{
		QTreeWidgetItem *obj_item = m_view->topLevelItem( index );
		if ( const int child_count = obj_item->childCount( ) )
		{
			for ( int nn = 0; nn < child_count; ++nn )
			{
				QTreeWidgetItem *child = obj_item->child( nn );
				const pa_element child_el = pa_element::from_byte_array( child->data( aux::working_column, Qt::UserRole ).toByteArray( ) );
				if ( el.layer_id( ) == child_el.layer_id( ) && el.feature_id( ) == child_el.feature_id( ) )
				{
					obj_item->removeChild( child );
					return;
				}
			}
		}
	}
}

void pa_db::objects_tree_controller::on_key_clicked( int key )
{
	if ( key == Qt::Key_Delete
		&& m_view->currentItem( ) )
	{
		if ( m_view->currentItem( )->parent( ) )
		{
			const pa_element el = pa_element::from_byte_array( m_view->currentItem( )->data( aux::working_column, Qt::UserRole ).toByteArray( ) );
			m_manager->move_element_from_obj_to_free( el );
		}
		else
			m_manager->delete_object( m_view->currentItem( )->data( aux::working_column, Qt::UserRole ).toInt( ) );
	}
}

void pa_db::objects_tree_controller::on_double_click( )
{
	if ( m_view->currentItem( ) && m_view->currentItem( )->parent( ) )
	{
		const pa_element cur_el = pa_element::from_byte_array( m_view->currentItem( )->data( aux::working_column, Qt::UserRole ).toByteArray( ) );
		m_manager->select( cur_el );
	}
	else
	{
		m_manager->select( m_view->currentItem( )->data( aux::working_column, Qt::UserRole ).toInt( ), false );
	}
}

void pa_db::objects_tree_controller::update_object_name( int object_id, const QString &name )
{
	const int index = find_object( object_id );
	if ( index != -1 )
		m_view->topLevelItem( index )->setText( aux::working_column, name );
}

