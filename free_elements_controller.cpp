#include "free_elements_controller.h"
#include <QListWidget>
#include <QMouseEvent>
#include <QBuffer>
#include <QUrl>
#include "resources.h"
#include "pa_manager.h"
#include "free_elements_widget.h"

pa_db::free_elements_controller::free_elements_controller( pa_manager *manager )
	: QObject( manager )
	, m_manager( manager )
{
}

void pa_db::free_elements_controller::set_list_widget( free_elements_widget *widget )
{
	m_view = widget;
	connect( m_view, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ),
		SLOT( on_element_double_click( QListWidgetItem* ) ) );
	m_view->set_controller( this );
}

void pa_db::free_elements_controller::on_element_double_click( QListWidgetItem *item )
{
	if ( item )
		m_manager->select( pa_element::from_byte_array( item->data( Qt::UserRole ).toByteArray( ) ) );
}

void pa_db::free_elements_controller::render( const QVector< pa_element > &elements )
{
	if ( m_view )
	{
		m_view->clear( );
		for ( int nn = 0; nn < elements.count( ); ++nn )
		{
			QListWidgetItem *item = new QListWidgetItem;
			item->setData( Qt::UserRole, elements[ nn ].to_byte_array( ) );
			QString text = elements[ nn ].feature_type( );
			item->setText( elements[ nn ].feature_type( ) );
			item->setIcon( QPixmap( pa_db::icon::oopt_element_unlinked ) );
			m_view->addItem( item );
		}
	}
}

void pa_db::free_elements_controller::drop_element( const pa_element &el )
{
	m_manager->move_element_from_obj_to_free( el );
}
